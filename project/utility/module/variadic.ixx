module;

#include <cstddef>
#include <utility>
#include <array>
#include <concepts>

export
module utility:variadic;

import :concepts;

export
namespace util::variadic
{
	template <typename _Type, std::size_t _size>
	struct fill
	{
	private:
		using index_sequence = std::make_index_sequence<_size>;

		template <typename>
		struct Array;

		template <std::size_t... _index>
		struct Array<std::index_sequence<_index...>>
		{
			static constexpr auto make() -> std::array<_Type, _size>
			{ return {_index...}; };

			template <typename _Arg>
				requires std::constructible_from<_Type, _Arg>
			static constexpr auto make(_Arg&& _arg) -> std::array<_Type, _size>
			{ return {(_index, std::forward<_Arg>(_arg))...}; };

			template <typename... _Args>
				requires std::constructible_from<_Type, _Args...>
			static constexpr auto make(_Args&&... _args) -> std::array<_Type, _size>
			{ return {(_index, _Type(std::forward<_Args>(_args)...))...}; };

			template <concepts::callable _Func>
				requires concepts::function<_Func, _Type>
			static constexpr auto make(_Func&& _func) -> std::array<_Type, _size>
			{ return {(_index, _func())...}; };

			template <concepts::callable _Func>
				requires concepts::function<_Func, _Type, size_t>
			static constexpr auto make(_Func&& _func) -> std::array<_Type, _size>
			{ return {_func(_index)...}; };

			template <concepts::callable _Func, typename... _Args>
				requires concepts::function<_Func, _Type, _Args...>
			static constexpr auto make(_Func _func, _Args&&... _args) -> std::array<_Type, _size>
			{ return {(_index, _func(std::forward<_Args>(_args)...))...}; };

			template <concepts::callable _Func, typename... _Args>
				requires concepts::function<_Func, _Type, _Args..., size_t>
			static constexpr auto make(_Func _func, _Args&&... _args) -> std::array<_Type, _size>
			{ return {_func(std::forward<_Args>(_args)..., _index)...}; };
		};

	public:
		static constexpr auto array() -> std::array<_Type, _size>
		{ return Array<index_sequence>::make(); }

		template <typename _Arg>
		static constexpr auto array(_Arg&& _arg) -> std::array<_Type, _size>
		{ return Array<index_sequence>::make(std::forward<_Arg>(_arg)); }

		template <typename... _Args>
		static constexpr auto array(_Args&&... _args) -> std::array<_Type, _size>
		{ return Array<index_sequence>::make(std::forward<_Args>(_args)...); }

		template <concepts::callable _Func, typename... _Args>
		static constexpr auto array(_Func _func, _Args&&... _args) -> std::array<_Type, _size>
		{ return Array<index_sequence>::make(_func, std::forward<_Args>(_args)...); }
	};
} // namespace util::variadic

export
namespace util::variadic
{
	template <std::size_t _size>
	struct calculate
	{
	private:
		template <typename>
		struct Pow;

		template <std::size_t... _index>
		struct Pow<std::index_sequence<_index...>>
		{
			template <std::integral _Type>
			static constexpr auto make(_Type _value) -> _Type
			{
				_Type result = 1;
				if constexpr (sizeof...(_index) > 0)
					return result *= ((_index, _value) * ...);
				else
					return result;
			}
		};

	public:
		template <std::integral _Type>
		static constexpr auto pow(_Type _value) -> _Type
		{ return Pow<std::make_index_sequence<_size>>::make(_value); }
	};
} // namespace util::variadic