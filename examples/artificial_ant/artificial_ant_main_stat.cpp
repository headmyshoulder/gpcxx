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
#include <sstream>
#include <random>
#include <algorithm>


#include <fstream>
#include <map>


int main( int argc , char *argv[] )
{
    using namespace ant_example;
    using rng_type = std::mt19937;
    std::random_device rd;
    rng_type rng( rd() );
    char const newl { '\n' };
    
    //[world_definition 
    board const b{ santa_fe::x_size, santa_fe::y_size };
    int const max_steps { 400 };
    ant_simulation::food_trail_type santa_fe_trail { santa_fe::make_santa_fe_trail( b ) };
    ant_simulation                  ant_sim_santa_fe{ santa_fe_trail, b.get_size_x(), b.get_size_y(), { 0, 0 }, east, max_steps };
    //]

    //[node_generate
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
    //]
    
    //[envolve_settings
    size_t const population_size = 500;
    size_t const generation_max = 51;
    double const number_elite = 0;
    double const mutation_rate = 0.0;
    double const crossover_rate = 0.9;
    double const reproduction_rate = 0.1;
    size_t const min_tree_height_gen = 1; 
    size_t const max_tree_height_gen = 6;
    size_t const max_tree_height = 17;
    size_t const tournament_size = 9;
    //]
        
    
    std::map< size_t , size_t > hist;
    gpcxx::timer overall_timer; 
    size_t r_max = 10000;
    for( size_t r=0 ; r<r_max ; ++r )
    {
        population_type population;
        population.reserve( population_size );
        
        //[tree_generator
        auto tree_generator = gpcxx::make_ramp( rng , node_generator , min_tree_height_gen , max_tree_height_gen , 0.5 );
        for( size_t i=0 ; i<population_size ; ++i )
        {
            size_t count = 0;
            while( true )
            {
                ++count;
                if( count == 1000 )
                {
                    std::string msg = "Could not create tree " + std::to_string( i ) ;
                    throw std::runtime_error( msg );
                }
                
                tree_type t;
                tree_generator( t );
                bool equal = false;
                for( size_t j=0 ; j<i ; ++j )
                {
                    if( population[j] == t )
                    {
                        // std::cout << "EQUAL : " << gpcxx::simple( population[j] ) << " = " << gpcxx::simple( t ) << std::endl;
                        equal = true;
                        break;
                    }
                }
                if( !equal )
                {
                    population.push_back( std::move( t ) );
                    break;
                }
            }
        }
        //]

        //[evolver_definition
        using evolver_type = gpcxx::static_pipeline< population_type , fitness_type , rng_type > ;
        evolver_type evolver( number_elite , mutation_rate , crossover_rate , reproduction_rate , rng );

        evolver.mutation_function() = gpcxx::make_mutation(
            gpcxx::make_point_mutation( rng , tree_generator , max_tree_height , 20 ) ,
            gpcxx::make_tournament_selector( rng , tournament_size ) );
        
        evolver.crossover_function() = gpcxx::make_crossover( 
            gpcxx::make_one_point_crossover_strategy( rng , max_tree_height ) ,
            gpcxx::make_tournament_selector( rng , tournament_size ) );
        
        evolver.reproduction_function() = gpcxx::make_reproduce( gpcxx::make_tournament_selector( rng , tournament_size ) );
        //]
        
        //[fitness_defintion
        evaluator       fitness_f;
        fitness_type    fitness( population_size , 0.0 );
        //]
        
        //[generation_loop
        bool    has_optimal_fitness { false };
        size_t  generation { 0 };
        do
        {
            gpcxx::timer iteration_timer;
            if( generation != 0 )
                evolver.next_generation( population , fitness );
            double evolve_time { iteration_timer.seconds() };
            
            iteration_timer.restart();
            //[fitness_calculation
            std::transform( population.begin() , population.end() , fitness.begin() , [&]( tree_type const &t ) { 
                return fitness_f( t , ant_sim_santa_fe ); 
            } );
            //]
            
            
//             {
//                 std::ofstream fout( std::string(  "pop_" ) + std::to_string( generation ) );
//                 std::vector< size_t > indices( population_size );
//                 std::iota( indices.begin() , indices.end() , 0 );
//                 std::sort( indices.begin() , indices.end() , [&]( size_t i , size_t j ) -> bool {
//                     return fitness[i] < fitness[j]; } );
//                 
//                 for( size_t j=0 ; j<population_size ; ++j )
//                 {
//                     size_t i = indices[j];
//                     fout << j << " " << i << " "
//                         << fitness[i] << " " << population[i].root().height() << " "
//                         << gpcxx::simple( population[i] , false )
//                         << std::endl;
//                 }
//             }
    
            /*        
            {
                using namespace std;
                std::vector< size_t > idx;
                auto iter = gpcxx::sort_indices( fitness , idx );
                auto best = population[ idx[0] ];
                ant_simulation sim { santa_fe_trail, b.get_size_x(), b.get_size_y(), { 0, 0 }, east, max_steps };
                for( size_t i=0 ; i<100 ; ++i )
                {
                    cout << simple( best , false ) << endl << endl;
                    best.root()->eval( sim );
                    cout << sim.get_board_as_str() << endl;
                    usleep( 50 * 1000 );
                }
            }*/
                
            
            double eval_time = iteration_timer.seconds();
            
            
            //[breakup_conditions
            generation++;
            size_t best_fitness = *std::min_element( fitness.begin(), fitness.end() );
            has_optimal_fitness = ( 0 == best_fitness );
            
//            std::cout << gpcxx::indent( 0 ) << "Generation "    << generation << newl;
//            std::cout << gpcxx::indent( 1 ) << "Best fitness : " << best_fitness << newl;
//             std::cout << gpcxx::indent( 1 ) << "Evolve time "   << evolve_time << newl;
//             std::cout << gpcxx::indent( 1 ) << "Eval time "     << eval_time << newl;
//             std::cout << gpcxx::indent( 1 ) << "Best individuals\n" << gpcxx::best_individuals( population , fitness , 2 , 3 , false ) << newl;
//            std::cout << gpcxx::indent( 1 ) << "Statistics : "      << gpcxx::calc_population_statistics( population ) << newl;

        } while( !has_optimal_fitness && generation < generation_max );
        //]
        
        hist[ generation ] += 1;
        
        std::cout << r << " / " << r_max << " " << generation << std::endl;
        
        std::ofstream fout( "hist.dat" );
        size_t cc = 0;
        double cumulativa = 0.0;
        for( size_t i=0 ; i<=generation_max ; ++i )
        {
            auto iter = hist.find( i );
            size_t v1 = ( iter != hist.end() ) ? iter->second : 0;
            double val = double( v1 ) / double( r + 1 );
            cc += v1;
            cumulativa += val;
            fout << i << " " << val << " " << cumulativa << " " << v1 << " " << cc << " " << r << newl;
        }
    }
    
    
    
    std::cout << "Overall time : " << overall_timer.seconds() << newl;
    
    return 0;
}
