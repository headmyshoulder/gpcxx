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

#include "simulation.hpp"
#include "nodes.hpp"
#include "santa_fe_trail.hpp"
#include <gpcxx/io.hpp>
#include <iostream>

char const newl = '\n';

//[ant_move_test
bool ant_move_test()
{
    size_t const board_size_x { 32 };
    size_t const board_size_y { 32 };
    ant_example::board  const the_board { board_size_x , board_size_y };
    ant_example::ant an_ant { the_board.pos_2d_to_1d( { 0, 0 } ), ant_example::east };
    
    
    an_ant.move(the_board);
    
    bool position_is_valid      = an_ant.pos() == the_board.pos_2d_to_1d( { 1, 0 } );
    bool step_count_is_valid    = an_ant.steps_done() == 1;
    
    
    return position_is_valid && step_count_is_valid ;
} 
//]


int main( int argc , char *argv[] )
{
    using namespace ant_example;
   // ant_move_test();



    tree_type tree;
    auto root = tree.root();
    auto a01 = tree.insert_below( root, node_type { if_food_ahead{} , "IF-FOOD-AHEAD" } );

    tree.insert_below( a01,  node_type { ant_move_task_terminal{} , "MOVE" } );
    auto b02 = tree.insert_below( a01,  node_type { prog3{} , "PROG3" } );

    tree.insert_below( b02,  node_type { ant_turn_left_task_terminal{} , "LEFT" } );
    auto c02 = tree.insert_below( b02,  node_type { prog2{} , "PROG2" } );
    auto c03 = tree.insert_below( b02,  node_type { prog2{} , "PROG2" } );


    auto d01 = tree.insert_below( c02,  node_type { if_food_ahead{} , "IF-FOOD-AHEAD" } );
    auto d02 = tree.insert_below( c02,  node_type { prog2{} , "PROG2" } );


    tree.insert_below( d01,  node_type { ant_move_task_terminal{} , "MOVE" } );
    tree.insert_below( d01,  node_type { ant_turn_right_task_terminal{} , "RIGHT" } );

    tree.insert_below( d02,  node_type { ant_turn_right_task_terminal{} , "RIGHT" } );
    auto e01 = tree.insert_below( d02,  node_type { prog2{} , "PROG2" } );


    tree.insert_below( e01,  node_type { ant_turn_left_task_terminal{} , "LEFT" } );
    tree.insert_below( e01,  node_type { ant_turn_right_task_terminal{} , "RIGHT" } );

    auto d03 = tree.insert_below( c03, node_type { if_food_ahead{} , "IF-FOOD-AHEAD" } );

    tree.insert_below( d03,  node_type { ant_move_task_terminal{} , "MOVE" } );
    tree.insert_below( d03,  node_type { ant_turn_left_task_terminal{} , "LEFT" } );

    tree.insert_below( c03,  node_type { ant_move_task_terminal{} , "MOVE" } );

    std::ofstream("artificial_ant_fittest_individual.dot") << gpcxx::graphviz( tree , false );


    board const b{ santa_fe::x_size, santa_fe::y_size };
    int const max_steps { 400 };

    {
        ant_simulation ant_sim{santa_fe::make_santa_fe_trail(b), santa_fe::x_size, santa_fe::y_size, {0, 0}, east, max_steps};
        while (!ant_sim.is_finsh())
        {
            tree.root()->eval(ant_sim);
        }
        std::cout << "Tree:" <<  ant_sim.score() << "\n";
    }

    {
        ant_simulation ant_sim{santa_fe::make_santa_fe_trail(b), santa_fe::x_size, santa_fe::y_size, {0, 0}, east, max_steps};

        while (!ant_sim.is_finsh())
        {
            std::cout << "NEW Run, i have done " << ant_sim.steps_done() << "steps, score " << ant_sim.score() << ", my pos " << ant_sim.ant_position() << ":" << ant_sim.ant_direction() << newl;
            if(ant_sim.food_in_front())
            {
                std::cout << "Food in front, i have done " << ant_sim.steps_done() << "steps, score " << ant_sim.score() << ", my pos " << ant_sim.ant_position() << ":" << ant_sim.ant_direction() << newl;
                ant_sim.move();
                std::cout << "Moved, i have done " << ant_sim.steps_done() << "steps, score " << ant_sim.score() << ", my pos " << ant_sim.ant_position() << ":" << ant_sim.ant_direction() << newl;
            }
            else
            {//PROG3
                std::cout << "NO Food in front, i have done " << ant_sim.steps_done() << "steps, score " << ant_sim.score() << ", my pos " << ant_sim.ant_position() << ":" << ant_sim.ant_direction() << newl;
                ant_sim.turn_left();
                std::cout << "Left, i have done " << ant_sim.steps_done() << "steps, score " << ant_sim.score() << ", my pos " << ant_sim.ant_position() << ":" << ant_sim.ant_direction() << newl;
                {//PROG2
                    if(ant_sim.food_in_front())
                    {
                        std::cout << "Food in front, i have done " << ant_sim.steps_done() << "steps, score " << ant_sim.score() << ", my pos " << ant_sim.ant_position() << ":" << ant_sim.ant_direction() << newl;
                        ant_sim.move();
                        std::cout << "Moved, i have done " << ant_sim.steps_done() << "steps, score " << ant_sim.score() << ", my pos " << ant_sim.ant_position() << ":" << ant_sim.ant_direction() << newl;
                    }
                    else
                    {
                        std::cout << "NO Food in front, i have done " << ant_sim.steps_done() << "steps, score " << ant_sim.score() << ", my pos " << ant_sim.ant_position() << ":" << ant_sim.ant_direction() << newl;
                        ant_sim.turn_right();
                        std::cout << "Right, i have done " << ant_sim.steps_done() << "steps, score " << ant_sim.score() << ", my pos " << ant_sim.ant_position() << ":" << ant_sim.ant_direction() << newl;
                    }
                    {//PROG2
                        ant_sim.turn_right();
                        std::cout << "Right, i have done " << ant_sim.steps_done() << "steps, score " << ant_sim.score() << ", my pos " << ant_sim.ant_position() << ":" << ant_sim.ant_direction() << newl;
                        {//PROG2
                            ant_sim.turn_left();
                            std::cout << "Left, i have done " << ant_sim.steps_done() << "steps, score " << ant_sim.score() << ", my pos " << ant_sim.ant_position() << ":" << ant_sim.ant_direction() << newl;
                            ant_sim.turn_right();
                            std::cout << "Right, i have done " << ant_sim.steps_done() << "steps, score " << ant_sim.score() << ", my pos " << ant_sim.ant_position() << ":" << ant_sim.ant_direction() << newl;
                        }
                    }
                }
                {//PROG2
                    if(ant_sim.food_in_front())
                    {
                        std::cout << "Food in front, i have done " << ant_sim.steps_done() << "steps, score " << ant_sim.score() << ", my pos " << ant_sim.ant_position() << ":" << ant_sim.ant_direction() << newl;
                        ant_sim.move();
                        std::cout << "Moved, i have done " << ant_sim.steps_done() << "steps, score " << ant_sim.score() << ", my pos " << ant_sim.ant_position() << ":" << ant_sim.ant_direction() << newl;
                    }
                    else
                    {
                        std::cout << "NO Food in front, i have done " << ant_sim.steps_done() << "steps, score " << ant_sim.score() << ", my pos " << ant_sim.ant_position() << ":" << ant_sim.ant_direction() << newl;
                        ant_sim.turn_left();
                        std::cout << "Left, i have done " << ant_sim.steps_done() << "steps, score " << ant_sim.score() << ", my pos " << ant_sim.ant_position() << ":" << ant_sim.ant_direction() << newl;
                    }
                    ant_sim.move();
                    std::cout << "Moved, i have done " << ant_sim.steps_done() << "steps, score " << ant_sim.score() << ", my pos " << ant_sim.ant_position() << ":" << ant_sim.ant_direction() << newl;
                }
            }
        }

        std::cout << "Manual:" << ant_sim.score() << "\n";
    }



    int count = 0;
    auto santa_fe_trail = santa_fe::make_santa_fe_trail(b);
    for(int y = 0; y < santa_fe::y_size; ++y){
        for(int x = 0; x < santa_fe::x_size; ++x){
            count += santa_fe_trail[ b.pos_2d_to_1d( { x, y} ) ];
            std::cout << (santa_fe_trail[ b.pos_2d_to_1d( { x, y} ) ] ? 'X' : ' ');
        }
        std::cout << std::endl;
    }
    std::cout << count;


}

