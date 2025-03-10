module;

#include <cstddef>
#include <cmath>
#include <random>
#include <ranges>
#include <utility>
#include <vector>

export module core:elements;

import utility;
import components.sdl;

import :env;

export
namespace core
{
	class Background : public sdl::Drawable
	{
		sdl::MyTexture texture;

	public:
		void init(sdl::TexturePool _pool) override
		{
			texture.ptr = _pool[env::textures::background];
		}
		void draw(sdl::Renderer* _renderer) const override
		{
			sdl::MyRect source;

			source.position.x += 32;

			source.size = env::size::background;

			source.size.x -= (32 + 14);

			texture.draw(_renderer, source);
		}
		void update() override
		{
			texture.destination = *this;
		}
	};



	class Interface : public sdl::Drawable
	{
		sdl::MyTexture status_bar;
		sdl::MyTexture progress_bar;

		struct {
			sdl::MyTexture key;
			sdl::MyTexture level;
			sdl::MyTexture settings;
		} icons;

	public:
		void init(sdl::TexturePool _pool) override
		{
			status_bar.ptr   = _pool[env::textures::status_bar];
			progress_bar.ptr = _pool[env::textures::progress_bar];

			icons.key.ptr      = _pool[env::textures::icons::key];
			icons.level.ptr    = _pool[env::textures::icons::level];
			icons.settings.ptr = _pool[env::textures::icons::settings];
		}
		void draw(sdl::Renderer* _renderer) const override
		{
			status_bar.draw(_renderer);
			progress_bar.draw(_renderer);

			icons.key.draw(_renderer);
			icons.level.draw(_renderer);
			icons.settings.draw(_renderer);
		}

		void update() override
		{
			status_bar.destination   = *this;
			progress_bar.destination = *this;

			icons.key.destination      = *this;
			icons.level.destination    = *this;
			icons.settings.destination = *this;

			//------------------------>

			status_bar.destination.size   = env::size::adjust(env::size::status_bar, size, .9f);
			progress_bar.destination.size = env::size::adjust(env::size::progress_bar, size, .7f);

			icons.key.destination.size      = env::size::adjust(env::size::icons::key, size, .1f);
			icons.level.destination.size    = env::size::adjust(env::size::icons::level, size, .45f);
			icons.settings.destination.size = env::size::adjust(env::size::icons::settings, size, .1f);

			//------------------------>

			status_bar.destination.position.y   += size.y * .01f;
			progress_bar.destination.position.y += size.y * .08f;

			icons.level.destination.position.y += size.y * .20f;

			icons.settings.destination.position.y += size.y * .91f;
			icons.key.destination.position.y      += size.y * .91f;

			//------------------------>

			status_bar.destination.position.x   += size.x * (1 - .9f) / 2;
			progress_bar.destination.position.x += size.x * (1 - .7f) / 2;

			icons.level.destination.position.x += size.x * (1 - .45f) / 2;

			icons.settings.destination.position.x += size.x * .1f * .3f;
			icons.key.destination.position.x      += size.x * .1f * 8.5f;
		}
	};

	class Mascots : public sdl::Drawable
	{
		sdl::MyTexture mascot1;
		sdl::MyTexture mascot2;

	public:
		void init(sdl::TexturePool _pool) override
		{
			mascot1.ptr = _pool[env::textures::mascot1];
			mascot2.ptr = _pool[env::textures::mascot2];
		}
		void draw(sdl::Renderer* _renderer) const override
		{
			mascot1.draw(_renderer);
			mascot2.draw(_renderer);
		}

		void update() override
		{
			mascot1.destination = *this;
			mascot2.destination = *this;

			mascot1.destination.size = env::size::adjust(env::size::mascot1, size, .24f);
			mascot2.destination.size = env::size::adjust(env::size::mascot2, size, .34f);

			mascot1.destination.position.y += size.y * .16f;
			mascot2.destination.position.y += size.y * .17f;

			mascot1.destination.position.x += size.x * .13f;
			mascot2.destination.position.x += size.x * .67f;
		}
	};
} // namespace core

export
namespace core
{
	class Grid : public sdl::Drawable
	{
		static constexpr std::size_t capacity = env::size::matrix::rows * env::size::matrix::columns;

		static constexpr std::size_t minmatch = 3;

		static constexpr std::size_t segmentation = 0x10;

		struct Indices
		{
			std::size_t row    = env::size::matrix::rows;
			std::size_t column = env::size::matrix::columns;
		};

