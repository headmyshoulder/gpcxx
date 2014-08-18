/*
 * gpcxx/examples/santa_fe_ant_trail/santa_fe_trail.hpp
 * Date: 2014-08-18
 * Author: Gerard Choinka (gerard.choinka+gpcxx@gmail.com)
 * Copyright: Gerard Choinka
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_EXAMPLES_SANTA_FE_ANT_TRAIL_SANTA_FE_TRAIL_HPP_INCLUDED
#define GPCXX_EXAMPLES_SANTA_FE_ANT_TRAIL_SANTA_FE_TRAIL_HPP_INCLUDED

#include "board.hpp"
#include "ant_simulation.hpp"

namespace santa_fe 
{

using namespace ant_example;
size_t const x_size = 32;
size_t const y_size = 32;

char const * const board[y_size] = {
" XXX                            ",
"   X                            ",
"   X                    .XXX..  ",
"   X                    X    X  ",
"   X                    X    X  ",
"   XXXX.XXXXX       .XX..    .  ",
"            X       .        X  ",
"            X       X        .  ",
"            X       X        X  ",
"            X       X        .  ",
"            .       X        .  ",
"            X       .        X  ",
"            X       .        .  ",
"            X       X  ...XXX.  ",
"            X       X  X        ",
"            .   .X...  .        ",
"            .   .      .        ",
"            X   .      .X...    ",
"            X   X          X    ",
"            X   X          .    ",
"            X   X          .    ",
"            X   X      ...X.    ",
"            X   .      X        ",
"            X   .               ",
" ..XX..XXXXX.   X               ",
" X              X               ",
" X              X               ",
" X     .XXXXXXX..               ",
" X     X                        ",
" .     X                        ",
" .XXXX..                        ",
"                                "
};

ant_simulation::food_trail_type make_santa_fe_trail(ant_example::board b)
{
    ant_simulation::food_trail_type santa_fe_trail;
    for(int x = 0; x < santa_fe::x_size; ++x)
        for(int y = 0; y < santa_fe::y_size; ++y)
            if(santa_fe::board[y][x] == 'X')
                santa_fe_trail[b.pos_2d_to_1d({x, y})] = true;
    return santa_fe_trail;
}

} // namespace santa_fe 


#endif // GPCXX_EXAMPLES_SANTA_FE_ANT_TRAIL_SANTA_FE_TRAIL_HPP_INCLUDED