/*
 * symb_reg_erc.cpp
 * Date: 2015-04-04
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/tree.hpp>
#include <gpcxx/generate.hpp>
#include <gpcxx/operator.hpp>
#include <gpcxx/eval.hpp>
#include <gpcxx/evolve.hpp>
#include <gpcxx/io.hpp>
#include <gpcxx/stat.hpp>
#include <gpcxx/app.hpp>
#include <gpcxx/util.hpp>




#include <iostream>
#include <random>
#include <vector>
#include <functional>

template< typename Value , typename ErcDist >
struct uniform_symbol_erc_generator
{
    using value_type = Value;
    using erc_dist_type = ErcDist;


    uniform_symbol_erc_generator( std::vector< value_type > symbols , double prob_fraction_erc , erc_dist_type erc_dist )
    : m_symbols( std::move( symbols ) ) , m_prob_fraction_erc( prob_fraction_erc ) , m_erc_dist( std::move( erc_dist ) )
    {
        assert( !m_symbols.empty() );
        assert( m_prob_fraction_erc > 0.0 );
    }

    template< typename Rng >
    value_type operator()( Rng &rng )
    {
        std::discrete_distribution< size_t > dist( { 1.0 , m_prob_fraction_erc } );
        if( dist( rng ) == 0 )
        {
            return random_symbol( rng );
        }
        else
        {
            return erc( rng );
        }
    }

    template< typename Rng >
    value_type random_symbol( Rng &rng ) const
    {
        assert( !m_symbols.empty() );
        std::uniform_int_distribution< size_t > dist( 0 , m_symbols.size() - 1 );
        return m_symbols[ dist( rng ) ];
    }
    
    template< typename Rng >
    value_type erc( Rng &rng ) 
    {
        return m_erc_dist( rng );
    }

    size_t num_symbols( void ) const { return m_symbols.size(); }

private:

    std::vector< value_type > m_symbols;
    double m_prob_fraction_erc;
    erc_dist_type m_erc_dist;
};

template< typename Value , typename ErcGen >
auto make_uniform_symbol_erc_generator( std::vector< Value > symbols , double prob_fraction_erc , ErcGen erc_gen )
{
    return uniform_symbol_erc_generator< Value , ErcGen >( std::move( symbols ) , prob_fraction_erc , std::move( erc_gen ) );
}

template< typename Value , typename Dist >
struct erc_generator
{
    using value_type = Value;
    
    erc_generator( Dist const& dist )
    : m_dist( dist ) {}
    
    template< typename Rng >
    value_type operator()( Rng& rng ) const
    {
        auto x = m_dist( rng );
        return value_type { [x]( auto const& c , auto const& n ) { return x; } , std::to_string( x ) };
    }
    
private:
    
    Dist m_dist;
};

template< typename Value , typename Dist >
auto make_erc_generator( Dist const& dist )
{
    return erc_generator< Value , Dist >( dist );
}





int main( int argc , char *argv[] )
{
    //[ create_training_data
    using rng_type = std::mt19937;
    rng_type rng;
    
    gpcxx::regression_training_data< double , 3 > c;
    gpcxx::generate_regression_test_data( c , 1024 , rng , []( double x1 , double x2 , double x3 )
            { return  x1 * x1 * x1 + 1.0 / 10.0 * x2 * x2 - 3.0 / 4.0 * x3 + 1.0 ; } );
    //]
    
    //[ define_tree_types
    using context_type = gpcxx::regression_context< double , 3 >;
    using node_type = gpcxx::intrusive_named_func_node< double , const context_type > ;
    using tree_type = gpcxx::intrusive_tree< node_type >;
    //]
    
    
    
    //[ define_terminal_set
    auto erc_gen = make_erc_generator< node_type >( []( auto& rng ) {
        std::normal_distribution<> dist( 0.0 , 1.0 );
        return dist( rng ); } );
    auto terminal_gen = make_uniform_symbol_erc_generator(
        std::vector< node_type >{
            node_type { gpcxx::array_terminal< 0 >{}                                     ,      "x" } ,
            node_type { gpcxx::array_terminal< 1 >{}                                     ,      "y" } ,
            node_type { gpcxx::array_terminal< 2 >{}                                     ,      "z" } } ,
        0.25 ,
        erc_gen );
    //]

    //[ define_function_set
    auto unary_gen = gpcxx::make_uniform_symbol( std::vector< node_type > {
        node_type { gpcxx::sin_func {}                                               ,      "s" } ,
        node_type { gpcxx::cos_func {}                                               ,      "c" }
    } );

    auto binary_gen = gpcxx::make_uniform_symbol( std::vector< node_type > {
        node_type { gpcxx::plus_func {}                                              ,      "+" } ,
        node_type { gpcxx::minus_func {}                                             ,      "-" } ,
        node_type { gpcxx::multiplies_func {}                                        ,      "*" } ,
        node_type { gpcxx::divides_func {}                                           ,      "/" }
    } );
    //]

    //[ define_node_generator
    auto node_generator = gpcxx::node_generator< node_type , rng_type , 3 > {
        { 1.0 , 0 , terminal_gen } ,
        { 1.0 , 1 , unary_gen } ,
        { 1.0 , 2 , binary_gen } };
    //]

    //[ define_gp_parameters
    size_t population_size = 512;
    size_t generation_size = 50;
    size_t number_elite = 1;
    double mutation_rate = 0.1;
    double crossover_rate = 0.8;
    double reproduction_rate = 0.0;
    size_t min_tree_height = 2 , max_tree_height = 12;
    size_t tournament_size = 15;
    //]

        
    //[ define_population_and_fitness
    using population_type = std::vector< tree_type >;
    using fitness_type = std::vector< double >;
    
    fitness_type fitness( population_size , 0.0 );
    population_type population( population_size );
    //]
    
    //[ define_evolution
    using evolver_type = gpcxx::dynamic_pipeline< population_type , fitness_type , rng_type >;
    evolver_type evolver( rng , number_elite );
    //]

    //[define_evaluator
    using evaluator = struct {
        using context_type = gpcxx::regression_context< double , 3 >;
        using value_type = double;
        value_type operator()( tree_type const& t , context_type const& c ) const {
            return t.root()->eval( c );
        } };
    //]
        
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
    //]


    //[init_population
    for( size_t i=0 ; i<population.size() ; ++i )
    {
        tree_generator( population[i] );
        fitness[i] = fitness_f( population[i] , c );
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
            fitness[i] = fitness_f( population[i] , c );
        
        std::cout << "Iteration " << i << std::endl;
        std::cout << "Best individuals" << std::endl << gpcxx::best_individuals( population , fitness , 1 ) << std::endl;
        std::cout << "Statistics : " << gpcxx::calc_population_statistics( population ) << std::endl << std::endl;
    }
    //]

    return 0;
}
