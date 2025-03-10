module;

#include <cstddef>
#include <concepts>
#include <type_traits>

export
module utility:concepts;

import :traits;

export
namespace util::concepts
{
	template <std::size_t _size, typename... _Args>
	concept argc = _size == sizeof...(_Args);

	template <typename _Type>
	concept lambda = requires { &_Type::operator(); };

	template <typename _Type>
	concept callable = traits::function<_Type>::callable;

	template <typename _Func, typename _ReturnType>
	concept returns = std::same_as<_ReturnType, typename traits::function<_Func>::return_type>;

	template <typename _Func>
	concept returns_bool = std::convertible_to<typename traits::function<_Func>::return_type, bool>;

	template <typename _Func, typename _ReturnType, typename... _Args>
	concept function = returns<_Func, _ReturnType> && std::invocable<_Func, _Args...>;

	template <typename _Type>
	concept arithmetic = std::is_arithmetic_v<_Type>;

	template <typename _Type>
	concept enumeration = std::is_enum<_Type>::value;
} // namespace util::concepts