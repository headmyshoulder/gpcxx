/*
 * gpcxx/examples/artificial_ant/ant_simulation_tests_main.cpp
 * Date: 2014-09-01
 * Author: Gerard Choinka (gerard.choinka+gpcxx@gmail.com)
 * Copyright: Gerard Choinka
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "ant_simulation.hpp"
#include "ant_simulation_nodes.hpp"
#include "santa_fe_trail.hpp"

bool ant_simulation_test()
{
    size_t board_size_x = 32;
    size_t board_size_y = 32;
    ant_example::board b { board_size_x , board_size_y };
    
    ant_example::ant an_ant { b.pos_2d_to_1d( { 0, 0 } ), ant_example::east };
    
    an_ant.move(b);
    
    return an_ant.pos() == b.pos_2d_to_1d( { 1, 0 } ) ;
} 



int main( int argc , char *argv[] )
{
    ant_simulation_test();
}

