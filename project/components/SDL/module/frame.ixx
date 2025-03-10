module;

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>

#include <concepts>
#include <initializer_list>

export module components.sdl:frame;

import :bindings;
import :exceptions;
import :drawable;

namespace sdl::impl
{
	template <typename _Type>
	concept drawable = std::derived_from<_Type, Drawable>;

	template <typename _Func>
	concept draws = std::invocable<_Func, const Drawable&>;

	template <typename _Func>
	concept modifies = std::invocable<_Func, Drawable&>;
}

export
namespace sdl
{
	class Frame
	{
		Window*   window   = nullptr;
		Renderer* renderer = nullptr;

	public:
		const Point size = {};

	public:
		Frame(Window* _window, Renderer* _renderer, Point _size) :
			window(_window), renderer(_renderer), size(_size) {}

		Frame(const Frame&) = default;

	private:
		template <impl::modifies _Func, impl::drawable... _Drawable>
		void apply(_Func _func, _Drawable&... _objects) const
		{
			(void)std::initializer_list<int>{(_func(_objects), 0)...};
		}

		template <impl::draws _Func, impl::drawable... _Drawable>
		void apply(_Func _func, const _Drawable&... _objects) const
		{
			(void)std::initializer_list<int>{(_func(_objects), 0)...};
		}

	public:
		auto scaling() const -> FPoint
		{
			Point current;
			SDL_GetWindowSize(window, &current.x, &current.y);
			return {
				.x = (float)current.x / (float)size.x,
				.y = (float)current.y / (float)size.y
			};
		}

		template <impl::drawable... _Drawable>
		void scale(_Drawable&... _objects) const
		{
			apply([this](Drawable& _obj){ _obj.scaling = scaling(); }, _objects...);
		}
		template <impl::drawable... _Drawable>
		void update(_Drawable&... _objects) const
		{
			apply([this](Drawable& _obj){ _obj.update(); }, _objects...);
		}
		template <impl::drawable... _Drawable>
		void draw(const _Drawable&... _objects) const
		{
			apply([this](const Drawable& _obj){ _obj.draw(renderer); }, _objects...);
		}

		void present() const
		{
			SDL_RenderPresent(renderer);
		}

		void clear(Color _color) const
		{
			auto [r, g, b, a] = _color;
			if (int error = SDL_SetRenderDrawColor(renderer, r, g, b, a))
				throw ErrorSDL(error);
			SDL_RenderClear(renderer);
		}
		void clear() const
		{
			if (int error = SDL_RenderClear(renderer))
				throw ErrorSDL(error);
		}
	};
} // namespace sdl