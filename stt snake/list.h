/**
    Basic 1D list type.
*/
#pragma once

#include <utility>

#include "functor.h"
#include "printer.h"

/**
    Ordered list of types.
*/
template <typename...>
struct List { };

/**
    Get size of a list;
*/
template <typename list>
struct list_size;

template <typename... xs>
struct list_size<List<xs...>> :
    std::integral_constant<size_t, sizeof...(xs)> { };

template <typename list>
using list_size_t = typename list_size<list>::type;


/**
    Get the head of an list
*/
template <typename list>
struct car;

template <typename x, typename... xs>
struct car<List<x, xs...>> {
    using type = x;
};

template <typename list>
using car_t = typename car<list>::type;

/**
    Get the rest of a list, excluding the head.
*/
template <typename list>
struct cdr;

template <typename x, typename... xs>
struct cdr<List<x, xs...>> {
    using type = List<xs...>;
};

template <typename list>
using cdr_t = typename cdr<list>::type;

/**
    Prepend a value onto an `integer_sequence`.
*/
template <typename x, typename list>
struct cons;

template <typename x, typename... xs>
struct cons<x, List<xs...>> {
     using type = List<x, xs...>;
};

template <typename x, typename list>
using cons_t = typename cons<x, list>::type;

/**
    Lookup a value in an `integer_sequence`.
*/
template <size_t N, typename list>
struct get;

template <size_t N, typename x, typename... xs>
struct get<N, List<x, xs...>> {
    using type = typename get<N - 1, List<xs...>>::type;
};

template <typename x, typename... xs>
struct get<0, List<x, xs...>> {
    using type = x;
};

template <size_t N, typename list>
using get_t = typename get<N, list>::type;

/**
   Put a value.
*/
template <size_t N, typename newValue, typename list>
struct put;

template <typename newValue, typename x, typename... xs>
struct put<0, newValue, List<x, xs...>> {
    using type = List<newValue, xs...>;
};

template <size_t N, typename newValue, typename x, typename... xs>
struct put<N, newValue, List<x, xs...>> {
    using type = cons_t<x, typename put<N - 1, newValue, List<xs...>>::type>;
};

template <size_t N, typename newValue, typename list>
using put_t = typename put<N, newValue, list>::type;


/**
    Build a list of `element` repeated `N` times.
*/
template <size_t N, typename element>
struct gen {
    using type = cons_t<element, typename gen<N - 1, element>::type>;
};

template <typename element>
struct gen<0, element> {
    using type = List<>;
};

template <size_t N, typename element>
using gen_t = typename gen<N, element>::type;

/**
    Maps a functor `f` over a list.
*/
template <template<typename> class f>
struct Fmap<List<>, f> {
    using type = List<>;
};

template <typename x, typename... xs, template<typename> class f>
struct Fmap<List<x, xs...>, f> {
    using type = cons_t<
        typename f<x>::type,
        fmap_t<List<xs...>, f>>;
};


/**
    Prints out the list.
*/
template <>
struct Printer<List<>>
{
    static void Print(std::ostream& output) { /* noop */ }
};

template <typename x, typename... xs>
struct Printer<List<x, xs...>>
{
    static void Print(std::ostream& output)
    {
        Printer<x>::Print(output);
        Printer<List<xs...>>::Print(output);
    }
};
