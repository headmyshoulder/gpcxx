/*
 * main_optimize.cpp
 * Date: 2013-01-28
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#define FUSION_MAX_VECTOR_SIZE 20

#include <gpcxx/tree/basic_tree.hpp>
#include <gpcxx/generate/uniform_symbol.hpp>
#include <gpcxx/generate/ramp.hpp>
#include <gpcxx/operator/mutation.hpp>
#include <gpcxx/operator/simple_mutation_strategy.hpp>
#include <gpcxx/operator/random_selector.hpp>
#include <gpcxx/operator/tournament_selector.hpp>
#include <gpcxx/operator/crossover.hpp>
#include <gpcxx/operator/one_point_crossover_strategy.hpp>
#include <gpcxx/operator/reproduce.hpp>
#include <gpcxx/eval/static_eval_erc.hpp>
#include <gpcxx/evolve/static_pipeline.hpp>
#include <gpcxx/stat/best_individuals.hpp>
#include <gpcxx/stat/population_statistics.hpp>

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
    value_type operator()( Tree const & t , const context_type &c ) const
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
    typedef char symbol_type;
    typedef std::array< value_type , 3 > eval_context_type;
    typedef std::vector< value_type > fitness_type;
    

    rng_type rng;

    context_type c;
    generate_test_data3( c.y , c.x1 , c.x2 , c.x3 , 1024 , rng ,
                         []( double x1 , double x2 , double x3 ) { return  x1 * x1 * x1 + 1.0 / 10.0 * x2 * x2 - 3.0 / 4.0 * ( x3 - 4.0 ) + 1.0 ; } );
    
    auto eval = gpcxx::make_static_eval_erc< value_type , symbol_type , eval_context_type >(
        fusion::make_vector( 1.0 , std::normal_distribution<>( 0.0 , 1.0 ) ) ,
        fusion::make_vector(
            fusion::make_vector( 'x' , []( eval_context_type const& t ) { return t[0]; } )
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
    typedef eval_type::node_attribute_type node_attribute_type;
    
    typedef gpcxx::basic_tree< node_attribute_type > tree_type;
    typedef std::vector< tree_type > population_type;
    typedef gpcxx::static_pipeline< population_type , fitness_type , rng_type > evolver_type;

    
    size_t population_size = 512;
    double elite_rate = double( 1 ) / double( population_size );
    double mutation_rate = 0.2;
    double crossover_rate = 0.6;
    double reproduction_rate = 0.3;
    size_t min_tree_height = 8 , max_tree_height = 8;



    // generators< rng_type > gen( rng );
    auto terminal_gen = eval.get_terminal_symbol_distribution();
    auto unary_gen = eval.get_unary_symbol_distribution();
    auto binary_gen = eval.get_binary_symbol_distribution();
    std::array< int , 3 > weights = {{ 2 * int( terminal_gen.num_symbols() ) ,
                                       int( unary_gen.num_symbols() ) ,
                                       int( binary_gen.num_symbols() ) }};
    auto tree_generator = gpcxx::make_ramp( rng , terminal_gen , unary_gen , binary_gen , min_tree_height , max_tree_height , 0.5 , weights );
    

    evolver_type evolver( elite_rate , mutation_rate , crossover_rate , reproduction_rate , rng );
    std::vector< double > fitness( population_size , 0.0 );
    std::vector< tree_type > population( population_size );


    auto fitness_f = fitness_function< eval_type >( eval );
    evolver.mutation_function() = gpcxx::make_mutation(
        gpcxx::make_simple_mutation_strategy( rng , terminal_gen , unary_gen , binary_gen ) ,
        gpcxx::make_random_selector( rng ) );
    evolver.crossover_function() = gpcxx::make_crossover( 
        gpcxx::make_one_point_crossover_strategy( rng , 10 ) ,
        gpcxx::make_random_selector( rng ) );
    evolver.reproduction_function() = gpcxx::make_reproduce( gpcxx::make_random_selector( rng ) );


    // initialize population with random trees and evaluate fitness
    for( size_t i=0 ; i<population.size() ; ++i )
    {
        tree_generator( population[i] );
        fitness[i] = fitness_function< eval_type >( eval )( population[i] , c );
    }
    
    std::cout << "Best individuals" << std::endl << gpcxx::best_individuals( population , fitness ) << std::endl;
    std::cout << "Statistics : " << gpcxx::calc_population_statistics( population ) << std::endl;
    std::cout << std::endl << std::endl;

    for( size_t i=0 ; i<100 ; ++i )
    {
        evolver.next_generation( population , fitness );
        for( size_t i=0 ; i<population.size() ; ++i )
            fitness[i] = fitness_f( population[i] , c );
        
        std::cout << "Iteration " << i << std::endl;
        std::cout << "Best individuals" << std::endl << gpcxx::best_individuals( population , fitness , 1 ) << std::endl;
        std::cout << "Statistics : " << gpcxx::calc_population_statistics( population ) << std::endl << std::endl;
    }

    return 0;
}
