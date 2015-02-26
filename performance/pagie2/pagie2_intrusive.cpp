/*
 * main_optimize.cpp
 * Date: 2013-01-28
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#define FUSION_MAX_VECTOR_SIZE 20

#include <gpcxx/tree/intrusive_tree.hpp>
#include <gpcxx/tree/intrusive_named_func_node.hpp>
#include <gpcxx/tree/intrusive_func.hpp>

#include <gpcxx/generate/uniform_symbol.hpp>
#include <gpcxx/generate/node_generator.hpp>
#include <gpcxx/generate/basic_generate_strategy.hpp>
#include <gpcxx/generate/ramp.hpp>
#include <gpcxx/operator/mutation.hpp>
#include <gpcxx/operator/simple_mutation_strategy.hpp>
#include <gpcxx/operator/random_selector.hpp>
#include <gpcxx/operator/tournament_selector.hpp>
#include <gpcxx/operator/crossover.hpp>
#include <gpcxx/operator/one_point_crossover_strategy.hpp>
#include <gpcxx/operator/reproduce.hpp>
#include <gpcxx/eval/static_eval.hpp>
#include <gpcxx/eval/regression_fitness.hpp>
#include <gpcxx/evolve/static_pipeline.hpp>
#include <gpcxx/io/best_individuals.hpp>
#include <gpcxx/stat/population_statistics.hpp>
#include <gpcxx/app/timer.hpp>
#include <gpcxx/app/normalize.hpp>


#include <map>
#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <functional>
#include <fstream>

const std::string tab = "\t";



typedef double value_type;
typedef gpcxx::regression_training_data< value_type , 3 > trainings_data_type;
typedef std::mt19937 rng_type ;
typedef std::array< value_type , 3 > eval_context_type;
typedef std::vector< value_type > fitness_type;
typedef gpcxx::intrusive_named_func_node< double , eval_context_type const > node_type;
typedef gpcxx::intrusive_tree< node_type > tree_type;
typedef std::vector< tree_type > population_type;


struct evaluator
{
    typedef eval_context_type context_type;
    typedef double value_type;

    value_type operator()( tree_type const& t , context_type const& c ) const
    {
        return t.root()->eval( c );
    }
};

template< typename F >
void generate_test_data( trainings_data_type &data, double rmin , double rmax , double stepsize , F f )
{
    data.x[0].clear(); data.x[1].clear(); data.x[2].clear(); data.y.clear();
    
    for( double xx = rmin ; xx <= rmax ; xx += stepsize )
    {
        for( double yy = rmin ; yy <= rmax ; yy += stepsize )
        {
            for( double zz = rmin ; zz <= rmax ; zz += stepsize )
            {
                data.x[0].push_back( xx );
                data.x[1].push_back( yy );
                data.x[2].push_back( zz );
                data.y.push_back( f( xx , yy , zz ) );
            }
        }
    }
}





template< typename Pop >
void write_height_hist( Pop const& p , std::ostream &out )
{
    std::map< size_t , size_t > h;
    for( auto const& t : p )
    {
        h[ t.root().height() ] ++;
    }
    for( auto const &e : h )
        out << e.first << " " << e.second << "\n";
}

template< typename Pop >
void write_height_hist( Pop const& p , std::string const &filename )
{
    std::ofstream fout( filename );
    write_height_hist( p , fout );
}


template< typename Pop >
void write_size_hist( Pop const& p , std::ostream &out )
{
    std::map< size_t , size_t > h;
    for( auto const& t : p )
    {
        h[ t.size() ] ++;
    }
    for( auto const &e : h )
        out << e.first << " " << e.second << "\n";
}

template< typename Pop >
void write_size_hist( Pop const& p , std::string const &filename )
{
    std::ofstream fout( filename );
    write_size_hist( p , fout );
}



namespace pl = std::placeholders;

int main( int argc , char *argv[] )
{
    rng_type rng;
    
    trainings_data_type c;
    generate_test_data( c , -5.0 , 5.0 + 0.1 , 0.4 , []( double x1 , double x2 , double x3 ) {
                        return  1.0 / ( 1.0 + pow( x1 , -4.0 ) ) + 1.0 / ( 1.0 + pow( x2 , -4.0 ) ) + 1.0 / ( 1.0 + pow( x3 , -4.0 ) ); } );
    gpcxx::normalize( c.y );
    

    std::ofstream fout1( "testdata.dat" );
    for( size_t i=0 ; i<c.x[0].size() ; ++i )
        fout1 << c.y[i] << " " << c.x[0][i] << " " << c.x[1][i] << " " << c.x[2][i] << "\n";
    fout1.close();
    

	gpcxx::uniform_symbol< node_type > terminal_gen { std::vector< node_type >{
        node_type { gpcxx::array_terminal< 0 >{} , "x" } ,
        node_type { gpcxx::array_terminal< 1 >{} , "y" } ,
        node_type { gpcxx::array_terminal< 2 >{} , "z" } } };
    gpcxx::uniform_symbol< node_type > unary_gen { std::vector< node_type >{
        node_type { gpcxx::sin_func{} , "s" } ,
        node_type { gpcxx::cos_func{} , "c" } ,
        node_type { gpcxx::exp_func{} , "e" } ,
        node_type { gpcxx::log_func{} , "l" } } };
    gpcxx::uniform_symbol< node_type > binary_gen { std::vector< node_type > {
        node_type { gpcxx::plus_func{} , "+" } ,
        node_type { gpcxx::minus_func{} , "-" } ,
        node_type { gpcxx::multiplies_func{} , "*" } ,
        node_type { gpcxx::divides_func{} , "/" }    
    } };
    
    gpcxx::node_generator< node_type , rng_type , 3 > node_generator {
        { double ( terminal_gen.num_symbols() ) , 0 , terminal_gen } ,
        { double ( unary_gen.num_symbols() ) , 1 , unary_gen } ,
        { double ( binary_gen.num_symbols() ) , 2 , binary_gen } };
    
    size_t population_size = 512;
    size_t generation_size = 20;
    double number_elite = 1;
    double mutation_rate = 0.0;
    double crossover_rate = 0.6;
    double reproduction_rate = 0.3;
    size_t max_tree_height = 8;
    size_t tournament_size = 15;
    

    auto tree_generator = gpcxx::make_basic_generate_strategy( rng , node_generator , max_tree_height , max_tree_height );
    // size_t min_tree_height = 1
    // auto new_tree_generator = gpcxx::make_ramp( rng , node_generator , min_tree_height , max_tree_height , 0.5 );


    typedef gpcxx::static_pipeline< population_type , fitness_type , rng_type > evolver_type;
    evolver_type evolver( number_elite , mutation_rate , crossover_rate , reproduction_rate , rng );
    std::vector< double > fitness( population_size , 0.0 );
    std::vector< tree_type > population( population_size );


    evolver.mutation_function() = gpcxx::make_mutation(
        gpcxx::make_simple_mutation_strategy( rng , node_generator ) ,
        gpcxx::make_tournament_selector( rng , tournament_size ) );
    evolver.crossover_function() = gpcxx::make_crossover( 
        gpcxx::make_one_point_crossover_strategy( rng , max_tree_height ) ,
        gpcxx::make_tournament_selector( rng , tournament_size ) );
    evolver.reproduction_function() = gpcxx::make_reproduce(
        gpcxx::make_tournament_selector( rng , tournament_size ) );
    
    gpcxx::timer timer;
    auto fitness_f = gpcxx::make_regression_fitness( evaluator() );

    // initialize population with random trees and evaluate fitness
    timer.restart();
    for( size_t i=0 ; i<population.size() ; ++i )
    {
        tree_generator( population[i] );
        fitness[i] = fitness_f( population[i] , c );
    }
    std::cout << gpcxx::indent( 0 ) << "Generation time " << timer.seconds() << std::endl;
    std::cout << gpcxx::indent( 1 ) << "Best individuals" << std::endl << gpcxx::best_individuals( population , fitness , 1 , 10 ) << std::endl;
    std::cout << gpcxx::indent( 1 ) << "Statistics : " << gpcxx::calc_population_statistics( population ) << std::endl;
    std::cout << gpcxx::indent( 1 ) << std::endl << std::endl;
    
    write_height_hist( population , "initial_height.hist" );
    write_size_hist( population , "initial_size.hist" );
    
    timer.restart();
    for( size_t generation=1 ; generation<=generation_size ; ++generation )
    {
        gpcxx::timer iteration_timer;
        iteration_timer.restart();
        evolver.next_generation( population , fitness );
        double evolve_time = iteration_timer.seconds();
        iteration_timer.restart();
        std::transform( population.begin() , population.end() , fitness.begin() , [&]( tree_type const &t ) { return fitness_f( t , c ); } );
        double eval_time = iteration_timer.seconds();
        
        write_height_hist( population , "height_" + std::to_string( generation ) + ".hist" );
        write_size_hist( population , "size_" + std::to_string( generation ) + ".hist" );
        
        std::cout << gpcxx::indent( 0 ) << "Generation " << generation << std::endl;
        std::cout << gpcxx::indent( 1 ) << "Evolve time " << evolve_time << std::endl;
        std::cout << gpcxx::indent( 1 ) << "Eval time " << eval_time << std::endl;
        std::cout << gpcxx::indent( 1 ) << "Best individuals" << std::endl << gpcxx::best_individuals( population , fitness , 2 , 10 ) << std::endl;
        std::cout << gpcxx::indent( 1 ) << "Statistics : " << gpcxx::calc_population_statistics( population ) << std::endl << std::endl;
    }
    std::cout << "Overall time : " << timer.seconds() << std::endl;


// 	generate_data();
// 	init_constants();
// 	init_node_types();   // init generators
// 	init_population();
// 	evolve();
// 	report();

    return 0;
}