		union {
			struct {
				env::ElemID    id       = env::ElemID::none;
				sdl::MyTexture texture  = {};
			} matrix[env::size::matrix::rows][env::size::matrix::columns], flatten[capacity] = {};
		} elems;

		union {
			struct {
				env::Direction direction = env::Direction::down;
				std::size_t    distance  = 0;
				bool           reversed  = false;
				bool           moving    = false;
			} matrix[env::size::matrix::rows][env::size::matrix::columns], flatten[capacity] = {};
		} movement;

		union {
			struct {
				bool removal = false;
			} matrix[env::size::matrix::rows][env::size::matrix::columns], flatten[capacity] = {};
		} marked;

		struct {
			struct {
				std::size_t row    = env::size::matrix::rows;
				std::size_t column = env::size::matrix::columns;
			} elem;
			struct {
				sdl::FPoint position = {};
			} mouse;
		} dragged;

		sdl::Texture* pool[(env::ElemID_t)(env::ElemID::size)];

		static auto address(std::size_t _index) -> Indices
		{
			return {
				.row    = _index / env::size::matrix::columns,
				.column = _index % env::size::matrix::columns,
			};
		}

		static auto address(std::size_t _row, std::size_t _column, env::Direction _direction, std::size_t _depth = 1) -> Indices
		{
			Indices indices{
				.row    = _row,
				.column = _column,
			};

			switch (_direction)
			{
				using enum env::Direction;
			case up:
				indices.row    -= _depth;
				break;
			case left:
				indices.column -= _depth;
				break;
			case down:
				indices.row    += _depth;
				break;
			case right:
				indices.column += _depth;
				break;
			default:
				// TODO: make assert or throw an exception
				break;
			}

			return indices;
		}

		static bool isvalid(std::size_t _row, std::size_t _column, env::Direction _direction, std::size_t _depth = 1)
		{
			switch (_direction)
			{
				using enum env::Direction;
			case up:
				return (_row    >= _depth);
			case left:
				return (_column >= _depth);
			case down:
				return (_row    +  _depth) < env::size::matrix::rows;
			case right:
				return (_column +  _depth) < env::size::matrix::columns;
			default:
				// TODO: make assert or throw an exception
				return false;
			}
		}

		auto getid(std::size_t _row, std::size_t _column, env::Direction _direction, std::size_t _depth = 1) const -> env::ElemID
		{
			if (!isvalid(_row, _column, _direction, _depth))
				return env::ElemID::none;

			switch (_direction)
			{
				using enum env::Direction;
			case up:
				return elems.matrix[_row - _depth][_column].id;
			case left:
				return elems.matrix[_row][_column - _depth].id;
			case down:
				return elems.matrix[_row + _depth][_column].id;
			case right:
				return elems.matrix[_row][_column + _depth].id;
			default:
				// TODO: make assert or throw an exception
				return env::ElemID::none;
			}
		}

		auto commoncount(std::size_t _row, std::size_t _column, env::Direction _direction) const -> std::size_t
		{
			env::ElemID id = elems.matrix[_row][_column].id;

			std::size_t common = 0;

			for (std::size_t depth = 1; isvalid(_row, _column, _direction, depth); depth++)
				if (getid(_row, _column, _direction, depth) == id)
					common++;
				else
					break;

			return common;
		}

		bool checkline(std::size_t _row, std::size_t _column, env::Orientation _orientation) const
		{
			std::size_t common = 1;

			switch (_orientation)
			{
				using enum env::Orientation;
			case vertical:
				common += commoncount(_row, _column, env::Direction::up);
				common += commoncount(_row, _column, env::Direction::down);
				break;
			case horizontal:
				common += commoncount(_row, _column, env::Direction::left);
				common += commoncount(_row, _column, env::Direction::right);
				break;
			default:
				// TODO: make assert or throw an exception
				break;
			}

			return common >= minmatch;
		}

		bool checkmatch(std::size_t _row, std::size_t _column)
		{
			return
				checkline(_row, _column, env::Orientation::vertical)
				||
				checkline(_row, _column, env::Orientation::horizontal);
		}

		void markcommon(std::size_t _row, std::size_t _column, env::Direction _direction)
		{
			env::ElemID id = elems.matrix[_row][_column].id;

			for (std::size_t depth = 1; isvalid(_row, _column, _direction, depth); depth++)
				if (getid(_row, _column, _direction, depth) == id)
				{
					auto neighbor = address(_row, _column, _direction, depth);
					marked.matrix[neighbor.row][neighbor.column].removal = true;
				}
				else
					break;
		}

