module;

#include <type_traits>
#include <tuple>

export
module utility:traits;

export
namespace util::traits
{
	// primary template handles types that have no operator() defined
	template <typename, typename = void>
	struct lambda
	{ using type = void; };

	// specialization recognizes types that do have a operator() defined
	template <typename _Type>
	struct lambda<_Type, std::void_t<decltype(&_Type::operator())>>
	{ using type = decltype(&_Type::operator()); };
} // namespace util::traits

export
namespace util::traits
{
	template <typename>
	struct function;

	// primary template handles types that are not function-like
	template <>
	struct function<void>
	{ static constexpr bool callable = false; };

	// specialization recognizes types that are function-like
	template <typename _ReturnType, typename... _Args>
	struct function<_ReturnType(_Args...)>
	{
		static constexpr bool callable = true;

		using return_type = _ReturnType;

		using arguments = std::tuple<_Args...>;

		template <std::size_t i>
		using argument_type = typename std::tuple_element<i, arguments>::type;

		static constexpr size_t arity = std::tuple_size<arguments>::value;
	};

	// Generic type
	template <typename _Type>
	struct function : function<typename lambda<_Type>::type> {};

	// Function pointer
	template <typename _ReturnType, typename... _Args>
	struct function<_ReturnType (*)(_Args...)> : function<_ReturnType(_Args...)> {};

	// L-Value
	// -------

	// Lambda or member function []
	template <typename _CallableType, typename _ReturnType, typename... _Args>
	struct function<_ReturnType (_CallableType::*)(_Args...)> : function<_ReturnType(_Args...)> {};

	// Lambda or member function [const]
	template <typename _CallableType, typename _ReturnType, typename... _Args>
	struct function<_ReturnType (_CallableType::*)(_Args...) const> : function<_ReturnType(_Args...)> {};

	// Lambda or member function [volatile]
	template <typename _CallableType, typename _ReturnType, typename... _Args>
	struct function<_ReturnType (_CallableType::*)(_Args...) volatile> : function<_ReturnType(_Args...)> {};

	// Lambda or member function [const volatile]
	template <typename _CallableType, typename _ReturnType, typename... _Args>
	struct function<_ReturnType (_CallableType::*)(_Args...) const volatile> : function<_ReturnType(_Args...)> {};

	// L-Value-Ref
	// -------

	// Lambda or member function [&]
	template <typename _CallableType, typename _ReturnType, typename... _Args>
	struct function<_ReturnType (_CallableType::*)(_Args...) &> : function<_ReturnType(_Args...)> {};

	// Lambda or member function [const &&]
	template <typename _CallableType, typename _ReturnType, typename... _Args>
	struct function<_ReturnType (_CallableType::*)(_Args...) const &> : function<_ReturnType(_Args...)> {};

	// Lambda or member function [volatile &&]
	template <typename _CallableType, typename _ReturnType, typename... _Args>
	struct function<_ReturnType (_CallableType::*)(_Args...) volatile &> : function<_ReturnType(_Args...)> {};

	// Lambda or member function [const volatile &&]
	template <typename _CallableType, typename _ReturnType, typename... _Args>
	struct function<_ReturnType (_CallableType::*)(_Args...) const volatile &> : function<_ReturnType(_Args...)> {};

	// R-Value-Ref
	// -------

	// Lambda or member function [&&]
	template <typename _CallableType, typename _ReturnType, typename... _Args>
	struct function<_ReturnType (_CallableType::*)(_Args...) &&> : function<_ReturnType(_Args...)> {};

	// Lambda or member function [const &&]
	template <typename _CallableType, typename _ReturnType, typename... _Args>
	struct function<_ReturnType (_CallableType::*)(_Args...) const &&> : function<_ReturnType(_Args...)> {};

	// Lambda or member function [volatile &&]
	template <typename _CallableType, typename _ReturnType, typename... _Args>
	struct function<_ReturnType (_CallableType::*)(_Args...) volatile &&> : function<_ReturnType(_Args...)> {};

	// Lambda or member function [const volatile &&]
	template <typename _CallableType, typename _ReturnType, typename... _Args>
	struct function<_ReturnType (_CallableType::*)(_Args...) const volatile &&> : function<_ReturnType(_Args...)> {};

