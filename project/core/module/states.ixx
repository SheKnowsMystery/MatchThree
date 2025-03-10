module;

export module core:states;

import components.sdl;

import :scene;

export
namespace core
{
	enum class StateID
	{
		fill, move, check, clear, wait, drag,
		last
	};

	struct State
	{
		struct Enter {
			Scene& scene;
		};

		struct Handle {
			Scene& scene;
			const sdl::Event& event;
		};

		struct Scale {
			Scene& scene;
			const sdl::Frame& frame;
		};

		struct Update {
			Scene& scene;
			const sdl::Frame& frame;
		};

		struct Draw {
			const Scene& scene;
			const sdl::Frame& frame;
		};

		struct Next {
			Scene& scene;
		};

		size_t updated = 0;
		const StateID id;
		State(StateID _id) : id(_id) {}

		virtual void scale(Scale _data)          = 0;
		virtual void handle(Handle _data)        = 0;
		virtual void update(Update _data)        = 0;
		virtual void draw(Draw _data)            = 0;
		virtual auto next(Next _data) -> StateID = 0;
	};
}

namespace core::impl
{
	struct Fill : State
	{
		Fill() : State(StateID::fill) {}

		void scale(Scale _data) override
		{
			auto [scene, frame] = _data;

			frame.scale(scene.background, scene.interface, scene.mascots, scene.grid);
		}
		void handle(Handle _data) override
		{
			auto [scene, event] = _data;
		}
		void update(Update _data) override
		{
			auto [scene, frame] = _data;

			scene.grid.fill();
			scene.grid.fall();

			frame.update(scene.background, scene.interface, scene.mascots, scene.grid);
		}
		void draw(Draw _data) override
		{
			auto [scene, frame] = _data;

			frame.draw(scene.background, scene.interface, scene.mascots, scene.grid);
		}
		auto next(Next _data) -> StateID override
		{
			auto [scene] = _data;

			return scene.grid.ismoving() ? StateID::move : StateID::check;
		}
	};

	struct Move : State
	{
		Move() : State(StateID::move) {}

		void scale(Scale _data) override
		{
			auto [scene, frame] = _data;

			frame.scale(scene.background, scene.interface, scene.mascots, scene.grid);
		}
		void handle(Handle _data) override
		{
			auto [scene, event] = _data;
		}
		void update(Update _data) override
		{
			auto [scene, frame] = _data;

			scene.grid.move();

			frame.update(scene.background, scene.interface, scene.mascots, scene.grid);
		}
		void draw(Draw _data) override
		{
			auto [scene, frame] = _data;

			frame.draw(scene.background, scene.interface, scene.mascots, scene.grid);
		}
		auto next(Next _data) -> StateID override
		{
			auto [scene] = _data;

			return scene.grid.ismoving() ? StateID::move : StateID::check;
		}
	};

	struct Check : State
	{
		Check() : State(StateID::check) {}

		void scale(Scale _data) override
		{
			auto [scene, frame] = _data;

			frame.scale(scene.background, scene.interface, scene.mascots, scene.grid);
		}
		void handle(Handle _data) override
		{
			auto [scene, event] = _data;
		}
		void update(Update _data) override
		{
			auto [scene, frame] = _data;

			scene.grid.unselect();

			if (!scene.grid.isempty())
				scene.grid.mark();

			frame.update(scene.background, scene.interface, scene.mascots, scene.grid);
		}
		void draw(Draw _data) override
		{
			auto [scene, frame] = _data;

			frame.draw(scene.background, scene.interface, scene.mascots, scene.grid);
		}
		auto next(Next _data) -> StateID override
		{
			auto [scene] = _data;

			if (scene.grid.isempty())
				return StateID::fill;
			if (scene.grid.ismatched())
				return StateID::clear;

			return StateID::wait;
		}
	};

	struct Clear : State
	{
		Clear() : State(StateID::clear) {}

		void scale(Scale _data) override
		{
			auto [scene, frame] = _data;

			frame.scale(scene.background, scene.interface, scene.mascots, scene.grid);
		}
		void handle(Handle _data) override
		{
			auto [scene, event] = _data;
		}
		void update(Update _data) override
		{
			auto [scene, frame] = _data;

			scene.grid.clear();

			frame.update(scene.background, scene.interface, scene.mascots, scene.grid);
		}
		void draw(Draw _data) override
		{
			auto [scene, frame] = _data;

			frame.draw(scene.background, scene.interface, scene.mascots, scene.grid);
		}
		auto next(Next _data) -> StateID override
		{
			auto [scene] = _data;

			// TODO: check if level is not fully cleared

			return StateID::fill;
		}
	};

	struct Wait : State
	{
		Wait() : State(StateID::wait) {}

		void scale(Scale _data) override
		{
			auto [scene, frame] = _data;

			frame.scale(scene.background, scene.interface, scene.mascots, scene.grid);
		}
		void handle(Handle _data) override
		{
			auto [scene, event] = _data;

			if (event.type == sdl::event::MOUSEBUTTONDOWN)
				if (event.button.button == 1)
					scene.grid.drag(sdl::motion(event));
		}
		void update(Update _data) override
		{
			auto [scene, frame] = _data;

			frame.update(scene.background, scene.interface, scene.mascots, scene.grid);
		}
		void draw(Draw _data) override
		{
			auto [scene, frame] = _data;

			frame.draw(scene.background, scene.interface, scene.mascots, scene.grid);
		}
		auto next(Next _data) -> StateID override
		{
			auto [scene] = _data;

			return scene.grid.isdragged() ? StateID::drag : StateID::wait;
		}
	};

	struct Drag : State
	{
		Drag() : State(StateID::drag) {}

		void scale(Scale _data) override
		{
			auto [scene, frame] = _data;

			frame.scale(scene.background, scene.interface, scene.mascots, scene.grid);
		}
		void handle(Handle _data) override
		{
			auto [scene, event] = _data;

			if (event.type == sdl::event::MOUSEBUTTONUP)
				if (event.button.button == 1)
					scene.grid.follow(sdl::motion(event));
		}
		void update(Update _data) override
		{
			auto [scene, frame] = _data;

			frame.update(scene.background, scene.interface, scene.mascots, scene.grid);
		}
		void draw(Draw _data) override
		{
			auto [scene, frame] = _data;

			frame.draw(scene.background, scene.interface, scene.mascots, scene.grid);
		}
		auto next(Next _data) -> StateID override
		{
			auto [scene] = _data;

			return scene.grid.isdragged() ? StateID::drag : StateID::move;
		}
	};
}

namespace core::impl::get
{
	auto state(StateID _id) -> State*
	{
		static auto fill  = Fill();
		static auto move  = Move();
		static auto check = Check();
		static auto clear = Clear();
		static auto wait  = Wait();
		static auto drag  = Drag();

		switch (_id)
		{
		case StateID::fill:
			return &fill;
		case StateID::move:
			return &move;
		case StateID::check:
			return &check;
		case StateID::clear:
			return &clear;
		case StateID::wait:
			return &wait;
		case StateID::drag:
			return &drag;
		default:
			return nullptr;
		}
	}
}

export
namespace core
{
	class Automat
	{
		StateID id = StateID::fill;

	public:
		Automat()
		{
			(void)impl::get::state(StateID::last);
		}
		auto current() const -> State*
		{
			return impl::get::state(id);
		}
		void next(StateID _id)
		{
			id = _id;
		}
	};
}