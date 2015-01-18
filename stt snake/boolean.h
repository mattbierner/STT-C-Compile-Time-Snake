#pragma once

/**
    Encodes a boolean as a type so we can perform call by need operations on it.
*/
template <bool value>
struct boolean : std::integral_constant<bool, value> {
    template <typename p, typename... args>
    using logical_and = typename p::template apply<args...>;
};

template <>
struct boolean<false> : std::integral_constant<bool, false> {
    template <typename p, typename...>
    using logical_and = boolean<false>;
};

template <bool value, typename s, typename... args>
using logical_and = typename boolean<value>::template logical_and<s, args...>;

/**
    Branch on `value` using call by need evaluation.
*/
template <bool value, typename consequent, typename alternate>
struct branch {
    using type = typename consequent::type;
};

template <typename consequent, typename alternate>
struct branch<false, consequent, alternate> {
    using type = typename alternate::type;
};

template <bool value, typename consequent, typename alternate>
using branch_t = typename branch<value, consequent, alternate>::type;
