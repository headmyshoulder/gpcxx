/*
 * lorenz_single.cpp
 * Date: 2016-02-26
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "generate_data.hpp"
#include "tree_types.hpp"
#include "program_options.hpp"
#include "serialize.hpp"

#include <gpcxx/app.hpp>
#include <gpcxx/eval.hpp>
#include <gpcxx/evolve.hpp>
#include <gpcxx/tree.hpp>
#include <gpcxx/intrusive_nodes.hpp>
#include <gpcxx/operator.hpp>
#include <gpcxx/stat.hpp>
#include <gpcxx/generate.hpp>
#include <gpcxx/io.hpp>

#include <boost/program_options.hpp>

#include <iostream>

namespace po = boost::program_options;


int main( int argc , char** argv )
{
    auto options = dynsys::get_options();
    auto positional_options = dynsys::get_positional_options();
    
    po::options_description cmdline_options;
    cmdline_options.add( options ).add( positional_options.second );
    
    po::variables_map vm;
    try
    {
        po::store( po::command_line_parser( argc , argv ).options( cmdline_options ).positional( positional_options.first ).run() , vm );
        po::notify( vm );
    }
    catch( std::exception& e )
    {
        std::cerr << "Error " << e.what() << "\n\n";
        std::cerr << cmdline_options << "\n";
        return -1;
    }
    
    dynsys::rng_type rng;
    
    
    //[ create_lorenz_data
    auto training_data = dynsys::generate_data();
    
    std::array< std::pair< double , double > , dynsys::dim > xstat , ystat;
    for( size_t i=0 ; i<dynsys::dim ; ++i )
    {
        xstat[i].first = ystat[i].first = 0.0;
        xstat[i].second = ystat[i].second = 1.0;
    }
    if( vm.count( "normalize" ) )
    {
        xstat = dynsys::normalize_data( training_data.first );
        ystat = dynsys::normalize_data( training_data.second );
    }
    // plot_data( training_data );
    //]
    
    //[ lorenz_define_node_generator
    auto node_generator = dynsys::create_node_generator();
    //]
    
    
    //[ define_gp_parameters
//     size_t population_size = 256;
//     size_t generation_size = 20;
    size_t population_size = 512 * 32;
    size_t generation_size = 2000;
    
    
    size_t number_elite = 1;
    double mutation_rate = 0.2;
    double crossover_rate = 0.6;
    double reproduction_rate = 0.3;
    size_t min_tree_height = 1 , max_tree_height = 8;
    size_t tournament_size = 15;
    //]

    //[ define_population_and_fitness
    using population_type = std::vector< dynsys::tree_type >;
    using fitness_type = std::vector< double >;
    //]
    
    //[ define_evolution
    using evolver_type = gpcxx::dynamic_pipeline< population_type , fitness_type , dynsys::rng_type >;
    evolver_type evolver( rng , number_elite );
    //]
    
    //[define_evaluator
    using evaluator = struct {
        using context_type = gpcxx::regression_context< double , dynsys::dim >;
        using value_type = double;
        value_type operator()( dynsys::tree_type const& t , context_type const& c ) const {
            return t.root()->eval( c );
        } };
    //]
        
    //[define_genetic_operators
    auto tree_generator = gpcxx::make_ramp( rng , node_generator , min_tree_height , max_tree_height , 0.5 );
    auto fitness_f = gpcxx::make_regression_fitness( evaluator {} );
    evolver.add_operator(
        gpcxx::make_mutation(
            gpcxx::make_point_mutation( rng , tree_generator , max_tree_height , 20 ) ,
            gpcxx::make_tournament_selector( rng , tournament_size )
        ) , mutation_rate );
    evolver.add_operator(
        gpcxx::make_crossover( 
            gpcxx::make_one_point_crossover_strategy( rng , 10 ) ,
            gpcxx::make_tournament_selector( rng , tournament_size )
        ) , crossover_rate );
    evolver.add_operator(
        gpcxx::make_reproduce(
            gpcxx::make_tournament_selector( rng , tournament_size )
        ) , reproduction_rate );
    //]

    
    std::ofstream fout { vm[ "evolution" ].as< std::string >() };
    std::ofstream fout2 { vm[ "result" ].as< std::string >() };
    fout2.precision( 14 );
    for( size_t j=0 ; j<dynsys::dim ; ++j )
        fout2 << xstat[j].first << " " << xstat[j].second << " " << ystat[j].first << " " << ystat[j].second << "\n";
    fout2 << std::endl << std::endl;
    
    
    
    std::array< dynsys::tree_type , dynsys::dim > winner;
    for( size_t dimension = 0 ; dimension < dynsys::dim ; ++dimension )
    {
        fitness_type fitness ( population_size , 0.0 );
        population_type population ( population_size );

        gpcxx::regression_training_data< double , dynsys::dim > problem;
        for( size_t i=0 ; i<training_data.first.size() ; ++i )
        {
            problem.y.push_back( training_data.second[i][ dimension ] );
            for( size_t j=0 ; j<dynsys::dim ; ++j )
                problem.x[j].push_back( training_data.first[i][j] );
        }
        
        //[init_population
        for( size_t i=0 ; i<population.size() ; ++i )
        {
            tree_generator( population[i] );
            fitness[i] = fitness_f( population[i] , problem );
        }
        
        std::cout << "Best individuals" << std::endl << gpcxx::best_individuals( population , fitness ) << std::endl;
        std::cout << "Statistics : " << gpcxx::calc_population_statistics( population ) << std::endl;
        std::cout << std::endl << std::endl;
        //]
        
        //[main_loop
        for( size_t i=0 ; i<generation_size ; ++i )
        {
            evolver.next_generation( population , fitness );
            for( size_t i=0 ; i<population.size() ; ++i )
                fitness[i] = fitness_f( population[i] , problem );
            
            std::cout << "Iteration " << i << std::endl;
            std::cout << "Best individuals" << std::endl << gpcxx::best_individuals( population , fitness , 1 ) << std::endl;
            std::cout << "Statistics : " << gpcxx::calc_population_statistics( population ) << std::endl << std::endl;
            
            fout << "Iteration " << i << std::endl;
            fout << "Best individuals" << std::endl << gpcxx::best_individuals( population , fitness , 1 ) << std::endl;
            fout << "Statistics : " << gpcxx::calc_population_statistics( population ) << std::endl << std::endl;
            
            auto min_fitness = * ( std::min_element( fitness.begin() , fitness.end() ) );
            if( std::abs( min_fitness ) < 1.0e-7 )
            {
                std::cout << "Minimal fitness is small then 1.0e-7. Stopping now." << std::endl << std::endl << std::endl << std::endl;
                fout << "Minimal fitness is small then 1.0e-7. Stopping now." << std::endl << std::endl << std::endl << std::endl;
                break;
            }
        }
        //]
        
        fout2 << gpcxx::best_individuals( population , fitness , 0 ) << std::endl << std::endl;
        
        std::vector< size_t > idx;
        gpcxx::sort_indices( fitness , idx );
        winner[dimension] = population[ idx[0] ];
    }
    
    std::ofstream winner_out { vm[ "winner" ].as< std::string >() };
    winner_out << dynsys::serialize_winner( winner , xstat , ystat ) << "\n";
    
    
    return 0;
}