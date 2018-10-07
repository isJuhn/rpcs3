#include "bin_patch.h"
#include <yaml-cpp/yaml.h>
#include "File.h"
#include "Config.h"
#include "./Emu/Cell/PPUModule.h"
#include "types.h"

extern void ppu_register_range(u32 addr, u32 size);

template <>
void fmt_class_string<patch_type>::format(std::string& out, u64 arg)
{
	format_enum(out, arg, [](patch_type value)
	{
		switch (value)
		{
		case patch_type::load: return "load";
		case patch_type::byte: return "byte";
		case patch_type::le16: return "le16";
		case patch_type::le32: return "le32";
		case patch_type::le64: return "le64";
		case patch_type::bef32: return "bef32";
		case patch_type::bef64: return "bef64";
		case patch_type::be16: return "be16";
		case patch_type::be32: return "be32";
		case patch_type::be64: return "be64";
		case patch_type::lef32: return "lef32";
		case patch_type::lef64: return "lef64";
		case patch_type::func: return "func";
		case patch_type::funcl: return "funcl";
		}

		return unknown;
	});
}

void patch_engine::append(const std::string& patch)
{
	if (fs::file f{patch})
	{
		YAML::Node root;

		try
		{
			root = YAML::Load(f.to_string());
		}
		catch (const std::exception& e)
		{
			LOG_FATAL(GENERAL, "Failed to load patch file %s\n%s thrown: %s", patch, typeid(e).name(), e.what());
			return;
		}

		for (auto pair : root)
		{
			auto& name = pair.first.Scalar();
			auto& data = m_map[name];

			for (auto patch : pair.second)
			{
				u64 type64 = 0;
				cfg::try_to_enum_value(&type64, &fmt_class_string<patch_type>::format, patch[0].Scalar());

				struct patch info{};
				info.type   = static_cast<patch_type>(type64);
				info.offset = patch[1].as<u32>(0);

				switch (info.type)
				{
				case patch_type::load:
				{
					// Special syntax: copy named sequence (must be loaded before)
					const auto found = m_map.find(patch[1].Scalar());

					if (found != m_map.end())
					{
						// Address modifier (optional)
						const u32 mod = patch[2].as<u32>(0);

						for (const auto& rd : found->second)
						{
							info = rd;
							info.offset += mod;
							data.emplace_back(info);
						}

						continue;
					}

					// TODO: error
					break;
				}
				case patch_type::bef32:
				case patch_type::lef32:
				{
					info.value_as<f32>() = patch[2].as<f32>();
					break;
				}
				case patch_type::bef64:
				case patch_type::lef64:
				{
					info.value_as<f64>() = patch[2].as<f64>();
					break;
				}
				case patch_type::func:
				case patch_type::funcl:
				{
					const auto& str = patch[2].Scalar();
					info.str.resize(str.size());
					info.str.assign(str);
					break;
				}
				default:
				{
					info.value = patch[2].as<u64>();
					break;
				}
				}

				data.emplace_back(info);
			}
		}
	}
}

