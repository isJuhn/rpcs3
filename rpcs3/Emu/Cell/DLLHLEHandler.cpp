#include "stdafx.h"

#include "Emu/IdManager.h"
#include "Emu/Cell/PPUFunction.h"
#include "PPUCallback.h"
#include "Emu/System.h"
#include "Emu/RSX/Overlays/overlay_hle.h"
#include <Windows.h>

LOG_CHANNEL(dll_hle);

using dll_function_t = void(__cdecl*)(ppu_thread&);
using dll_init_t = void(__cdecl*)(const std::vector<void*>&);

std::vector<dll_function_t> g_func_table{};
std::unordered_map<std::string, u64> g_name_to_index_map{};
std::unordered_map<std::string, HINSTANCE> g_name_to_dll{};
std::unordered_map<u32, u32> g_addr_to_old_op{};

extern void init_dll(const std::string& dll, const std::string& hash)
{
	char szFullPath[MAX_PATH] = {};
	GetCurrentDirectoryA(MAX_PATH, szFullPath);
	strcat_s(szFullPath, fmt::format("\\%s", dll).c_str());
	g_name_to_dll[dll] = LoadLibraryA(szFullPath);

	if (!g_name_to_dll[dll])
	{
		dll_hle.error("Could not load HLE dynamic library %s", szFullPath);
		return;
	}
	dll_hle.warning("Loaded HLE dynamic library %s", szFullPath);

	dll_init_t init_dll = (dll_init_t)GetProcAddress(g_name_to_dll[dll], "init_dll");
	if (!init_dll)
	{
		dll_hle.error("Could not locate dll_init function");
		return;
	}

	std::function<void(u32 addr, be_t<u32> value)> write32 = vm::write32;
	std::function<const be_t<u32>& (u32 addr)> read32 = vm::read32;
	std::function<u32(ppu_thread&, u32 size, u32 align)> stack_alloc = [](ppu_thread& ppu, u32 size, u32 align) {return ppu.stack_push(size, align); };
	std::function<void(ppu_thread&, u32 addr, u32 size)> stack_dealloc = [](ppu_thread& ppu, u32 addr, u32 size) {ppu.stack_pop_verbose(addr, size); };
	std::function<void(ppu_thread&, u32 addr)> do_call = [](ppu_thread& ppu, u32 addr) {vm::ptr<void(void)> ptr{ vm::addr_t{ addr } }; ptr(ppu); };

	std::function<void(const std::string& text, int x, int y, int font_size, color4f color, const std::string& font)> draw_text =
		[](const std::string& text, int x, int y, int font_size, color4f color, const std::string& font) {
		if (auto manager = g_fxo->get<rsx::overlays::display_manager>())
		{
			manager->get<rsx::overlays::HLE_overlay>()->draw_text(text, x, y, font_size, color, font);
		}
	};

	std::function<void(int x, int y, int w, int h, color4f col)> draw_square = [](int x, int y, int w, int h, color4f col) {
		if (auto manager = g_fxo->get<rsx::overlays::display_manager>())
		{
			manager->get<rsx::overlays::HLE_overlay>()->draw_square(x, y, w, h, col);
		}
	};

	std::function<void(int x, int y, int w, int h, const std::string& filename)> draw_image = [](int x, int y, int w, int h, const std::string& filename) {
		if (auto manager = g_fxo->get<rsx::overlays::display_manager>())
		{
			manager->get<rsx::overlays::HLE_overlay>()->draw_image(x, y, w, h, filename);
		}
	};

	std::vector<void*> table{ (void*)&vm::g_base_addr, (void*)&hash, &write32, &read32, &stack_alloc, &stack_dealloc, &do_call, &draw_text, &draw_square, &draw_image };
	init_dll(table);
}

extern u64 register_function(const std::string& dll, const std::string& name)
{
	if (g_name_to_index_map.find(name) != g_name_to_index_map.end())
	{
		return g_name_to_index_map[name];
	}

	dll_function_t func = (dll_function_t)GetProcAddress(g_name_to_dll[dll], name.c_str());
	if (!func)
	{
		dll_hle.error("Could not locate dll HLE function %s", name);
		return -1ull;
	}

	g_func_table.push_back(func);
	u64 index = g_func_table.size() - 1;
	g_name_to_index_map[name] = index;

	dll_hle.warning("Registered dll HLE function %s with index 0x%x", name, index);

	return index;
}

extern void execute_HLE(ppu_thread& ppu, u64 code, bool lk)
{
	if (code < g_func_table.size())
	{
		auto func = g_func_table[code];
		const auto old_f = ppu.last_function;
		ppu.last_function = fmt::format("dll HLE #0x%x", code).c_str();
		func(ppu);
		ppu.last_function = old_f;
		if (!lk)
		{
			ppu.cia = ppu.lr;
		}
		dll_hle.trace("dll HLE function 0x%x finished, r3=0x%llx", code, ppu.gpr[3]);
		return;
	}

	fmt::throw_exception("Invalid dll HLE function number (0x%x)", code);
}
