#include <iostream>
#include <sstream>
#include <fstream>

#include "cell.h"
#include "direction.h"
#include "list.h"
#include "grid.h"
#include "snake.h"

/**
    Save the output state
*/
template <typename state>
void serialize_game()
{
    std::ofstream s;
    s.open("current_game.h");
    s << "#import \"snake.h\"\n";
    
    s << "using state = ";
    Serialize<state>::Write(s);
    s << ";";
    s.close();
}

/**
    Print out the result of the game.
*/
int main(int argc, const char* argv[])
{
#include "current_game.h"

#include "get_input.h"
    
    // on first turn, don't consume any input but print the game world.
#if defined(FIRSTTURN)
    using game = state;
#else
    using game = step_t<input, state>;
#endif

    Printer<game>::Print(std::cout);
    
    serialize_game<game>();
    
    return 0;
}

