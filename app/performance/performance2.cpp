/*
 * main_optimize.cpp
 * Date: 2013-01-28
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include "eval.hpp"

#include <gp/ga/ga2.hpp>
#include <gp/tree/linked_node.hpp>
#include <gp/tree/generate_random_linked_tree.hpp>
#include <gp/operator/mutation.hpp>
#include <gp/operator/one_point_crossover_strategy.hpp>
#include <gp/operator/crossover.hpp>
#include <gp/operator/reproduce.hpp>
#include <gp/operator/random_selector.hpp>
#include <gp/operator/tournament_selector.hpp>
#include <gp/stat/best_individuals.hpp>
#include <gp/stat/population_statistics.hpp>
#include <gp/util/timer.hpp>

#include <boost/make_shared.hpp>
#include <boost/concept_check.hpp>

#include <fstream>
#include <iostream>
#include <random>
#include <vector>
#include <chrono>

#define tab "\t"

using namespace std;


template< class Rng , class Func >
void generate_test_data( vector_t &y , vector_t &x1 , vector_t &x2 , vector_t &x3 , size_t n , Rng &rng , Func f )
{
    std::normal_distribution< double > dist1( 0.0 , 1.0 ) , dist2( 0.0 , 1.0 ) , dist3( 0.0 , 1.0 );
    x1.resize( n ) ; x2.resize( n ) ; x3.resize( n ) ; y.resize( n ) ;
    for( size_t i = 0 ; i < n ; ++i )
    {
        x1[i] = dist1( rng );
        x2[i] = dist2( rng );
        x3[i] = dist3( rng );
        y[i] = f( x1[i] , x2[i] , x3[i] );
    }
}




int main( int argc , char *argv[] )
{
    typedef std::mt19937 rng_type ;
    typedef fitness_function::context_type context_type;
    typedef std::vector< node_type > population_type;
    typedef std::vector< double > fitness_type;
    typedef gp::genetic_evolver2< population_type , fitness_type , fitness_function::context_type , std::mt19937 > evolver_type;
    
    
    size_t population_size = 1024;
    size_t data_size = 1024 * 16;
    double elite_rate = 0.0;
    double mutation_rate = 0.0;
    double crossover_rate = 0.9;
    double reproduction_rate = 0.1;
    size_t min_tree_height = 3 , max_tree_height = 6;
    size_t tournament_size = 7;



    gp::timer timer;
    rng_type rng;
    context_type c;
    generate_test_data( c.y , c.x1 , c.x2 , c.x3 , data_size , rng , []( double x1 , double x2 , double x3 ) {
                            return x1 * x1 * x1 + 1.0 / 10.0 * x2 * x2 - 3.0 / 4.0 * ( x3 - 4.0 ) + 1.0 ; } );
    generators< rng_type > gen( rng );

    std::array< int , 3 > weights = {{ int( gen.gen0.num_symbols() ) ,
                                       int( gen.gen1.num_symbols() ) , 
                                       int( gen.gen2.num_symbols() ) }};
    std::function< void( node_type& ) > tree_generator = make_tree_generator_binder( rng , gen.gen0 , gen.gen1 , gen.gen2 , min_tree_height , max_tree_height , weights );

    evolver_type evolver( elite_rate , mutation_rate , crossover_rate , reproduction_rate , rng );
    fitness_type fitness( population_size , 0.0 );
    population_type population( population_size );


    evolver.fitness_function() = fitness_function();
    evolver.crossover_function() = gp::make_crossover( gp::make_one_point_crossover_strategy( rng , max_tree_height ) , gp::make_tournament_selector( rng , tournament_size ) );
    evolver.reproduction_function() = gp::make_reproduce( gp::make_random_selector( rng ) );


    // initialize population with random trees and evaluate fitness
    timer.restart();
    for( size_t i=0 ; i<population.size() ; ++i )
    {
        tree_generator( population[i] );
        fitness[i] = fitness_function()( population[i] , c );
    }
    cout << "Finished initial creation in " << timer.seconds() << " seconds" << endl;
    cout << "Best individuals" << endl << best_individuals( population , fitness , 1 ) << endl;
    cout << "Statistics " << population_statistics( population , fitness ) << endl << endl;
    

    for( size_t i=0 ; i<50 ; ++i )
    {
        timer.restart();
        evolver.next_generation( population , fitness , c );
        cout << "Finished iteration " << i << " in " << timer.seconds() << " seconds" << endl;
        cout << "Best individuals" << endl << best_individuals( population , fitness , 1 ) << endl;
        cout << "Statistics " << population_statistics( population , fitness ) << endl << endl;
    }

    return 0;
}
