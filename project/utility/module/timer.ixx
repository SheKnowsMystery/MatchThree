module;

#include <chrono>
#include <thread>

export module utility:timer;

export
namespace util
{
	template <std::size_t _fps>
	class timer
	{
		using point_t = std::chrono::time_point<std::chrono::high_resolution_clock>;
		using unit_t  = std::chrono::nanoseconds;

		const size_t fps = _fps;
		const unit_t duration = std::chrono::duration_cast<unit_t>(std::chrono::seconds(1)) / _fps;

		struct {
			point_t start;
			point_t end;
			unit_t  delta;
		} time;
	public:
		void start()
		{
			time.start = std::chrono::high_resolution_clock::now();
		}
		void end()
		{
			time.end   = std::chrono::high_resolution_clock::now();
			time.delta = time.end - time.start;
		}
		void wait()
		{
			std::this_thread::sleep_for(
				std::max(
					duration - time.delta,
					std::chrono::nanoseconds::zero()
				)
			);
		}
	};
}