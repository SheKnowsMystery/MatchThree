module;

#include <SDL2/SDL_error.h>
#include <SDL2/SDL_image.h>

#include <stdexcept>
#include <string>
#include <string_view>

export module components.sdl:exceptions;

export
namespace sdl
{
	class ErrorBase : public std::runtime_error
	{
	public:
		using Base = std::runtime_error;

		const int code = 0;

		ErrorBase(std::string_view _message, int _code = 0) :
			Base(std::string(_message)), code(_code) {}
	};

	class ErrorSDL : public ErrorBase
	{
	public:
		using Base = ErrorBase;
		ErrorSDL(int _code = 0) : Base(SDL_GetError(), _code) {}
	};

	class ErrorIMG : public ErrorBase
	{
	public:
		using Base = ErrorBase;
		ErrorIMG(int _code = 0) : Base(IMG_GetError(), _code) {}
	};
} // namespace sdl::exc