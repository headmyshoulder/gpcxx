/*
 * main_optimize.cpp
 * Date: 2013-01-28
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

// #include "eval.hpp"

#define FUSION_MAX_VECTOR_SIZE 20

#include <gp/tree/basic_tree.hpp>
#include <gp/generate/random_symbol_generator.hpp>
#include <gp/generate/ramp.hpp>
#include <gp/operator/mutation.hpp>
#include <gp/operator/simple_mutation_strategy.hpp>
#include <gp/operator/random_selector.hpp>
#include <gp/operator/tournament_selector.hpp>
#include <gp/operator/crossover.hpp>
#include <gp/operator/one_point_crossover_strategy.hpp>
#include <gp/operator/reproduce.hpp>

#include <gp/eval/basic_eval.hpp>
#include <gp/evolve/static_pipeline.hpp>

#include <gp/stat/best_individuals.hpp>
#include <gp/stat/population_statistics.hpp>

#include "../common/generate_test_data.hpp"
#include "../common/statistics.hpp"

#include <boost/fusion/include/make_vector.hpp>

#include <iostream>
#include <random>
#include <vector>
#include <functional>

#define tab "\t"

namespace fusion = boost::fusion;

typedef double value_type;
typedef std::vector< value_type > vector_type;

struct context_type
{
    vector_type x1 , x2 , x3 , y;
};


template< typename Eval >
struct fitness_function
{
    Eval m_eval;
    fitness_function( Eval eval ) : m_eval( eval ) { }

    template< typename Tree >
    double operator()( Tree const & t , const context_type &c ) const
    {
        double chi2 = 0.0;
        for( size_t i=0 ; i<c.x1.size() ; ++i )
        {
            typename Eval::eval_context_type cc;
            cc[0] = c.x1[i];
            cc[1] = c.x2[i];
            cc[2] = c.x3[i];
            double yy = m_eval( t , cc );
            chi2 += ( yy - c.y[i] ) * ( yy - c.y[i] );
        }
        chi2 /= double( c.x1.size() );
        return - 1.0 / ( 1.0 + chi2 );
    }
};





namespace pl = std::placeholders;

int main( int argc , char *argv[] )
{
    typedef std::mt19937 rng_type ;
    typedef char attribute_type;
    typedef gp::basic_tree< attribute_type > tree_type;
    typedef std::array< value_type , 3 > eval_context_type;
    typedef std::vector< tree_type > population_type;
    typedef std::vector< value_type > fitness_type;
    typedef gp::static_pipeline< population_type , fitness_type , rng_type > evolver_type;

    rng_type rng;

    context_type c;
    generate_test_data3( c.y , c.x1 , c.x2 , c.x3 , 1024 , rng ,
                         []( double x1 , double x2 , double x3 ) { return  x1 * x1 * x1 + 1.0 / 10.0 * x2 * x2 - 3.0 / 4.0 * ( x3 - 4.0 ) + 1.0 ; } );
    
    auto eval = gp::make_basic_eval< value_type , eval_context_type , attribute_type >(
        fusion::make_vector(
            fusion::make_vector( '1' , []( eval_context_type const& t ) { return 1.0; } )
          , fusion::make_vector( '2' , []( eval_context_type const& t ) { return 2.0; } )
          , fusion::make_vector( '3' , []( eval_context_type const& t ) { return 2.0; } )
          , fusion::make_vector( '4' , []( eval_context_type const& t ) { return 2.0; } )
          , fusion::make_vector( '5' , []( eval_context_type const& t ) { return 2.0; } )
          , fusion::make_vector( '6' , []( eval_context_type const& t ) { return 2.0; } )
          , fusion::make_vector( '7' , []( eval_context_type const& t ) { return 2.0; } )
          , fusion::make_vector( '8' , []( eval_context_type const& t ) { return 2.0; } )
          , fusion::make_vector( '9' , []( eval_context_type const& t ) { return 2.0; } )
          , fusion::make_vector( 'x' , []( eval_context_type const& t ) { return t[0]; } )
          , fusion::make_vector( 'y' , []( eval_context_type const& t ) { return t[1]; } )
          , fusion::make_vector( 'z' , []( eval_context_type const& t ) { return t[2]; } )          
          ) ,
        fusion::make_vector(
            fusion::make_vector( 's' , []( double v ) -> double { return std::sin( v ); } )
          , fusion::make_vector( 'c' , []( double v ) -> double { return std::cos( v ); } ) 
          ) ,
        fusion::make_vector(
            fusion::make_vector( '+' , std::plus< double >() )
          , fusion::make_vector( '-' , std::minus< double >() )
          , fusion::make_vector( '*' , std::multiplies< double >() ) 
          , fusion::make_vector( '/' , std::divides< double >() ) 
          ) );
    typedef decltype( eval ) eval_type;
    
    size_t population_size = 100;
    double elite_rate = double( 1 ) / double( population_size );
    double mutation_rate = 0.2;
    double crossover_rate = 0.6;
    double reproduction_rate = 0.3;
    size_t min_tree_height = 8 , max_tree_height = 8;



    // generators< rng_type > gen( rng );
    auto terminal_gen = gp::make_random_symbol_generator( eval.get_terminal_symbols() , rng );
    auto unary_gen = gp::make_random_symbol_generator( eval.get_unary_symbols() , rng );
    auto binary_gen = gp::make_random_symbol_generator( eval.get_binary_symbols() , rng );
    std::array< int , 3 > weights = {{ 2 * int( terminal_gen.num_symbols() ) ,
                                       int( unary_gen.num_symbols() ) ,
                                       int( binary_gen.num_symbols() ) }};
    auto tree_generator = gp::make_ramp( rng , terminal_gen , unary_gen , binary_gen , min_tree_height , max_tree_height , 0.5 , weights );
    

    evolver_type evolver( elite_rate , mutation_rate , crossover_rate , reproduction_rate , rng );
    std::vector< double > fitness( population_size , 0.0 );
    std::vector< tree_type > population( population_size );


    auto fitness_f = fitness_function< eval_type >( eval );
    evolver.mutation_function() = gp::make_mutation(
        gp::make_simple_mutation_strategy( rng , terminal_gen , unary_gen , binary_gen ) ,
        gp::make_random_selector( rng ) );
    evolver.crossover_function() = gp::make_crossover( 
        gp::make_one_point_crossover_strategy( rng , 10 ) ,
        gp::make_random_selector( rng ) );
    evolver.reproduction_function() = gp::make_reproduce( gp::make_random_selector( rng ) );


    // initialize population with random trees and evaluate fitness
    for( size_t i=0 ; i<population.size() ; ++i )
    {
        tree_generator( population[i] );
        fitness[i] = fitness_function< eval_type >( eval )( population[i] , c );
    }
    std::cout << "Best individuals" << std::endl << gp::best_individuals( population , fitness ) << std::endl;
    std::cout << "Statistics" << std::endl << gp::population_statistics( population , fitness ) << std::endl;
    std::cout << std::endl << std::endl;

    for( size_t i=0 ; i<100 ; ++i )
    {
        evolver.next_generation( population , fitness );
        for( size_t i=0 ; i<population.size() ; ++i )
            fitness[i] = fitness_f( population[i] , c );
        
        std::cout << "Iteration " << i << std::endl;
        std::cout << "Best individuals" << std::endl << gp::best_individuals( population , fitness , 1 ) << std::endl;
        std::cout << "Statistics" << tab << gp::population_statistics( population , fitness ) << std::endl << std::endl;
    }

    return 0;
}
