module;

export module core:loop;

import utility;
import components.sdl;

import :states;
import :scene;

export
namespace core
{
	void loop(sdl::Frame _frame, sdl::TexturePool _pool)
	{
		auto timer = util::timer<60>();

		sdl::Event event;

		Automat automat;
		Scene scene;

		scene.init(_pool);
		scene.layout(_frame);

		for (bool running = true; running;)
		{
			timer.start();
			while (sdl::poll(event))
				switch (event.type)
				{
				case sdl::event::QUIT:
					running = false;
					break;
				case sdl::event::WINDOWEVENT:
					if (event.window.event == sdl::win::event::RESIZED)
						automat.current()->scale(
							{
								.scene = scene,
								.frame = _frame,
							}
						);
					break;
				default:
					automat.current()->handle(
						{
							.scene = scene,
							.event = event,
						}
					);
					break;
				}

			automat.current()->update(
				{
					.scene = scene,
					.frame = _frame,
				}
			);

			_frame.clear(sdl::env::black);

			automat.current()->draw(
				{
					.scene = scene,
					.frame = _frame,
				}
			);

			_frame.present();

			automat.next(
				automat.current()->next(
					{
						.scene = scene,
					}
				)
			);

			timer.end();
			timer.wait();
		}
	}
}