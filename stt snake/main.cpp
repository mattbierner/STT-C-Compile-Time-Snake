#include <iostream>
#include <utility>

#include "cell.h"
#include "direction.h"
#include "list.h"
#include "grid.h"
#include "state.h"

/**
*/
template <Input... inputs>
using PlayerInputs = std::integer_sequence<Input, inputs...>;


template <typename inputs, typename state>
struct play;

template <typename inputs, typename state>
using play_t = typename play<inputs, state>::type;

template <typename state>
struct play<PlayerInputs<>, state> {
    using type = List<state>;

};

template <Input input, Input... inputs, typename state>
struct play<PlayerInputs<input, inputs...>, state> {
    using nextState = step_t<input, state>;
    
    using type = cons_t<
        state,
        play_t<PlayerInputs<inputs...>, nextState>>;
};


int main(int argc, const char* argv[])
{
    using inputs = PlayerInputs<
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

