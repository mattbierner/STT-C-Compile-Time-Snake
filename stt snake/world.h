#pragma once

#include "cell.h"
#include "grid.h"
#include "boolean.h"

/**
    Default Size of the gameworld.
*/
constexpr const size_t worldSize = 10;

/**
    The initial game world.
*/
template <
    typename position,
    Direction direction>
using InitialWorld =
    put_grid<
        position,
        MakeSnakeCell<1, direction>,
        gen_grid<worldSize, EmptyCell>>;

/**
    Is a given position in the bounds of the world?
*/
template <typename pos, typename world>
using is_in_bounds =
    std::integral_constant<bool,
        pos::x < world::size && pos::y < world::size>;

/**
    Function that checks if cell is of a given type.
*/
template <CellState type>
struct is_type {
    template <typename pos, typename world>
    struct apply :
        std::integral_constant<bool,
            get_grid<pos, world>::state == type> { };
};

/**
    Is the cell at `pos` in `world` empty?
*/
template <typename pos, typename world>
struct is_empty :
    logical_and<
        is_in_bounds<pos, world>::value,
        is_type<CellState::Empty>,
        pos,
        world> { };

/**
    Is the cell at `pos` in `world` a food cell?
*/
template <typename pos, typename world>
struct is_food :
    logical_and<
        is_in_bounds<pos, world>::value,
        is_type<CellState::Food>,
        pos,
        world> { };

/**
    Can the cell at `pos` in `world` be moved to?
*/
template <typename pos, typename world>
struct is_free :
    std::integral_constant<bool,
        is_food<pos, world>::value || is_empty<pos, world>::value> { };

/**
    Get the weight of a snake cell
*/
template <typename pos, typename world>
struct get_weigth :
    std::integral_constant<size_t,
        get_grid<pos, world>::weight> { };

/**
    Extend that snake by one cell.
*/
template <size_t weight, Direction direction, typename pos, typename world>
using grow_snake = put_grid<pos, MakeSnakeCell<weight, direction>, world>;

/**
    Mark that a collision took place at `pos` in `world`.
*/
template <typename pos, typename world>
using mark_collision = put_grid<pos, CollisionCell, world>;

/**
    Decays all the snake cells by one.
*/
template <typename cell>
struct decay {
    using type = cell;
};

template <unsigned weight, Direction direction>
struct decay<Cell<CellState::Snake, weight, direction>> {
    using type =
        typename std::conditional<weight <= 1,
            EmptyCell,
            Cell<CellState::Snake, weight - 1, direction>>::type;
};

/**
    For a given grid and position, determine if we can continue from `position`
    in `direction`, i.e. without going beyond the bounds of the grid or hitting
    a snake cell.
*/
template <Direction direction, typename pos, typename world>
using can_continue_in_direction =
    std::integral_constant<bool,
        is_in_bounds<get_next_position<direction, pos>, world>::value &&
        is_free<get_next_position<direction, pos>, world>::value>;
