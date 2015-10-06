/*
 * symb_reg_simplify.cpp
 * Date: 2015-09-30
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/tree.hpp>
#include <gpcxx/primitive_sets.hpp>
#include <gpcxx/canonic.hpp>
#include <gpcxx/generate.hpp>
#include <gpcxx/operator.hpp>
#include <gpcxx/eval.hpp>
#include <gpcxx/evolve.hpp>
#include <gpcxx/io.hpp>
#include <gpcxx/stat.hpp>
// #include <gpcxx/app.hpp>

#include <gpcxx/benchmark_problems.hpp>

#include <fstream>
#include <iostream>
#include <random>
#include <vector>
#include <functional>


int main( int argc , char** argv )
{
    using rng_type = std::mt19937;
//     std::random_device rd;
//     rng_type rng { rd() };
    rng_type rng {};

    // auto problem = generate_koza1( rng );  // koza1 - koza3 
    // auto problem = generate_nguyen9( rng );
    auto problem = gpcxx::generate_pagie1();
    using problem_type = decltype( problem );
    
    // define_tree_types
    static size_t const dim = problem_type::dim;
    using context_type = gpcxx::regression_context< double , dim >;
    using node_type = gpcxx::algebraic_node< double , context_type const > ;
    using tree_type = gpcxx::intrusive_tree< node_type >;

    // define algebra types for simplification of trees
    using rule = std::function< gpcxx::rule_result( tree_type& , tree_type::cursor ) >;
    using rule_container = std::vector< rule >;

    // define rules
    auto rule1 = gpcxx::make_summarize_constant( []( double t ) {
        return node_type::make_constant_terminal( gpcxx::double_terminal< double >( t ) , std::to_string( t ) ); } );
    rule_container rules { rule1 };

    // gpcxx::transform_tree( rules , t );


    // define node types
    auto node_generator = gpcxx::koza_algebraic_primitve_set< node_type , rng_type , dim , true >();
        
    // define_gp_parameters
    size_t population_size = 4000;
    size_t generation_size = 50;
    size_t number_elite = 1;
    double mutation_rate = 0.2;
    double crossover_rate = 0.6;
    double reproduction_rate = 0.3;
    size_t min_tree_height = 4 , max_tree_height = 12;
    size_t tournament_size = 15;
    size_t number_output_best_individuals = 10;
        
    // define_population_and_fitness
    using population_type = std::vector< tree_type >;
    using fitness_type = std::vector< double >;
    
    fitness_type fitness( population_size , 0.0 );
    population_type population( population_size );

    // define_evolution
    using evolver_type = gpcxx::dynamic_pipeline< population_type , fitness_type , rng_type >;
    evolver_type evolver( rng , number_elite , [rules]( auto& t ) { gpcxx::transform_tree( rules , t ); } );
    // evolver_type evolver( rng , number_elite );
    

    // define_evaluator
    using evaluator = struct {
        using context_type = gpcxx::regression_context< double , 2 >;
        using value_type = double;
        value_type operator()( tree_type const& t , context_type const& c ) const {
            return t.root()->eval( c );
        } };
        
    //[define_genetic_operators
    auto tree_generator = gpcxx::make_ramp( rng , node_generator , min_tree_height , max_tree_height , 0.5 );
    auto fitness_f = gpcxx::make_regression_fitness( evaluator {} );
    evolver.add_operator( gpcxx::make_mutation(
            gpcxx::make_point_mutation( rng , tree_generator , max_tree_height , 20 ) ,
            gpcxx::make_tournament_selector( rng , tournament_size ) )
        , mutation_rate );
    evolver.add_operator( gpcxx::make_crossover( 
            gpcxx::make_one_point_crossover_strategy( rng , 10 ) ,
            gpcxx::make_tournament_selector( rng , tournament_size ) )
        , crossover_rate );
    evolver.add_operator( gpcxx::make_reproduce(
            gpcxx::make_tournament_selector( rng , tournament_size ) )
        , reproduction_rate );
    

    std::ofstream fout { "koza_evolution.json" };

    // init_population
    for( size_t i=0 ; i<population.size() ; ++i )
    {
        tree_generator( population[i] );
        fitness[i] = fitness_f( population[i] , problem );
    }
    
    fout << "[" << gpcxx::population_json( population , fitness , 1 , "\n" , false );

    std::cout << "Best individuals" << std::endl << gpcxx::best_individuals( population , fitness , 0 , number_output_best_individuals ) << std::endl;
    std::cout << "Statistics : " << gpcxx::calc_population_statistics( population ) << std::endl;
    std::cout << std::endl << std::endl;

    // main_loop
    for( size_t i=0 ; i<generation_size ; ++i )
    {
        evolver.next_generation( population , fitness );
        for( size_t i=0 ; i<population.size() ; ++i )
            fitness[i] = fitness_f( population[i] , problem );
        
        std::cout << "Iteration " << i << std::endl;
        std::cout << "Best individuals" << std::endl << gpcxx::best_individuals( population , fitness , 1 , number_output_best_individuals ) << std::endl;
        std::cout << "Statistics : " << gpcxx::calc_population_statistics( population ) << std::endl << std::endl;
        
        fout << " , " << "\n" << gpcxx::population_json( population , fitness , 1 , "\n" , false );
    }
    fout << "]" << "\n";

    return 0;
}

