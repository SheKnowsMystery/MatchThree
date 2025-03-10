module;

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>

#include <concepts>
#include <map>
#include <span>
#include <string>

export module components.sdl:pools;

import :bindings;
import :exceptions;

export
namespace sdl
{
	class TextureLoader
	{
		Renderer* renderer = nullptr;
		std::map<std::string, Texture*> dict;

		static auto filename(const std::string& _identifier) -> std::string
		{
			return "./assets/" + _identifier + ".png";
		}
	public:
		template <std::convertible_to<std::string> _Str>
		TextureLoader(Renderer* _renderer, std::span<_Str> _list) : renderer(_renderer)
		{
			for (const std::string& identifier : _list)
				add(identifier, filename(identifier));
		};
		~TextureLoader()
		{
			for (auto& [filename, texture] : dict)
				SDL_DestroyTexture(texture);
		}

		void add(const std::string& _identifier, const std::string& _filename)
		{
			Texture* texture = IMG_LoadTexture(renderer, _filename.data());
			if (!texture)
				throw ErrorIMG();
			dict[_identifier] = texture;
		}
		auto operator[](const std::string& _identifier) const -> Texture*
		{
			return dict.contains(_identifier) ? dict.at(_identifier) : nullptr;
		}
	};

	class TexturePool
	{
		const TextureLoader& loader;

	public:
		TexturePool(const TextureLoader& _loader) : loader(_loader) {}

		TexturePool(const TexturePool&) = default;

		auto operator[](const std::string& _identifier) const -> Texture*
		{
			return loader[_identifier];
		}
	};
}