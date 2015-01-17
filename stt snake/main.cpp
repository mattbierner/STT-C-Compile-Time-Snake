#include <iostream>

#include "cell.h"
#include "direction.h"
#include "list.h"
#include "grid.h"
#include "snake.h"

/**
    Play a game of snake for a given set of inputs,
    building a list of states.
*/
template <typename inputs, typename state>
struct play;

template <typename inputs, typename state>
using play_t = typename play<inputs, state>::type;

template <typename state>
struct play<PlayerInput<>, state> {
    using type = List<state>;
};

template <Input input, Input... inputs, typename state>
struct play<PlayerInput<input, inputs...>, state> {
    using type = cons_t<
        state,
        play_t<
            PlayerInput<inputs...>,
            step_t<input, state>>>;
};

/**
    Print out the result of the game.
*/
int main(int argc, const char* argv[])
{
    using inputs = PlayerInput<
        Input::Right, Input::Up, Input::None, Input::Right, Input::Up,
        Input::None, Input::None, Input::Left, Input::None, Input::None,
        Input::None, Input::None, Input::None, Input::Down, Input::None,
        Input::None, Input::None, Input::None, Input::None, Input::None,
        Input::None, Input::Right, Input::Up, Input::Left>;

    using state = InitialState;
    
    using game = play_t<inputs, state>;

    Printer<game>::Print(std::cout);
    
    return 0;
}

