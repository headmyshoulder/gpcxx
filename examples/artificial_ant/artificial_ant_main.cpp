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
    char newl = '\n';
    
       
    board b( santa_fe::x_size , santa_fe::y_size );
    ant_simulation::food_trail_type santa_fe_trail { santa_fe::make_santa_fe_trail( b ) };
    ant_simulation ant_sim_santa_fe { santa_fe_trail, b.get_size_x(), b.get_size_y(), { 0, 0 }, east, 400 };
    

    gpcxx::uniform_symbol< node_type > terminal_gen { std::vector< node_type >{
        node_type { ant_move_task_terminal{} ,          "m" } ,
        node_type { ant_turn_left_task_terminal{} ,     "l" } ,
        node_type { ant_turn_right_task_terminal{} ,    "r" } 
    } };
    
    gpcxx::uniform_symbol< node_type > binary_gen { std::vector< node_type > {
        node_type { prog2{} , "p2" } ,
        node_type { if_food_ahead{} , "if" }
    } };
    
    gpcxx::uniform_symbol< node_type > ternary_gen { std::vector< node_type > {
        node_type { prog3{} , "p3" }
    } };
    
    gpcxx::node_generator< node_type , rng_type , 3 > node_generator {
        { double( terminal_gen.num_symbols() ) , 0 , terminal_gen } ,
        { double( binary_gen.num_symbols() ) , 2 , binary_gen } ,
        { double( ternary_gen.num_symbols() ) , 3 , ternary_gen } };

    size_t population_size = 8192;
    size_t generation_max = 200;
    double number_elite = 2;
    double mutation_rate = 0.1;
    double crossover_rate = 0.6;
    double reproduction_rate = 0.3;
    size_t min_tree_height = 1; 
    size_t max_tree_height = 5;
    size_t tournament_size = 15;

    rng_type rng;
    auto tree_generator = gpcxx::make_basic_generate_strategy( rng , node_generator , max_tree_height , max_tree_height );
      
    typedef gpcxx::static_pipeline< population_type , fitness_type , rng_type > evolver_type;
    evolver_type evolver( number_elite , mutation_rate , crossover_rate , reproduction_rate , rng );
    std::vector< int > fitness( population_size , 0.0 );
    std::vector< tree_type > population( population_size );

//     evolver.mutation_function() = gpcxx::make_mutation(
//         gpcxx::make_simple_mutation_strategy( rng , node_generator ) ,
//         gpcxx::make_tournament_selector( rng , tournament_size ) );
    evolver.mutation_function() = gpcxx::make_mutation(
        gpcxx::make_point_mutation( rng , tree_generator , max_tree_height , 20 ) ,
        gpcxx::make_tournament_selector( rng , tournament_size ) );
    
    
    evolver.crossover_function() = gpcxx::make_crossover( 
        gpcxx::make_one_point_crossover_strategy( rng , max_tree_height ) ,
        gpcxx::make_tournament_selector( rng , tournament_size ) );
    evolver.reproduction_function() = gpcxx::make_reproduce( gpcxx::make_tournament_selector( rng , tournament_size ) );

    evaluator fitness_f;
    gpcxx::timer timer;
    timer.restart();

    std::transform( population.begin() , population.end() , fitness.begin() , [&]( tree_type &t ) { 
        tree_generator( t );
        return fitness_f( t , ant_sim_santa_fe ); 
    });

   
    std::cout << gpcxx::indent( 0 ) << "Generation time " << timer.seconds() << newl;
    std::cout << gpcxx::indent( 1 ) << "Best individuals\n" << gpcxx::best_individuals( population , fitness , 2 , 3 , false ) << newl;
    std::cout << gpcxx::indent( 1 ) << "Statistics : " << gpcxx::calc_population_statistics( population ) << newl << newl;

    timer.restart();
    
    bool has_optimal_fitness { false };
    size_t generation = 1;
    do
    {
        gpcxx::timer iteration_timer;
        iteration_timer.restart();
        evolver.next_generation( population , fitness );
        double evolve_time = iteration_timer.seconds();
        generation++;
        std::transform( population.begin() , population.end() , fitness.begin() , [&]( tree_type const &t ) { return fitness_f( t , ant_sim_santa_fe ); } );
        iteration_timer.restart();
        
        double eval_time = iteration_timer.seconds();
        
        std::cout << gpcxx::indent( 0 ) << "Generation "    << generation << newl;
        std::cout << gpcxx::indent( 1 ) << "Evolve time "   << evolve_time << newl;
        std::cout << gpcxx::indent( 1 ) << "Eval time "     << eval_time << newl;
        std::cout << gpcxx::indent( 1 ) << "Best individuals\n" << gpcxx::best_individuals( population , fitness , 2 , 3 , false ) << newl;
        std::cout << gpcxx::indent( 1 ) << "Statistics : "      << gpcxx::calc_population_statistics( population ) << newl << newl;
        
        has_optimal_fitness = std::any_of( fitness.begin(), fitness.end(), []( int fitness ){ return fitness == 0; } );
        
    } while( !has_optimal_fitness && (generation < generation_max) );
    
    std::cout << "Overall time : " << timer.seconds() << newl;
    return 0;
}

