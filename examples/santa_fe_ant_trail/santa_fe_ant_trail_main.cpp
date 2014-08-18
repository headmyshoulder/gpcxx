/*
 * gpcxx/examples/santa_fe_ant_trail/santa_fe_ant_trail_main.cpp
 * Date: 2014-07-18
 * Author: Gerard Choinka (gerard.choinka+gpcxx@gmail.com)
 * Copyright: Gerard Choinka
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <string>
#include <iostream>
#include <random>
#include <algorithm>


#include <gpcxx/app/timer.hpp>
#include <gpcxx/evolve/static_pipeline.hpp>
#include <gpcxx/generate/ramp.hpp>
#include <gpcxx/generate/uniform_symbol.hpp>
#include <gpcxx/io/best_individuals.hpp>
#include <gpcxx/operator/crossover.hpp>
#include <gpcxx/operator/mutation.hpp>
#include <gpcxx/operator/one_point_crossover_strategy.hpp>
#include <gpcxx/operator/reproduce.hpp>
#include <gpcxx/operator/simple_mutation_strategy.hpp>
#include <gpcxx/operator/tournament_selector.hpp>
#include <gpcxx/stat/population_statistics.hpp>



#include "ant_simulation.hpp"
#include "ant_simulation_nodes.hpp"
#include "santa_fe_trail.hpp"


int main( int argc , char *argv[] )
{
    using rng_type = std::mt19937;
    using namespace ant_example;
    char const newl = '\n';
    
    board b(santa_fe::x_size, santa_fe::y_size);
    ant_simulation::food_tail_type santa_fe_tail { santa_fe::make_santa_fe_tail( b ) };
    ant_simulation ant_sim_santa_fe{ santa_fe_tail, b.get_size_x(), b.get_size_y(), { 0, 0 }, east, 400 };

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

    size_t population_size = 1000;
    size_t generation_max = 200;
    double number_elite = 1;
    double mutation_rate = 0.0;
    double crossover_rate = 0.6;
    double reproduction_rate = 0.3;
    size_t min_tree_height = 1; 
    size_t max_tree_height = 8;
    size_t tournament_size = 15;

    rng_type rng;
    std::array< double , 3 > weights = { { double( terminal_gen.num_symbols() ) , 0.0 , double( binary_gen.num_symbols() ) } };    
    auto tree_generator = gpcxx::make_basic_generate_strategy( rng , terminal_gen , unary_gen , binary_gen , max_tree_height , max_tree_height , weights );
      
    typedef gpcxx::static_pipeline< population_type , fitness_type , rng_type > evolver_type;
    evolver_type evolver( number_elite , mutation_rate , crossover_rate , reproduction_rate , rng );
    std::vector< int > fitness( population_size , 0.0 );
    std::vector< tree_type > population( population_size );

    evolver.mutation_function() = gpcxx::make_mutation(
        gpcxx::make_simple_mutation_strategy( rng , terminal_gen , unary_gen , binary_gen ) ,
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
    std::cout << gpcxx::indent( 1 ) << "Best individuals\n" << gpcxx::best_individuals( population , fitness , 1 , 10 ) << newl;
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
        iteration_timer.restart();
        
        double eval_time = iteration_timer.seconds();
        
        std::cout << gpcxx::indent( 0 ) << "Generation "    << generation << newl;
        std::cout << gpcxx::indent( 1 ) << "Evolve time "   << evolve_time << newl;
        std::cout << gpcxx::indent( 1 ) << "Eval time "     << eval_time << newl;
        std::cout << gpcxx::indent( 1 ) << "Best individuals\n" << gpcxx::best_individuals( population , fitness , 2 , 10 ) << newl;
        std::cout << gpcxx::indent( 1 ) << "Statistics : "      << gpcxx::calc_population_statistics( population ) << newl << newl;
        
        bool has_optimal_fitness = std::any_of( fitness.begin(), fitness.end(), []( int fitness ){ return fitness == 0; } );
    }while(!has_optimal_fitness && generation < generation_max );
    
    std::cout << "Overall time : " << timer.seconds() << newl;
    return 0;
}

