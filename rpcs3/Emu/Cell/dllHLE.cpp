#include "stdafx.h"

#include "Emu/Cell/PPUFunction.h"
#include "PPUCallback.h"
#include "Emu/System.h"
#include <Windows.h>

using dll_function_t = void(__cdecl*)(ppu_thread&);
using dll_init_t = void(__cdecl*)(const std::vector<void*>&);

std::vector<dll_function_t> g_func_table{};
std::unordered_map<std::string, u64> g_name_to_index_map{};
std::unordered_map<std::string, HINSTANCE> g_name_to_dll{};

extern void init_dll(const std::string& dll, const std::string& hash)
{
	char szFullPath[MAX_PATH] = {};
	GetCurrentDirectoryA(MAX_PATH, szFullPath);
	strcat_s(szFullPath, fmt::format("\\%s", dll).c_str());
	g_name_to_dll[dll] = LoadLibraryA(szFullPath);

	if (!g_name_to_dll[dll])
	{
		LOG_ERROR(PPU, "Could not load HLE dynamic library %s", szFullPath);
		return;
	}
	LOG_WARNING(PPU, "Loaded HLE dynamic library %s", szFullPath);

	dll_init_t init_dll = (dll_init_t)GetProcAddress(g_name_to_dll[dll], "init_dll");
	if (!init_dll)
	{
		LOG_ERROR(PPU, "Could not locate dll_init function");
		return;
	}

	std::function<void(u32, be_t<u32>)> write32 = vm::write32;
	std::function<const be_t<u32>&(u32)> read32 = vm::read32;
	std::function<u32(ppu_thread&, u32, u32)> stack_alloc = [](ppu_thread& ppu, u32 size, u32 align) {return ppu.stack_push(size, align); };
	std::function<void(ppu_thread&, u32, u32)> stack_dealloc = [](ppu_thread& ppu, u32 addr, u32 size) {ppu.stack_pop_verbose(addr, size); };
	std::function<void(ppu_thread&, u32)> do_call = [](ppu_thread& ppu, u32 addr) {vm::ptr<void(void)> ptr{ vm::addr_t{ addr } }; ptr(ppu); };

	std::vector<void*> table{ (void*)&vm::g_base_addr, (void*)&hash, &write32, &read32, &stack_alloc, &stack_dealloc, &do_call };
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
		LOG_ERROR(PPU, "Could not locate dll HLE function %s", name);
		return -1ull;
	}

	g_func_table.push_back(func);
	u64 index = g_func_table.size() - 1;
	g_name_to_index_map[name] = index;

	LOG_WARNING(PPU, "Registered dll HLE function %s with index 0x%x", name, index);

	return index;
}

extern void execute_HLE(ppu_thread& ppu, u64 code, bool lk)
{
	if (code < g_func_table.size())
	{
		auto func = g_func_table[code];
		if (lk)
		{

		}
		else
		{
			const auto old_f = ppu.last_function;
			ppu.last_function = fmt::format("dll HLE #0x%x", code).c_str();
			func(ppu);
			ppu.last_function = old_f;
			ppu.cia = ppu.lr;
			LOG_TRACE(PPU, "dll HLE function 0x%x finished, r3=0x%llx", code, ppu.gpr[3]);
			return;
		}
	}

	fmt::throw_exception("Invalid dll HLE function number (0x%x)", code);
}