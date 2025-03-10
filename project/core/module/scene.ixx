module;

export module core:scene;

import components.sdl;

import :elements;

export
namespace core
{
	struct Scene
	{
		Background background;
		Interface  interface;
		Mascots    mascots;
		Grid       grid;

		void init(sdl::TexturePool _pool)
		{
			background.init(_pool);
			interface.init(_pool);
			mascots.init(_pool);
			grid.init(_pool);
		}
		void layout(sdl::Frame _frame)
		{
			background.size = !_frame.size;
			interface.size  = !_frame.size;
			mascots.size    = !_frame.size;
			grid.size      = !_frame.size;
		}
	};
}