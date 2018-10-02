#include "bin_patch.h"
#include <yaml-cpp/yaml.h>
#include "File.h"
#include "Config.h"
#include "./Emu/Cell/PPUModule.h"

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

std::size_t patch_engine::apply(const std::string& name, u8* dst) const
{
	const auto found = m_map.find(name);

	if (found == m_map.cend())
	{
		return 0;
	}

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
		{
			//LOG_ERROR(PPU, "patch func, offset=0x%x, type=0x%x, value=0x%x, value=%s", p.offset, p.type, p.value, p.str);
			u32 addr{};
			const auto& funcs = ppu_module_manager::patchModule.functions;
			for (const auto& func : funcs)
			{
				//LOG_ERROR(PPU, "func.first=0x%x, func.second.name=%s, func.second.export_addr=0x%x", func.first, func.second.name, func.second.export_addr);
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
			*reinterpret_cast<be_t<u32, 1>*>(ptr) = u32{ 0x3C000000 | (addr >> 16) };
			*reinterpret_cast<be_t<u32, 1>*>(ptr + 4) = u32{ 0x60000000 | (addr & 0xFFFF) };
			*reinterpret_cast<be_t<u32, 1>*>(ptr + 8) = u32{ 0x7C0903A6 };
			*reinterpret_cast<be_t<u32, 1>*>(ptr + 12) = u32{ 0x4E800420 };
			*reinterpret_cast<be_t<u32, 1>*>(ptr + 16) = u32{ 0x60000000 };
			LOG_NOTICE(LOADER, "Patched function at 0x%x with HLE function %s", p.offset, p.str);
			break;
		}
		}
	}

	return found->second.size();
}
