#pragma once

#include "direction.h"
#include "cell.h"
#include "input.h"
#include "grid.h"
#include "printer.h"
#include "random.h"
#include "world.h"

/**
    Default Size of the gameworld.
*/
constexpr const size_t worldSize = 10;

/**
*/
template <unsigned max>
using InitialRandom = prandom::PseudoRandomGenerator<max,
    prandom::Lfsr<
        prandom::bitset<true, false, true, false, true, true, false, false, true, true, true, false, false, false, false, true>,
        prandom::indicies<0, 10, 12, 13, 15>>>;

/**
*/
enum class GameState : unsigned
{
    Alive,
    Dead
};

/**
*/
template <
    GameState currentGameState,
    typename currentPosition,
    Direction currentDirection,
    typename currentWorld,
    typename currentRandom>
struct State
{
    static const GameState gameState = currentGameState;

    using position = currentPosition;
    
    using world = currentWorld;
    
    static const Direction direction = currentDirection;
    
    using random = currentRandom;
    
    template <typename newWorld>
    using set_world = State<gameState, position, direction, newWorld, random>;
    
    template <typename newRandom>
    using set_random = State<gameState, position, direction, world, newRandom>;
};


/**
*/
template <typename state, typename = void>
struct put_food {
    using type = typename put_food<
        typename state::template set_random<typename state::random::next::next>>::type;
    
};

template <typename state>
struct put_food<state,
    typename std::enable_if<
        is_empty<
            Position<state::random::value, state::random::next::value>,
            typename state::world>
        ::value>::type>
{
    static const size_t targetX = state::random::value;
    static const size_t targetY = state::random::next::value;
    
    using targetPosition = Position<targetX, targetY>;
    
    using type = typename state::template set_world<
        put_grid<
            targetPosition,
            FoodCell,
            typename state::world>>;
};

template <typename state>
using put_food_t = typename put_food<state>::type;


/**
    Create the initial game world.
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
*/
using InitialState =
    put_food_t<State<
        GameState::Alive,
        Position<worldSize / 2, worldSize / 2>,
        Direction::Right,
        InitialWorld<
            Position<worldSize / 2, worldSize / 2>,
            Direction::Right>,
        InitialRandom<worldSize>>>;


template <Input input, typename state>
struct step {
    static const Direction direction = get_new_direction<state::direction, input>::value;

    using nextPosition = get_next_position<direction, typename state::position>;
    
    static const unsigned currentWeight = get_weigth<typename state::position, typename state::world>::value;
    
    struct consume
    {
        using newWorld = grow_snake<currentWeight + 1, direction, nextPosition, typename state::world>;
        
        using type = put_food_t<State<
            GameState::Alive,
            nextPosition,
            direction,
            newWorld,
            typename state::random>>;
    };
    
    struct regular
    {
        using nextWorld = fmap_t<typename state::world, decay>;
    
        static const bool died = !is_empty<nextPosition, nextWorld>::value;
    
        struct die
        {
            using newWorld = mark_collision<
                typename std::conditional<is_in_bounds<nextPosition, nextWorld>::value,
                    nextPosition,
                    typename state::position>::type,
                typename state::world>;
        
            using type = State<
                GameState::Dead,
                nextPosition,
                direction,
                newWorld,
                typename state::random>;
        };
        
        struct live
        {
            using newWorld = grow_snake<
                currentWeight,
                direction,
                nextPosition,
                nextWorld>;
        
            using type = State<
                GameState::Alive,
                nextPosition,
                direction,
                newWorld,
                typename state::random>;
        };
    
        using type = typename lazy_conditional<
            died,
            die,
            live>::type;
    };
    
    using type = typename lazy_conditional<
        is_food<nextPosition, typename state::world>::value,
        consume,
        regular>::type;
   // using newWorld = fmap_t<state, decay>;

};

template <Input input, typename state>
using step_t = typename step<input, state>::type;

template <
    Input input,
    typename position,
    Direction direction,
    typename world,
    typename random>
struct step<input, State<GameState::Dead, position, direction, world, random>> {
    using type = State<GameState::Dead, position, direction, world, random>;
};

/**
*/
template <
    GameState gameState,
    typename position,
    Direction direction,
    typename world,
    typename random>
struct Printer<State<gameState, position, direction, world, random>>
{
    static void Print(std::ostream& output)
    {
        output << "----" << (gameState == GameState:: Dead ? " dead " : "------") << "----" << "\n";
        Printer<world>::Print(output);
    }
};

/**
*/
template <
    GameState gameState,
    typename position,
    Direction direction,
    typename world,
    typename random,
    template<typename> class f>
struct Fmap<State<gameState, position, direction, world, random>, f> {
    using type = State<gameState, position, direction, fmap_t<world, f>, random>;
};