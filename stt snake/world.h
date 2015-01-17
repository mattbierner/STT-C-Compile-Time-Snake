#pragma once

#include "cell.h"
#include "grid.h"
#include "boolean.h"

/**
    Is a given position in the bounds of the world?
*/
template <typename pos, typename world>
using is_in_bounds =
    std::integral_constant<bool,
        pos::x < world::size && pos::y < world::size>;

template <CellState type>
struct is_type {
    template <typename pos, typename world>
    struct apply :
        std::integral_constant<bool,
            get_grid<pos, world>::state == type> { };
};

/**
    Checks if a given cell in the world is empty.
*/
template <typename pos, typename world>
struct is_empty :
    pred_and<
        is_in_bounds<pos, world>::value,
        is_type<CellState::Empty>,
        pos,
        world> { };

template <typename pos, typename world>
struct is_food :
    pred_and<
        is_in_bounds<pos, world>::value,
        is_type<CellState::Food>,
        pos,
        world> { };

template <typename pos, typename world>
struct is_free :
    std::integral_constant<bool,
        is_food<pos, world>::value || is_empty<pos, world>::value> { };

/**
*/
template <typename pos, typename world>
struct get_weigth :
    std::integral_constant<size_t,
        get_grid<pos, world>::weight> { };

/**
*/
template <size_t weight, Direction direction, typename pos, typename world>
using grow_snake = put_grid<pos, MakeSnakeCell<weight, direction>, world>;

/**
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
*/
template <Direction direction, typename pos>
using get_next_position =
    Position<
        pos::x + direction_delta_x<direction>::value,
        pos::y + direction_delta_y<direction>::value>;


/**
    For a given grid and position, determine if we can continue from `position`
    in `direction` (i.e. without going beyond the bounds of the grid.)
*/
template <Direction direction, typename pos, typename world>
using can_continue_in_direction =
    std::integral_constant<bool,
        is_in_bounds<get_next_position<direction, pos>, world>::value &&
        is_free<get_next_position<direction, pos>, world>::value>;