		void markline(std::size_t _row, std::size_t _column, env::Orientation _orientation)
		{
			switch (_orientation)
			{
				using enum env::Orientation;
			case vertical:
				markcommon(_row, _column, env::Direction::up);
				markcommon(_row, _column, env::Direction::down);
				break;
			case horizontal:
				markcommon(_row, _column, env::Direction::left);
				markcommon(_row, _column, env::Direction::right);
				break;
			default:
				// TODO: make assert or throw an exception
				break;
			}
		}

		void place(std::size_t _row, std::size_t _column)
		{
			auto filtered = env::elems::list | std::views::filter(
				[=, this](env::ElemID _id) -> bool
				{
					env::ElemID id = elems.matrix[_row][_column].id;
					elems.matrix[_row][_column].id = _id;
					bool match = checkmatch(_row, _column);
					elems.matrix[_row][_column].id = id;
					return !match;
				}
			);

			auto available = std::vector<env::ElemID>(filtered.begin(), filtered.end());

			auto distribution = std::uniform_int_distribution<env::ElemID_t>(0, available.size() - 1);

			auto& [id, texture] = elems.matrix[_row][_column];

			id          = (env::ElemID)(available[util::random(distribution)]);
			texture.ptr = pool[(env::ElemID_t)(id)];
		}

		void move(std::size_t _row, std::size_t _column, env::Direction _direction, bool _revered = false)
		{
			auto& [direction, distance, reversed, moving] = movement.matrix[_row][_column];

			direction = _direction;
			distance  = _revered ? 0 : segmentation;
			reversed  = _revered;
			moving    = true;
		}

		void swap(std::size_t _row, std::size_t _column, env::Direction _direction, std::size_t _depth = 1)
		{
			auto neighbor = address(_row, _column, _direction, _depth);
			std::swap(elems.matrix[_row][_column], elems.matrix[neighbor.row][neighbor.column]);
		}

	public:
		void init(sdl::TexturePool _pool) override
		{
			for (env::ElemID_t id = 0; id < (env::ElemID_t)(env::ElemID::size); id++)
				pool[id] = _pool[env::textures::get::elem((env::ElemID)(id))];
		}
		void draw(sdl::Renderer* _renderer) const override
		{
			for (const auto& [id, texture] : elems.flatten)
				if (id != env::ElemID::none)
					texture.draw(_renderer);
		}
		void update() override
		{
			const struct {
				sdl::FPoint global;
				sdl::FPoint single;
			} offset = {
				.global = {
					.x = size.x * .13f,
					.y = size.y * .325f,
				},
				.single = {
					.x = size.x * .013f,
					.y = size.y * .013f,
				}
			};

			for (std::size_t row = 0; row < env::size::matrix::rows; row++)
				for (std::size_t column = 0; column < env::size::matrix::columns; column++)
				{
					auto& [id, texture] = elems.matrix[row][column];

					auto& destination = texture.destination;

					destination = *this;

					destination.size = env::size::adjust(env::size::get::elem(id), size, .18f);

					destination.position += offset.global;

					sdl::FPoint gridpos = {
						.x = (float)column,
						.y = (float)row
					};

					sdl::FPoint interval = destination.size + offset.single;

					destination.position += interval * gridpos;

					auto& [direction, distance, reversed, moving] = movement.matrix[row][column];

					sdl::FPoint realdistance = interval / segmentation * distance;

					if (reversed)
						realdistance = -realdistance;

					switch (direction)
					{
						using enum env::Direction;
					case up:
						destination.position.y += realdistance.y;
						break;
					case left:
						destination.position.x += realdistance.x;
						break;
					case down:
						destination.position.y -= realdistance.y;
						break;
					case right:
						destination.position.x -= realdistance.x;
						break;
					default:
						// TODO: make assert or throw an exception
						break;
					}
				}
		}

		void fill()
		{
			constexpr std::size_t row = 0;
			for (std::size_t column = 0; column < env::size::matrix::columns; column++)
				if (elems.matrix[row][column].id == env::ElemID::none)
					place(row, column);
		}

