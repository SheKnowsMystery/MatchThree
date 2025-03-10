module;

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <span>
#include <string>
#include <concepts>

export module components.sdl:context;

import utility;

import :bindings;
import :exceptions;
import :frame;
import :pools;

export
namespace sdl
{
	namespace type
	{
		template <std::convertible_to<std::string> _Str>
		using textures = std::span<_Str>;
		template <util::concepts::function<void, Frame, TexturePool> _Func>
		using function = _Func;
	}

	struct WindowData
	{
		std::string title;
		Rect rect;
		struct {
			Uint32 sdl;
			Uint32 img;
			Uint32 window;
			Uint32 renderer;
		} flags;
	};

	template <typename _Str, typename _Func>
	void context(const WindowData& _window_data, type::textures<_Str> _textures, type::function<_Func> _function)
	{
		Window*   window   = nullptr;
		Renderer* renderer = nullptr;

#ifdef _DEBUG
		SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
#else
		SDL_LogSetAllPriority(SDL_LOG_PRIORITY_ERROR);
#endif

		try
		{
			if (int error = SDL_Init(_window_data.flags.sdl))
				throw ErrorSDL(error);
			if (int flags = IMG_Init(_window_data.flags.img); flags ^ _window_data.flags.img)
				throw ErrorIMG();

			window = SDL_CreateWindow(_window_data.title.data(), _window_data.rect.x, _window_data.rect.y, _window_data.rect.w, _window_data.rect.h, _window_data.flags.window);
			if (!window)
				throw ErrorSDL();

			renderer = SDL_CreateRenderer(window, -1, _window_data.flags.renderer);
			if (!renderer)
				throw ErrorSDL();

			if (int error = SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND))
				throw ErrorSDL(error);

			Point size = {.x = _window_data.rect.w, .y = _window_data.rect.h};

			auto frame = Frame(window, renderer, size);
			auto loader = TextureLoader(renderer, _textures);
			auto pool = TexturePool(loader);

			_function(frame, pool);
		}
		catch (const ErrorBase& error)
		{
			SDL_LogError(error.code, error.what());
		}
		if (renderer)
			SDL_DestroyRenderer(renderer);
		if (window)
			SDL_DestroyWindow(window);

		IMG_Quit();
		SDL_Quit();
	}
} // namespace sdl