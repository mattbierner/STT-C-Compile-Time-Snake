#pragma once

#include "list.h"
#include "functor.h"
#include "printer.h"

/**
    Position in two dimensional space.
*/
template <size_t xVal, size_t yVal>
struct Position {
    static const size_t x = xVal;
    static const size_t y = yVal;
};

/**
    Two dimensional grid of values.
    
    Stores a list of column lists that store cells.
*/
template <typename r>
struct Grid {
    using rows = r;
    
    static const size_t size = list_size_t<r>::value;
};

/**
    Generate an NxN grid of `value`.
*/
template <size_t N, typename value>
using gen_grid = Grid<gen_t<N, gen_t<N, value>>>;


/**
    Get the element at `pos(x, y)` in a grid.
*/
template <typename pos, typename grid>
using get_grid = get_t<pos::x, get_t<pos::y, typename grid::rows>>;


/**
    Set the element at `pos(x, y)` in a grid to `value`.
*/
template <typename pos, typename value, typename grid>
using put_grid = Grid<
    put_t<
        pos::y,
        put_t<pos::x, value, get_t<pos::y, typename grid::rows>>,
        typename grid::rows>>;


/**
    Prints out a grid.
*/
template <>
struct Printer<Grid<List<>>>
{
    static void Print(std::ostream& output) { /* noop */ }
};

template <typename x, typename... xs>
struct Printer<Grid<List<x, xs...>>>
{
    static void Print(std::ostream& output)
    {
        Printer<x>::Print(output);
        output << "\n";
        Printer<Grid<List<xs...>>>::Print(output);
    }
};

/**
    Maps a functor `f` over a grid.
*/
template <typename rows, template<typename> class f>
struct Fmap<Grid<rows>, f> {

    template <typename val>
    struct fmap_inner {
        using type = fmap_t<val, f>;
    };
    
    using type = Grid<
        fmap_t<rows, fmap_inner>>;
};
