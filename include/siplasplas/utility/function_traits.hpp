#ifndef SIPLASPLAS_UTILITY_FUNCTION_TRAITS_HPP
#define SIPLASPLAS_UTILITY_FUNCTION_TRAITS_HPP

#include "meta.hpp"
#include <type_traits>
#include <ctti/type_id.hpp>

namespace cpp
{

enum class FunctionKind
{
    INVALID,
    FREE_FUNCTION,
    MEMBER_FUNCTION,
    CONST_MEMBER_FUNCTION,
    FUNCTOR
};

namespace detail
{
    /*
     * With VS2015 we still cannot use expression sfinae to check for operator(),
     * so here we define a trait in the old way
     */
    template<typename T>
    struct IsFunctorClass
    {
        template<typename U>
        static std::true_type check(decltype(&U::operator(), nullptr));
        template<typename U>
        static std::false_type check(...);

        static constexpr bool value = decltype(check<T>(nullptr))::value;
    };

    template<typename Function>
    struct get_function_signature
    {
        static constexpr FunctionKind kind = FunctionKind::INVALID;
    };

    template<typename R, typename... Args>
    struct get_function_signature<R(Args...)>
    {
        using args = meta::list<Args...>;
        using return_type = R;

        static constexpr FunctionKind kind = FunctionKind::FREE_FUNCTION;
    };

    template<typename C, typename R, typename... Args>
    struct get_function_signature<R (C::*)(Args...)> :
        get_function_signature<R(Args...)>
    {
        static constexpr FunctionKind kind = FunctionKind::MEMBER_FUNCTION;
    };

    template<typename C, typename R, typename... Args>
    struct get_function_signature<R (C::*)(Args...) const> :
        get_function_signature<R(Args...)>
    {
        static constexpr FunctionKind kind = FunctionKind::CONST_MEMBER_FUNCTION;
    };

}

template<typename Function, bool IsFunctor = detail::IsFunctorClass<Function>::value>
struct function_signature : public
    detail::get_function_signature<Function>
{};

template<typename Functor>
struct function_signature<Functor, true> :
    detail::get_function_signature<decltype(&Functor::operator())>
{
    static constexpr FunctionKind kind = FunctionKind::FUNCTOR;
};

template<typename Function>
using function_return_type = typename function_signature<Function>::return_type;

template<typename Function>
using function_arguments = typename function_signature<Function>::args;

template<std::size_t Index, typename Function>
using function_argument = meta::get_t<Index, function_arguments<Function>>;

template<typename Function>
constexpr FunctionKind function_kind()
{
    return function_signature<Function>::kind;
}

template<typename Function>
constexpr FunctionKind function_kind(Function)
{
    return function_kind<Function>();
}

template<typename A, typename B>
struct equal_signature : std::is_same<
    function_arguments<A>, function_arguments<B>
>{};

}

#endif // SIPLASPLAS_UTILITY_FUNCTION_TRAITS_HPP