std::size_t patch_engine::apply(const std::string& name, u8* dst)
{
	const auto found = m_map.find(name);

	if (found == m_map.cend())
	{
		return 0;
	}

	u32 funcl_count = std::count_if(found->second.cbegin(), found->second.cend(), [](auto p) { return p.type == patch_type::funcl; });

	u32 cleanup_code_addr = vm::alloc(funcl_count * 0x4 * 13, vm::main);

	ppu_register_range(cleanup_code_addr, funcl_count * 0x4 * 13);

	// Apply modifications sequentially
	for (const auto& p : found->second)
	{
		auto ptr = dst + p.offset;

		switch (p.type)
		{
		case patch_type::load:
		{
			// Invalid in this context
			break;
		}
		case patch_type::byte:
		{
			*ptr = static_cast<u8>(p.value);
			break;
		}
		case patch_type::le16:
		{
			*reinterpret_cast<le_t<u16, 1>*>(ptr) = static_cast<u16>(p.value);
			break;
		}
		case patch_type::le32:
		case patch_type::lef32:
		{
			*reinterpret_cast<le_t<u32, 1>*>(ptr) = static_cast<u32>(p.value);
			break;
		}
		case patch_type::le64:
		case patch_type::lef64:
		{
			*reinterpret_cast<le_t<u64, 1>*>(ptr) = static_cast<u64>(p.value);
			break;
		}
		case patch_type::be16:
		{
			*reinterpret_cast<be_t<u16, 1>*>(ptr) = static_cast<u16>(p.value);
			break;
		}
		case patch_type::be32:
		case patch_type::bef32:
		{
			*reinterpret_cast<be_t<u32, 1>*>(ptr) = static_cast<u32>(p.value);
			break;
		}
		case patch_type::be64:
		case patch_type::bef64:
		{
			*reinterpret_cast<be_t<u64, 1>*>(ptr) = static_cast<u64>(p.value);
			break;
		}
		case patch_type::func:
		case patch_type::funcl:
		{
			u32 addr{};
			const auto& funcs = ppu_module_manager::patchModule.functions;
			for (const auto& func : funcs)
			{
				if (p.str.compare(func.second.name) == 0)
				{
					addr = *func.second.export_addr;
					break;
				}
			}
			if (!addr)
			{
				LOG_ERROR(PPU, "Failed to patch function at 0x%x with HLE function %s", p.offset, p.str);
				break;
			}

			if (p.type == patch_type::funcl)
			{
				cleanup_code_map[(u32)ptr] = cleanup_code_addr;

				vm::write32(cleanup_code_addr, 0xE8010000); // ld r0, 0x0(r1)
				vm::write32(cleanup_code_addr + 0x4, 0x7C0803A6); // mtlr r0
				vm::write32(cleanup_code_addr + 0x8, 0xE8010008); // ld r0, 0x8(r1)
				vm::write32(cleanup_code_addr + 0xC, 0x38210010); // addi r1, r1, 0x10
				vm::write32(cleanup_code_addr + 0x10, *reinterpret_cast<be_t<u32>*>(ptr));
				vm::write32(cleanup_code_addr + 0x14, *reinterpret_cast<be_t<u32>*>(ptr + 0x4));
				vm::write32(cleanup_code_addr + 0x18, *reinterpret_cast<be_t<u32>*>(ptr + 0x8));
				vm::write32(cleanup_code_addr + 0x1C, *reinterpret_cast<be_t<u32>*>(ptr + 0xC));
				vm::write32(cleanup_code_addr + 0x20, *reinterpret_cast<be_t<u32>*>(ptr + 0x10));
				vm::write32(cleanup_code_addr + 0x24, *reinterpret_cast<be_t<u32>*>(ptr + 0x14));
				vm::write32(cleanup_code_addr + 0x28, *reinterpret_cast<be_t<u32>*>(ptr + 0x18));
				vm::write32(cleanup_code_addr + 0x2C, *reinterpret_cast<be_t<u32>*>(ptr + 0x1C));
				vm::write32(cleanup_code_addr + 0x30, 0x4E800420); // bcctr 20, 0, 0

				cleanup_code_addr += 0x34;

				*reinterpret_cast<be_t<u32, 1>*>(ptr) = u32{ 0xF821FFF1 }; // stdu r1, -0x10(r1)
				*reinterpret_cast<be_t<u32, 1>*>(ptr + 0x4) = u32{ 0xF8010008 }; // std r0, 0x8(r1)
				*reinterpret_cast<be_t<u32, 1>*>(ptr + 0x8) = u32{ 0x7C0802A6 }; // mflr r0
				*reinterpret_cast<be_t<u32, 1>*>(ptr + 0xC) = u32{ 0xF8010000 }; // std r0, 0x0(r1)

				ptr += 0x10;
			}

			*reinterpret_cast<be_t<u32, 1>*>(ptr) = u32{ 0x3C000000 | (addr >> 16) }; // lis r0, addr_h
			*reinterpret_cast<be_t<u32, 1>*>(ptr + 0x4) = u32{ 0x60000000 | (addr & 0xFFFF) }; // ori r0, addr_l(r0)
			*reinterpret_cast<be_t<u32, 1>*>(ptr + 0x8) = u32{ 0x7C0903A6 }; // mtctr r0
			*reinterpret_cast<be_t<u32, 1>*>(ptr + 0xC) = p.type == patch_type::func ? u32{ 0x4E800420 } : u32{ 0x4E800421 }; //bcctr(l) 20, 0, 0
			LOG_NOTICE(LOADER, "Patched function at 0x%x with HLE function %s", p.offset, p.str);
			break;
		}
		}
	}

	return found->second.size();
}

std::unordered_map<u32, u32>& patch_engine::get_HLE_rets()
{
	return cleanup_code_map;
}
