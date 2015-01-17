#pragma once

template <bool value>
struct pred : std::integral_constant<bool, value> {
    template <typename p, typename a, typename b>
    using pred_and = typename p::template apply<a, b>;
};

template <>
struct pred<false> : std::integral_constant<bool, false> {
    template <typename p, typename a, typename b>
    using pred_and = pred<false>;
};


template <bool value, typename s, typename a, typename b>
using pred_and = typename pred<value>::template pred_and<s, a, b>;



template <bool value, typename consequent, typename alternate>
struct lazy_conditional {
    using type = typename consequent::type;
};

template <typename consequent, typename alternate>
struct lazy_conditional<false, consequent, alternate> {
    using type = typename alternate::type;
};