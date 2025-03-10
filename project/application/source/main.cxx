
#include <SDL2/SDL_main.h>

import components.sdl;
import core;

namespace app
{
	constexpr struct {
		int w = 504;
		int h = 875;
	} window;
}

int main(int _argc, char** _argv)
{
	auto window_data = sdl::WindowData{
		.title = "Match Three",
		.rect  = {
			.x = sdl::win::pos::centered::mask,
			.y = sdl::win::pos::centered::mask,
			.w = app::window.w,
			.h = app::window.h,
		},
		.flags = {
			.sdl      = sdl::init::VIDEO,
			.img      = img::init::PNG,
			.window   = sdl::win::init::RESIZABLE,
			.renderer = sdl::renderer::ACCELERATED,
		},
	};

	auto textures = sdl::type::textures(core::env::textures::all);

	sdl::context(window_data, textures, core::loop);

	return 0;
}