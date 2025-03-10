module;

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

export module components.sdl:drawable;

import :bindings;
import :utility;
import :exceptions;
import :pools;

export
namespace sdl
{
	struct MyRect
	{
		FPoint size     = {};
		FPoint position = {};
		FPoint scaling  = {.x = 1.F, .y = 1.F};

		MyRect() = default;
		MyRect(const MyRect&) = default;
		MyRect(MyRect&&) = default;

		auto operator=(const MyRect&) -> MyRect& = default;
		auto operator=(MyRect&&)      -> MyRect& = default;

		operator FRect() const;
		operator Rect() const;

		bool contains(FPoint _point) const;
		bool contains(Point _point) const;

		void draw(Renderer* _renderer, Color _color = env::white) const;
		void fill(Renderer* _renderer, Color _color = env::white) const;
	};

	MyRect::operator FRect() const
	{
		return {
			.x = position.x * scaling.x,
			.y = position.y * scaling.y,
			.w = size.x * scaling.x,
			.h = size.y * scaling.y,
		};
	}

	MyRect::operator Rect() const
	{
		return !(FRect)*this;
	}

	bool MyRect::contains(sdl::FPoint _point) const
	{
		sdl::FRect rect = *this;
		return SDL_PointInFRect(&_point, &rect);
	}

	bool MyRect::contains(sdl::Point _point) const
	{
		sdl::FPoint point = {.x = (float)_point.x, .y = (float)_point.y};
		sdl::FRect  rect  = *this;
		return SDL_PointInFRect(&point, &rect);
	}

	void MyRect::draw(Renderer* _renderer, Color _color) const
	{
		auto [r, g, b, a] = _color;
		FRect rect = *this;
		SDL_SetRenderDrawColor(_renderer, r, g, b, a);
		SDL_RenderDrawRectF(_renderer, &rect);
	}

	void MyRect::fill(Renderer* _renderer, Color _color) const
	{
		auto [r, g, b, a] = _color;
		FRect rect = *this;
		SDL_SetRenderDrawColor(_renderer, r, g, b, a);
		SDL_RenderFillRectF(_renderer, &rect);
	}

	struct MyTexture
	{
		MyRect destination;

		Texture* ptr     = nullptr;
		Color    color   = {.r = 255, .g = 255, .b = 255, .a = 255};

		void draw(Renderer* _renderer) const;
		void draw(Renderer* _renderer, Rect _source) const;
	};

	void MyTexture::draw(Renderer* _renderer) const
	{
		auto [r, g, b, a]  = color;
		FRect dstrect = destination;
		if (int error = SDL_SetTextureColorMod(ptr, r, g, b))
			throw ErrorSDL(error);
		if (int error = SDL_RenderCopyF(_renderer, ptr, nullptr, &dstrect))
			throw ErrorSDL(error);
	}

	void MyTexture::draw(Renderer* _renderer, Rect _source) const
	{
		auto [r, g, b, a]  = color;
		FRect dstrect = destination;
		if (int error = SDL_SetTextureColorMod(ptr, r, g, b))
			throw ErrorSDL(error);
		if (int error = SDL_RenderCopyF(_renderer, ptr, &_source, &dstrect))
			throw ErrorSDL(error);
	}

	struct Drawable : virtual MyRect
	{
		virtual void init(TexturePool _pool)         = 0;
		virtual void draw(Renderer* _renderer) const = 0;
		virtual void update()                        = 0;
	};
} // namespace graphics