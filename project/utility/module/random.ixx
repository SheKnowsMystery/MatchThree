module;

#include <random>

export module utility:random;

namespace util::impl
{
	auto& random_engine()
	{
		static auto device = std::random_device();
		static auto engine = std::default_random_engine(device());
		return engine;
	}
}

export
namespace util
{
	template <typename _Type>
	auto random(std::uniform_int_distribution<_Type>& _distribution) -> _Type
	{
		return _distribution(impl::random_engine());
	}
}