		void fall()
		{
			constexpr std::size_t endrow = env::size::matrix::rows - 1;

			for (std::size_t row = 0; row < env::size::matrix::rows; row++)
				for (std::size_t column = 0; column < env::size::matrix::columns; column++)
				{
					auto& [id, texture] = elems.matrix[endrow - row][column];

					if (id != env::ElemID::none)
						continue;

					if (std::size_t uprow = row + 1; row < endrow)
						if (elems.matrix[endrow - uprow][column].id != env::ElemID::none)
							std::swap(elems.matrix[endrow - uprow][column], elems.matrix[endrow - row][column]);

					move(endrow - row, column, env::Direction::down);
				}
		}

		void move()
		{
			for (auto& [direction, distance, reversed, moving] : movement.flatten)
				if (moving)
				{
					if (reversed)
					{
						distance++;

						if (distance == segmentation)
						{
							direction = !direction;
							reversed  = false;
						}
					}
					else
					{
						distance--;

						if (distance == 0)
							moving = false;
					}
				}
		}

		void mark()
		{
			for (std::size_t row = 0; row < env::size::matrix::rows; row++)
				for (std::size_t column = 0; column < env::size::matrix::columns; column++)
					if (checkmatch(row, column))
					{
						if (checkline(row, column, env::Orientation::vertical))
							markline(row, column, env::Orientation::vertical);
						if (checkline(row, column, env::Orientation::horizontal))
							markline(row, column, env::Orientation::horizontal);
						marked.matrix[row][column].removal = true;
					}
		}

		void unselect()
		{
			for (auto& elem : elems.flatten)
				elem.texture.color = sdl::env::white;
		}

		void clear()
		{
			for (std::size_t row = 0; row < env::size::matrix::rows; row++)
				for (std::size_t column = 0; column < env::size::matrix::columns; column++)
					if (marked.matrix[row][column].removal)
					{
						marked.matrix[row][column].removal = false;
						elems.matrix[row][column].id       = env::ElemID::none;
					}
		}

		void drag(sdl::FPoint _mouse)
		{
			for (std::size_t row = 0; row < env::size::matrix::rows; row++)
				for (std::size_t column = 0; column < env::size::matrix::columns; column++)
				{
					auto& [id, texture] = elems.matrix[row][column];

					if (texture.destination.contains(_mouse))
					{
						dragged.elem.row    = row;
						dragged.elem.column = column;

						dragged.mouse.position = _mouse;

						texture.color = sdl::env::gray;
					}
				}
		}

		void follow(sdl::FPoint _mouse)
		{
			auto offset = _mouse - dragged.mouse.position;

			env::Orientation orientation = std::abs(offset.x) > std::abs(offset.y) ? env::Orientation::horizontal : env::Orientation::vertical;

			env::Direction direction;

			switch (orientation)
			{
			case env::Orientation::vertical:
				direction = offset.y > 0 ? env::Direction::down : env::Direction::up;
				break;
			case env::Orientation::horizontal:
				direction = offset.x > 0 ? env::Direction::right : env::Direction::left;
				break;
			}

			if (isvalid(dragged.elem.row, dragged.elem.column, direction))
			{
				auto neighbor = address(dragged.elem.row, dragged.elem.column, direction);

				elems.matrix[neighbor.row][neighbor.column].texture.color = sdl::env::gray;

				swap(dragged.elem.row, dragged.elem.column, direction);

				bool match =
					checkmatch(dragged.elem.row, dragged.elem.column)
					||
					checkmatch(neighbor.row, neighbor.column);

				if (match)
				{
					move(dragged.elem.row, dragged.elem.column, !direction);
					move(neighbor.row, neighbor.column, direction);
				}
				else
				{
					swap(dragged.elem.row, dragged.elem.column, direction);
					move(dragged.elem.row, dragged.elem.column, direction, true);
					move(neighbor.row, neighbor.column, !direction, true);
				}
			}

			dragged.elem.row    = env::size::matrix::rows;
			dragged.elem.column = env::size::matrix::columns;
		}

		bool ismoving() const
		{
			for (const auto& elem : movement.flatten)
				if (elem.moving)
					return true;
			return false;
		}

		bool ismatched() const
		{
			for (const auto& elem : marked.flatten)
				if (elem.removal)
					return true;
			return false;
		}

		bool isempty() const
		{
			for (const auto& elem : elems.flatten)
				if (elem.id == env::ElemID::none)
					return true;
			return false;
		}

		bool isdragged() const
		{
			return dragged.elem.row != env::size::matrix::rows && dragged.elem.column != env::size::matrix::columns;
		}

	};
} // namespace core