	// ========
	// Noexcept
	// ========

	// Function pointer
	template <typename _ReturnType, typename... _Args>
	struct function<_ReturnType (*)(_Args...) noexcept> : function<_ReturnType(_Args...)> {};

	// L-Value
	// -------

	// Lambda or member function [noexcept]
	template <typename _CallableType, typename _ReturnType, typename... _Args>
	struct function<_ReturnType (_CallableType::*)(_Args...) noexcept> : function<_ReturnType(_Args...)> {};

	// Lambda or member function [const noexcept]
	template <typename _CallableType, typename _ReturnType, typename... _Args>
	struct function<_ReturnType (_CallableType::*)(_Args...) const noexcept> : function<_ReturnType(_Args...)> {};

	// Lambda or member function [volatile noexcept]
	template <typename _CallableType, typename _ReturnType, typename... _Args>
	struct function<_ReturnType (_CallableType::*)(_Args...) volatile noexcept> : function<_ReturnType(_Args...)> {};

	// Lambda or member function [const volatile noexcept]
	template <typename _CallableType, typename _ReturnType, typename... _Args>
	struct function<_ReturnType (_CallableType::*)(_Args...) const volatile noexcept> : function<_ReturnType(_Args...)> {};

	// L-Value-Ref
	// -------

	// Lambda or member function [& noexcept]
	template <typename _CallableType, typename _ReturnType, typename... _Args>
	struct function<_ReturnType (_CallableType::*)(_Args...) & noexcept> : function<_ReturnType(_Args...)> {};

	// Lambda or member function [const && noexcept]
	template <typename _CallableType, typename _ReturnType, typename... _Args>
	struct function<_ReturnType (_CallableType::*)(_Args...) const & noexcept> : function<_ReturnType(_Args...)> {};

	// Lambda or member function [volatile && noexcept]
	template <typename _CallableType, typename _ReturnType, typename... _Args>
	struct function<_ReturnType (_CallableType::*)(_Args...) volatile & noexcept> : function<_ReturnType(_Args...)> {};

	// Lambda or member function [const volatile && noexcept]
	template <typename _CallableType, typename _ReturnType, typename... _Args>
	struct function<_ReturnType (_CallableType::*)(_Args...) const volatile & noexcept> : function<_ReturnType(_Args...)> {};

	// R-Value-Ref
	// -------

	// Lambda or member function [&& noexcept]
	template <typename _CallableType, typename _ReturnType, typename... _Args>
	struct function<_ReturnType (_CallableType::*)(_Args...) && noexcept> : function<_ReturnType(_Args...)> {};

	// Lambda or member function [const && noexcept]
	template <typename _CallableType, typename _ReturnType, typename... _Args>
	struct function<_ReturnType (_CallableType::*)(_Args...) const && noexcept> : function<_ReturnType(_Args...)> {};

	// Lambda or member function [volatile && noexcept]
	template <typename _CallableType, typename _ReturnType, typename... _Args>
	struct function<_ReturnType (_CallableType::*)(_Args...) volatile && noexcept> : function<_ReturnType(_Args...)> {};

	// Lambda or member function [const volatile && noexcept]
	template <typename _CallableType, typename _ReturnType, typename... _Args>
	struct function<_ReturnType (_CallableType::*)(_Args...) const volatile && noexcept> : function<_ReturnType(_Args...)> {};

} // namespace util::traits

export
namespace util::traits
{
	struct NonCopyable
	{
		NonCopyable()  = default;
		~NonCopyable() = default;
		NonCopyable(const NonCopyable&) = delete;
		NonCopyable(NonCopyable&&)      = default;
		NonCopyable& operator=(const NonCopyable&) = delete;
		NonCopyable& operator=(NonCopyable&&)      = default;
	};

	using no_copy = NonCopyable;

	struct NonMovable
	{
		NonMovable()  = default;
		~NonMovable() = default;
		NonMovable(const NonMovable&) = default;
		NonMovable(NonMovable&&)      = delete;
		NonMovable& operator=(const NonMovable&) = default;
		NonMovable& operator=(NonMovable&&)      = delete;
	};

	using no_move = NonMovable;
} // namespace util::traits