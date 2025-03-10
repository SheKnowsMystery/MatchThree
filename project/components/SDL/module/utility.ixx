module;

#include <SDL_events.h>

export module components.sdl:utility;

import utility;

import :bindings;

export
namespace sdl
{
	auto operator-(FPoint _point) -> FPoint
	{
		return {.x = -_point.x, .y = -_point.y};
	}

	template <util::concepts::arithmetic _Type>
	auto operator+(FPoint _point) -> FPoint
	{
		return {.x = +_point.x, .y = +_point.y};
	}

	template <util::concepts::arithmetic _Type>
	auto operator+(FPoint _point, _Type _val) -> FPoint
	{
		return {.x = _point.x + (float)_val, .y = _point.y + (float)_val};
	}
	template <util::concepts::arithmetic _Type>
	auto operator+=(FPoint& _point, _Type _val) -> FPoint&
	{
		_point.x += (float)_val;
		_point.y += (float)_val;
		return _point;
	}

	template <util::concepts::arithmetic _Type>
	auto operator-(FPoint _point, _Type _val) -> FPoint
	{
		return {.x = _point.x - (float)_val, .y = _point.y - (float)_val};
	}
	template <util::concepts::arithmetic _Type>
	auto operator-=(FPoint& _point, _Type _val) -> FPoint&
	{
		_point.x -= (float)_val;
		_point.y -= (float)_val;
		return _point;
	}

	template <util::concepts::arithmetic _Type>
	auto operator*(FPoint _point, _Type _val) -> FPoint
	{
		return {.x = _point.x * (float)_val, .y = _point.y * (float)_val};
	}
	template <util::concepts::arithmetic _Type>
	auto operator*=(FPoint& _point, _Type _val) -> FPoint&
	{
		_point.x *= (float)_val;
		_point.y *= (float)_val;
		return _point;
	}

	template <util::concepts::arithmetic _Type>
	auto operator/(FPoint _point, _Type _val) -> FPoint
	{
		return {.x = _point.x / (float)_val, .y = _point.y / (float)_val};
	}
	template <util::concepts::arithmetic _Type>
	auto operator/=(FPoint& _point, _Type _val) -> FPoint&
	{
		_point.x /= (float)_val;
		_point.y /= (float)_val;
		return _point;
	}

	auto operator+(FPoint _p1, FPoint _p2) -> FPoint
	{
		return {.x = _p1.x + _p2.x, .y = _p1.y + _p2.y};
	}
	auto operator+=(FPoint& _p1, FPoint _p2) -> FPoint
	{
		_p1.x += _p2.x;
		_p1.y += _p2.y;
		return _p1;
	}

	auto operator-(FPoint _p1, FPoint _p2) -> FPoint
	{
		return {.x = _p1.x - _p2.x, .y = _p1.y - _p2.y};
	}
	auto operator-=(FPoint& _p1, FPoint _p2) -> FPoint
	{
		_p1.x -= _p2.x;
		_p1.y -= _p2.y;
		return _p1;
	}

	auto operator*(FPoint _p1, FPoint _p2) -> FPoint
	{
		return {.x = _p1.x * _p2.x, .y = _p1.y * _p2.y};
	}
	auto operator*=(FPoint& _p1, FPoint _p2) -> FPoint
	{
		_p1.x *= _p2.x;
		_p1.y *= _p2.y;
		return _p1;
	}

	auto operator/(FPoint _p1, FPoint _p2) -> FPoint
	{
		return {.x = _p1.x / _p2.x, .y = _p1.y / _p2.y};
	}
	auto operator/=(FPoint& _p1, FPoint _p2) -> FPoint
	{
		_p1.x /= _p2.x;
		_p1.y /= _p2.y;
		return _p1;
	}

	auto operator!(FPoint _point) -> Point
	{
		return { .x = (int)_point.x, .y = (int)_point.y };
	}
	auto operator!(Point _point) -> FPoint
	{
		return {.x = (float)_point.x, .y = (float)_point.y};
	}

	auto operator!(FRect _point) -> Rect
	{
		return {
			.x = (int)_point.x,
			.y = (int)_point.y,
			.w = (int)_point.w,
			.h = (int)_point.h,
		};
	}

	auto operator!(Rect _point) -> FRect
	{
		return {
			.x = (float)_point.x,
			.y = (float)_point.y,
			.w = (float)_point.w,
			.h = (float)_point.h,
		};
	}
} // namespace sdl

export
{
	using sdl::operator+;
	using sdl::operator+=;
	using sdl::operator-;
	using sdl::operator-=;
	using sdl::operator*;
	using sdl::operator*=;
	using sdl::operator/;
	using sdl::operator/=;
	using sdl::operator!;
}

export
namespace sdl
{
	bool poll(Event& _event)
	{
		return SDL_PollEvent(&_event);
	}

	auto motion(const Event& _event) -> FPoint
	{
		return {.x = (float)_event.motion.x, .y = (float)_event.motion.y};
	}
} // namespace sdl