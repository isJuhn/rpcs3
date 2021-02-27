#pragma once

#include "overlays.h"

namespace rsx
{
	namespace overlays
	{
		struct HLE_overlay : overlay
		{
		private:

			std::vector<std::pair<std::shared_ptr<overlay_element>, bool>> v_elements{};
			std::unordered_map<std::string, std::shared_ptr<image_info>> image_cache{};

		public:

			HLE_overlay() = default;
			~HLE_overlay() = default;

			void draw_text(const std::string& str, int x, int y, int font_size, color4f col, const std::string& font)
			{
				label text{ str.c_str() };
				text.set_font(font.c_str(), ::narrow<u16>(font_size));
				text.back_color.a = 0.f;
				text.fore_color = col;
				text.auto_resize();
				text.set_pos(::narrow<u16>(x), ::narrow<u16>(y));
				v_elements.emplace_back(std::make_shared<label>(text), 0);
			}

			void draw_square(int x, int y, int w, int h, color4f col)
			{
				overlay_element square{ ::narrow<u16>(w), ::narrow<u16>(h) };
				square.set_pos(::narrow<u16>(x), ::narrow<u16>(y));
				square.back_color = col;
				v_elements.emplace_back(std::make_shared<overlay_element>(square), 0);
			}

			void draw_image(int x, int y, int w, int h, const std::string& filename)
			{
				image_view image{ ::narrow<u16>(w), ::narrow<u16>(h) };
				image.set_pos(::narrow<u16>(x), ::narrow<u16>(y));
				image.back_color.a = 0.f;
				auto i_info = image_cache[filename];
				if (!i_info)
				{
					i_info = std::make_shared<image_info>((fs::get_config_dir() + filename).c_str());
					if (i_info->data)
					{
						image_cache[filename] = i_info;
					}
					else
					{
						return;
					}
				}
				image.set_raw_image(i_info.get());
				v_elements.emplace_back(std::make_shared<image_view>(image), 1);
			}

			void update() override
			{
				v_elements.clear();
			}

			compiled_resource get_compiled() override
			{
				compiled_resource result;

				for (auto& element : v_elements)
				{
					if (!element.second)
					{
						result.add(element.first->get_compiled());
					}
					else
					{
						result.add(dynamic_cast<image_view*>(element.first.get())->get_compiled());
					}
				}

				return result;
			}
		};
	}
}