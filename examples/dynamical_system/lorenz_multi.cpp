/*
 * lorenz_multi.cpp
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
    size_t population_size = 256 ;
    size_t generation_size = 20;
//     size_t population_size = 512 * 32;
//     size_t generation_size = 2000;
    
    size_t number_elite = 1;
    double mutation_rate = 0.2;
    double crossover_rate = 0.6;
    double reproduction_rate = 0.3;
    size_t min_tree_height = 1 , max_tree_height = 8;
    size_t tournament_size = 15;
    //]
    
    
    //[ define_population_and_fitness
    using population_type = std::vector< dynsys::individual_type >;
    using fitness_type = std::vector< double >;
    
    fitness_type fitness ( population_size , 0.0 );
    population_type population ( population_size );
    //]
    
    
    //[ define_evolution
    using evolver_type = gpcxx::dynamic_pipeline< population_type , fitness_type , dynsys::rng_type >;
    evolver_type evolver( rng , number_elite );
    //]
    
    //[define_evaluator
    
    auto evaluator = []( auto const& individual , auto const& context ) {
        return dynsys::state_type {{
            individual[0].root()->eval( context ) ,
            individual[1].root()->eval( context ) ,
            individual[2].root()->eval( context ) }}; };
    
    auto distance = []( auto const& y , auto const& ytrain ) {
        const double weight_x = 1.0;
        const double weight_y = 1.0;
        const double weight_z = 1.0;
        dynsys::state_type diff {{ y[0] - ytrain[0] , y[1] - ytrain[1] , y[2] - ytrain[2] }};
        return weight_x * diff[0] * diff[0] + weight_y * diff[1] * diff[1] + weight_z * diff[2] * diff[2]; };
    //]
                
    //[define_genetic_operators
    auto tree_generator = gpcxx::make_ramp( rng , node_generator , min_tree_height , max_tree_height , 0.5 );
    auto fitness_f1 = gpcxx::make_multi_regression_fitness( evaluator , distance );
    auto fitness_f = [ fitness_f1 ]( auto const& individual , auto const& x , auto const& y ) {
        return fitness_f1.get_chi2( individual , x , y ); };
    evolver.add_operator(
        gpcxx::make_multi_mutation(
            rng , 
            gpcxx::make_point_mutation( rng , tree_generator , max_tree_height , 20 ) ,
            gpcxx::make_tournament_selector( rng , tournament_size )
        ) , mutation_rate );
    evolver.add_operator(
        gpcxx::make_multi_crossover(
            rng ,
            gpcxx::make_one_point_crossover_strategy( rng , 10 ) ,
            gpcxx::make_tournament_selector( rng , tournament_size )
        ) , crossover_rate );
    evolver.add_operator(
        gpcxx::make_reproduce(
            gpcxx::make_tournament_selector( rng , tournament_size )
        ) , reproduction_rate );
    //]
                    
                    
                    
    //[init_population
    std::ofstream fout { vm[ "evolution" ].as< std::string >() };
    for( size_t i=0 ; i<population.size() ; )
    {
        for( size_t j=0 ; j<dynsys::dim ; ++j )
        {
            population[i][j].clear();
            tree_generator( population[i][j] );
        }
        fitness[i] = fitness_f( population[i] , training_data.first , training_data.second );
        if( ! dynsys::is_number( fitness[i] ) )
        {
            continue;
        }
        ++i;
    }
    
    std::cout << "Initial population" << std::endl;
    dynsys::write_best_individuals( std::cout , population , fitness , 10 );
    dynsys::write_best_individuals( fout , population , fitness , 10 , true );
    //]
    
    //[main_loop
    for( size_t i=0 ; i<generation_size ; ++i )
    {
        evolver.next_generation( population , fitness );
        for( size_t i=0 ; i<population.size() ; ++i )
            fitness[i] = fitness_f( population[i] , training_data.first , training_data.second );
        
        std::cout << "Iteration " << i << std::endl;
        dynsys::write_best_individuals( std::cout , population , fitness , 10 );
        dynsys::write_best_individuals( fout , population , fitness , 10 , true );
    }
    //]

    std::ofstream fout2 { vm[ "result" ].as< std::string >() };
    fout2.precision( 14 );
    for( size_t j=0 ; j<dynsys::dim ; ++j )
        fout2 << xstat[j].first << " " << xstat[j].second << " " << ystat[j].first << " " << ystat[j].second << "\n";
    fout2 << std::endl << std::endl;
    dynsys::write_best_individuals( fout2 , population , fitness , 10 , true );
    

    return 0;
}