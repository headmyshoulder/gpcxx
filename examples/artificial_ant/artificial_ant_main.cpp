/*
 * gpcxx/examples/artificial_ant/santa_fe_ant_trail_main.cpp
 * Date: 2014-07-18
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

#include <gpcxx/app.hpp>
#include <gpcxx/evolve.hpp>
#include <gpcxx/generate.hpp>
#include <gpcxx/io.hpp>
#include <gpcxx/operator.hpp>
#include <gpcxx/stat.hpp>

#include <string>
#include <iostream>
#include <random>
#include <algorithm>



int main( int argc , char *argv[] )
{
    using rng_type = std::mt19937;
    using namespace ant_example;
    char const newl = '\n';
    
       
    board b(santa_fe::x_size, santa_fe::y_size);
    int const max_steps = 400;
    ant_simulation::food_trail_type santa_fe_trail { santa_fe::make_santa_fe_trail( b ) };
    ant_simulation ant_sim_santa_fe{ santa_fe_trail, b.get_size_x(), b.get_size_y(), { 0, 0 }, east, max_steps };
    

    gpcxx::uniform_symbol< node_type > terminal_gen { std::vector< node_type >{
        node_type { ant_move_task_terminal{} ,          "move" } ,
        node_type { ant_turn_left_task_terminal{} ,     "turn_left" } ,
        node_type { ant_turn_right_task_terminal{} ,    "turn_right" } 
    } };
    
    //this is just filled with a dummy node, 
    gpcxx::uniform_symbol< node_type > unary_gen { std::vector< node_type >{ node_type { do_nothing{} , " " } } };
    
    gpcxx::uniform_symbol< node_type > binary_gen { std::vector< node_type > {
        node_type { prog2{} , "prog2" } ,
        node_type { if_food_ahead{} , "if_food_ahead" }
    } };

    size_t const population_size = 1000;
    size_t const generation_max = 200;
    double const number_elite = 1;
    double const mutation_rate = 0.0;
    double const crossover_rate = 0.6;
    double const reproduction_rate = 0.3;
    size_t const min_tree_height = 1; 
    size_t const max_tree_height = 8;
    size_t const tournament_size = 15;

    rng_type rng;
    std::array< double , 3 > weights = { { double( terminal_gen.num_symbols() ) , 0.0 , double( binary_gen.num_symbols() ) } };    
    auto tree_generator = gpcxx::make_basic_generate_strategy( rng , terminal_gen , unary_gen , binary_gen , max_tree_height , max_tree_height , weights );
      
    using evolver_type = gpcxx::static_pipeline< population_type , fitness_type , rng_type > ;
    evolver_type    evolver( number_elite , mutation_rate , crossover_rate , reproduction_rate , rng );
    fitness_type    fitness( population_size , 0.0 );
    population_type population( population_size );
    for( auto & individum :  population )
        tree_generator( individum );


    evolver.mutation_function() = gpcxx::make_mutation(
        gpcxx::make_simple_mutation_strategy( rng , terminal_gen , unary_gen , binary_gen ) ,
        gpcxx::make_tournament_selector( rng , tournament_size ) );
    
    evolver.crossover_function() = gpcxx::make_crossover( 
        gpcxx::make_one_point_crossover_strategy( rng , max_tree_height ) ,
        gpcxx::make_tournament_selector( rng , tournament_size ) );
    evolver.reproduction_function() = gpcxx::make_reproduce( gpcxx::make_tournament_selector( rng , tournament_size ) );

    evaluator fitness_f;
    
    gpcxx::timer timer; 
    bool    has_optimal_fitness { false };
    size_t  generation { 0 };
    do
    {
        gpcxx::timer iteration_timer;
        if( generation != 0 )
            evolver.next_generation( population , fitness );
        double evolve_time = iteration_timer.seconds();
        
        iteration_timer.restart();
        std::transform( population.begin() , population.end() , fitness.begin() , [&]( tree_type const &t ) { 
            return fitness_f( t , ant_sim_santa_fe ); 
        } );
        double eval_time = iteration_timer.seconds();
        
        std::cout << gpcxx::indent( 0 ) << "Generation "    << generation << newl;
        std::cout << gpcxx::indent( 1 ) << "Evolve time "   << evolve_time << newl;
        std::cout << gpcxx::indent( 1 ) << "Eval time "     << eval_time << newl;
        std::cout << gpcxx::indent( 1 ) << "Best individuals\n" << gpcxx::best_individuals( population , fitness , 2 , 10 ) << newl;
        std::cout << gpcxx::indent( 1 ) << "Statistics : "      << gpcxx::calc_population_statistics( population ) << newl << newl;
        
        generation++;
        has_optimal_fitness = ( 0 == *std::min_element( fitness.begin(), fitness.end() ) );
    } while( !has_optimal_fitness && generation < generation_max );
    
    std::cout << "Overall time : " << timer.seconds() << newl;
    return 0;
}

