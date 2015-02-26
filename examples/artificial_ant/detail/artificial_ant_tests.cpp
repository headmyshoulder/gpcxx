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
#include <utility>

char const newl = '\n';



void  paper_tree()
{
    using namespace ant_example;
    typedef typename tree_type::node_type intrusive_node_type;
    typedef std::pair< size_t , std::function< intrusive_node_type() > >
            intrusive_mapped_func;
    typedef std::unordered_map< std::string , intrusive_mapped_func >
            intrusive_mapper_map_type;
    typedef std::function< intrusive_mapped_func( std::string const& ) >
            intrusive_mapper_type;

    intrusive_mapper_map_type mm = intrusive_mapper_map_type {
            std::make_pair( "if" , std::make_pair( 2 , []() { return intrusive_node_type(
                    if_food_ahead {} , "if" ); } ) ) ,
            std::make_pair( "p2" , std::make_pair( 2 , []() { return intrusive_node_type(
                    prog2 {} , "p2" ); } ) ) ,
            std::make_pair( "p3" , std::make_pair( 3 , []() { return intrusive_node_type(
                    prog3 {} , "p3" ); } ) ) ,
            std::make_pair( "m" , std::make_pair( 0 , []() { return intrusive_node_type(
                    ant_move_task_terminal {} , "m" ); } ) ) ,
            std::make_pair( "l" , std::make_pair( 0 , []() { return intrusive_node_type(
                    ant_turn_left_task_terminal {} , "l" ); } ) ) ,
            std::make_pair( "r" , std::make_pair( 0 , []() { return intrusive_node_type(
                    ant_turn_right_task_terminal {} , "r" ); } ) )
    };
    intrusive_mapper_type mapper = [mm]( std::string const& str ) {
        return mm.find( str )->second; };

    std::string koza2 = "if|m|p3|r|if|m|l|p3|l|if|m|r|m";
    tree_type tree;
    gpcxx::read_polish( koza2 , tree , mapper );

    board const b{ santa_fe::x_size, santa_fe::y_size };
    int const max_steps { 400 };

    {
        ant_simulation ant_sim{santa_fe::make_santa_fe_trail(b), santa_fe::x_size, santa_fe::y_size, {0, 0}, east, max_steps};
        while (!ant_sim.is_finsh())
        {
            tree.root()->eval(ant_sim);
        }
        std::cout << "Koza2:" <<  ant_sim.score() << "\n";
    }

}


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
            if(ant_sim.food_in_front())
            {
                ant_sim.move();
            }
            else
            {//PROG3
                ant_sim.turn_left();
                {//PROG2
                    if(ant_sim.food_in_front())
                    {
                        ant_sim.move();
                    }
                    else
                    {
                        ant_sim.turn_right();
                    }
                    {//PROG2
                        ant_sim.turn_right();
                        {//PROG2
                            ant_sim.turn_left();
                            ant_sim.turn_right();
                        }
                    }
                }
                {//PROG2
                    if(ant_sim.food_in_front())
                    {
                        ant_sim.move();
                    }
                    else
                    {
                        ant_sim.turn_left();
                    }
                    ant_sim.move();
                }
            }
        }

        std::cout << "Manual:" << ant_sim.score() << "\n";
    }


    paper_tree();


    size_t count = 0;
    auto santa_fe_trail = santa_fe::make_santa_fe_trail(b);
    for( size_t y = 0; y < santa_fe::y_size; ++y){
        for( size_t x = 0; x < santa_fe::x_size; ++x){
            count += santa_fe_trail[ b.pos_2d_to_1d( { x, y} ) ];
            std::cout << (santa_fe_trail[ b.pos_2d_to_1d( { x, y} ) ] ? 'X' : ' ');
        }
        std::cout << std::endl;
    }
    std::cout << count;


}

