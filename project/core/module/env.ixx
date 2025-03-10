module;

#include <type_traits>
#include <initializer_list>

export module core:env;

import utility;
import components.sdl;

export
namespace core::env
{
	enum class ElemID
	{
		avocado, strawberry, lemon,
		size,
		none = size,
	};

	using ElemID_t = std::underlying_type_t<ElemID>;

	enum class Direction
	{
		up, left, down, right,
	};

	enum class Orientation
	{
		vertical, horizontal,
	};
} // namespace core::env

export
namespace core::env
{
	auto operator!(Direction _direction) -> Direction
	{
		switch (_direction)
		{
			using enum env::Direction;
		case up:
			return down;
		case left:
			return right;
		case down:
			return up;
		case right:
			return left;
		default:
			// TODO: make assert or throw an exception
			return {};
		}
	}

	auto operator!(Orientation _orientation) -> Orientation
	{
		switch (_orientation)
		{
			using enum env::Orientation;
		case vertical:
			return horizontal;
		case horizontal:
			return vertical;
		default:
			// TODO: make assert or throw an exception
			return {};
		}
	}
} // namespace core::env

export
namespace core::env::elems
{
	constexpr auto list = util::variadic::fill<env::ElemID, (env::ElemID_t)(env::ElemID::size)>::array([](std::size_t _id) { return (env::ElemID)(_id); });
}

export
namespace core::env::textures
{
	constexpr auto background = "fon";

	namespace icons
	{
		constexpr auto key      = "key";
		constexpr auto settings = "settings";
		constexpr auto level    = "level";
	} // namespace icons

	constexpr auto status_bar   = "statuds";
	constexpr auto progress_bar = "uroven";

	constexpr auto mascot1 = "mascot01";
	constexpr auto mascot2 = "mascot2";

	namespace elems::light
	{
		constexpr auto avocado    = "avokado";
		constexpr auto strawberry = "klubnika-svet";
		constexpr auto lemon      = "limon";
		constexpr auto all = {
			avocado,
			strawberry,
			lemon,
		};
	} // namespace elems::light

	namespace elems::dark
	{
		constexpr auto avocado    = "avokado-tmn";
		constexpr auto strawberry = "klubnika-tenm";
		constexpr auto lemon      = "limon";
		constexpr auto all = {
			avocado,
			strawberry,
			lemon,
		};
	} // namespace elems::dark

	namespace get
	{
		auto elem(ElemID _id, bool _dark = false) -> const char*
		{
			switch (_id)
			{
			case ElemID::avocado:
				return _dark ? elems::dark::avocado : elems::light::avocado;
			case ElemID::strawberry:
				return _dark ? elems::dark::strawberry : elems::light::strawberry;
			case ElemID::lemon:
				return _dark ? elems::dark::lemon : elems::light::lemon;
			default:
				// TODO: make assert or throw an exception
				return nullptr;
			}
		}
	}

	inline constexpr auto all = {
		background,

		icons::key,
		icons::level,
		icons::settings,

		status_bar,
		progress_bar,

		mascot1,
		mascot2,

		elems::light::avocado,
		elems::light::strawberry,
		elems::light::lemon,

		elems::dark::avocado,
		elems::dark::strawberry,
		elems::dark::lemon,
	};
} // namespace core::env::textures

export
namespace core::env::size
{
	constexpr sdl::FPoint background = {.x = 308, .y = 460};

	namespace icons
	{
		constexpr sdl::FPoint key      = {.x = 54,  .y = 54};
		constexpr sdl::FPoint level    = {.x = 141, .y = 50};
		constexpr sdl::FPoint settings = {.x = 51,  .y = 51};
	} // namespace icons

	constexpr sdl::FPoint status_bar   = {.x = 202, .y = 26};
	constexpr sdl::FPoint progress_bar = {.x = 168, .y = 39};

	constexpr sdl::FPoint mascot1 = {.x = 79,  .y = 136};
	constexpr sdl::FPoint mascot2 = {.x = 101, .y = 130};

	namespace elems::light
	{
		constexpr sdl::FPoint avocado    = {.x = 89, .y = 88};
		constexpr sdl::FPoint strawberry = {.x = 89, .y = 88};
		constexpr sdl::FPoint lemon      = {.x = 88, .y = 88};
	} // namespace elems::light

	namespace elems::dark
	{
		constexpr sdl::FPoint avocado    = {.x = 74, .y = 73};
		constexpr sdl::FPoint strawberry = {.x = 88, .y = 88};
		constexpr sdl::FPoint lemon      = {.x = 88, .y = 88};
	} // namespace elems::dark

	namespace get
	{
		auto elem(ElemID _id, bool _dark = false) -> sdl::FPoint
		{
			switch (_id)
			{
			case ElemID::avocado:
				return _dark ? elems::dark::avocado : elems::light::avocado;
			case ElemID::strawberry:
				return _dark ? elems::dark::strawberry : elems::light::strawberry;
			case ElemID::lemon:
				return _dark ? elems::dark::lemon : elems::light::lemon;
			default:
				// TODO: make assert or throw an exception
				return {};
			}
		}

	} // namespace get

	auto adjust(sdl::FPoint _raw, sdl::FPoint _size, float _scale) -> sdl::FPoint
	{
		return {
			.x = _size.x * _scale,
			.y = _raw.y * _size.x * _scale / _raw.x,
		};
	}

	namespace matrix
	{
		constexpr std::size_t columns = 4;
		constexpr std::size_t rows    = 5;
	} // namespace matrix
} // namespace core::env::size