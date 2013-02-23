/*
 * main_optimize.cpp
 * Date: 2013-01-28
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include "eval.hpp"
#include "fitness.hpp"

#include <gp/ga/ga1.hpp>
#include <gp/tree/linked_node_tree.hpp>
#include <gp/tree/generate_random_linked_tree.hpp>
#include <gp/operator/mutation.hpp>
#include <gp/operator/crossover.hpp>
#include <gp/stat/population_statistics.hpp>

#include <boost/circular_buffer.hpp>
#include <boost/numeric/odeint/stepper/runge_kutta4.hpp>
#include <boost/numeric/odeint/integrate/integrate_const.hpp>
#include <boost/array.hpp>

#include <fstream>
#include <iostream>
#include <random>
#include <vector>

#define tab "\t"

using namespace std;


template< class Rng , class Func >
void generate_test_data(
    fitness_function::vector_type &y ,
    fitness_function::vector_type &x1 ,
    fitness_function::vector_type &x2 ,
    fitness_function::vector_type &x3 , size_t n , Rng &rng , Func f )
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

void generate_lorenz( fitness_function::context_type &c )
{
    using namespace boost::numeric::odeint;

    typedef boost::array< double , 3 > state_type;
    runge_kutta4< state_type > rk4;
    auto lorenz = []( const state_type &x , state_type &dxdt , double t ) {
        const double sigma = 10.0;
        const double R = 28.0;
        const double b = 8.0 / 3.0;
        dxdt[0] = sigma * ( x[1] - x[0] );
        dxdt[1] = R * x[0] - x[1] - x[0] * x[2];
        dxdt[2] = -b * x[2] + x[0] * x[1];
    };

    const double dt = 0.01;
    const size_t emb_dim = 3;
    const size_t emb_tau = 9;
    const size_t N = 10000;

    state_type x = { 10.0 , 10.0 , 10.0 };

    // transients
    integrate_const( rk4 , lorenz , x , 0.0 , 100.0 , dt );
    
    state_type dxdt;
    lorenz( x , dxdt , 0.0 );

    // initially full buffer
    boost::circular_buffer< std::pair< double , double > > buf( ( emb_dim - 1 ) * emb_tau +  1 );
    for( size_t i=0 ; i<buf.size() + 1 ; ++i )
    {
        rk4.do_step( lorenz , x , dxdt , 0.0 , dt );
        lorenz( x , dxdt , 0.0 );
        buf.push_back( std::make_pair( x[0] , dxdt[0] ) );
    }

    c.x1.resize( N );
    c.x2.resize( N );
    c.x3.resize( N );
    c.y.resize( N );

    for( unsigned int i=0 ; i<N ; i++ )
    {
        c.x1[i] = buf[ 0 * emb_dim ].first;
        c.x2[i] = buf[ 1 * emb_dim ].first;
        c.x3[i] = buf[ 2 * emb_dim ].first;
        c.y[i] = buf.back().second;

        rk4.do_step( lorenz , x , dxdt , 0.0 , dt );
        lorenz( x , dxdt , 0.0 );
        buf.push_back( std::make_pair( x[0] , dxdt[0] ) );
    }
}

std::pair< double , double > normalize( fitness_function::vector_type &x )
{
    double mean = 0.0 , sq_mean = 0.0;
    for( auto d : x )
    {
        mean += d;
        sq_mean += d * d;
    }
    mean /= double( x.size() );
    sq_mean /= double( x.size() );
    double stdev = sqrt( sq_mean - mean * mean );
    for( auto &d : x )
    {
        d = ( d - mean ) / stdev;
    }
    return make_pair( mean , stdev );
}


void normalize( fitness_function::vector_type &x1 , fitness_function::vector_type &x2 , fitness_function::vector_type &x3 )
{
    auto p1 = normalize( x1 );
    auto p2 = normalize( x2 );
    auto p3 = normalize( x3 );
    GP_LOG_LEVEL_MODULE( gp::LogLevel::PROGRESS , gp::INIT ) << "Normalize input x1, mean = " << p1.first << ", stdev = " << p1.second;
    GP_LOG_LEVEL_MODULE( gp::LogLevel::PROGRESS , gp::INIT ) << "Normalize input x2, mean = " << p2.first << ", stdev = " << p2.second;
    GP_LOG_LEVEL_MODULE( gp::LogLevel::PROGRESS , gp::INIT ) << "Normalize input x3, mean = " << p3.first << ", stdev = " << p3.second;
}

std::vector< boost::shared_ptr< std::ostream > > streams;

void init_logging( void )
{
    using namespace Amboss::Log;
    LoggerCollection &logger = GlobalLogger::getInstance();
    logger.data().clear();
    
    auto filter = []( const LogEntry &e ) { return ( e.logLevel >= NOISE ); };
    std::shared_ptr< OStreamLogger > l( std::make_shared< OStreamLogger >( std::cerr , gp::DefaultFormatter() , filter ) );
    logger.data().push_back( std::shared_ptr< ILogger >( l ) );

    boost::shared_ptr< std::ostream > s = boost::make_shared< std::ofstream >( "log.dat" );
    streams.push_back( s );
    std::shared_ptr< OStreamLogger > ll = std::make_shared< OStreamLogger >( *s , gp::DefaultFormatter() , filter );
    logger.data().push_back( std::shared_ptr< ILogger >( ll ) );
}



namespace pl = std::placeholders;

int main( int argc , char *argv[] )
{
    init_logging();

    typedef std::mt19937 rng_type ;
    typedef fitness_function::context_type context_type;
    typedef gp::genetic_evolver1< tree_type , fitness_function::context_type , std::mt19937 > evolver_type;





    rng_type rng;

    context_type c;
    // generate_test_data( c.y , c.x1 , c.x2 , c.x3 , 10000 , rng ,
    //                     []( double x1 , double x2 , double x3 ) { return x1 + x2 - x3; } );
    // generate_test_data( c.y , c.x1 , c.x2 , c.x3 , 1000 , rng ,
    //                     []( double x1 , double x2 , double x3 ) { return x1 + x2 - 0.3 * x3; } );
    generate_lorenz( c );
    normalize( c.x1 , c.x2 , c.x3 );

    generators< rng_type > gen( rng );

    size_t population_size = 200;
    double elite_rate = double( 2 ) / double( population_size );
    double mutation_rate = 0.2;
    double crossover_rate = 0.6;
    size_t min_tree_height = 2 , max_tree_height = 6;

    std::function< void( tree_type& ) > tree_generator;
    tree_generator = make_tree_generator_binder( rng , gen.gen0 , gen.gen1 , gen.gen2 , min_tree_height , max_tree_height );

    evolver_type evolver( elite_rate , mutation_rate , crossover_rate , rng );
    std::vector< double > fitness( population_size , 0.0 );
    std::vector< tree_type > population( population_size );


    evolver.fitness_function() = fitness_function();
    evolver.mutation_function() = gp::make_mutation_binder( rng , gen.gen0 , gen.gen1 , gen.gen2 );
    evolver.crossover_function() = gp::make_crossover_binder( rng , max_tree_height );
    evolver.random_individual_function() = tree_generator;


    // initialize population with random trees and evaluate fitness
    GP_LOG_LEVEL_MODULE( gp::LogLevel::PROGRESS , gp::MAIN ) << "Starting Initialization!";
    for( size_t i=0 ; i<population.size() ; ++i )
    {
        // if( i == 0 )
        // {
        //     population[0].set_data(
        //         new node_type( '+' ,
        //                        new node_type( '*' , new node_type( 1.23 ) , new node_type( 'x' ) ) ,
        //                        new node_type( '+'  ,
        //                                       new node_type( '*' , new node_type( 1.44 ) , new node_type( 'y' )  ) ,
        //                                       new node_type( '*' , new node_type( 1.66 ) , new node_type( 'z' )  )
        //                            )
        //             )
        //         );

        //     fitness_function::node_vector nodes;
        //     fitness_function::find_double_terminals( population[i] , nodes );

        //     print_simple( population[i] , cout );
        //     cout << endl;
        //     for( auto n : nodes ) cout << n << " " << n->value << endl;
        //     fitness[i] = fitness_function()( population[i] , c );
        //     return -1;
        // }

        tree_generator( population[i] );
        fitness[i] = fitness_function()( population[i] , c );
    }
    GP_LOG_LEVEL_MODULE( gp::LogLevel::PROGRESS , gp::MAIN ) << "Finishing Initialization!";
    
    for( size_t i=0 ; i<200 ; ++i )
    {
        GP_LOG_LEVEL_MODULE( gp::LogLevel::PROGRESS , gp::MAIN ) << "Starting Iteration " << i << "!";

        evolver.next_generation( population , fitness , c );

        std::vector< size_t > idx;
        auto iter = gp::sort_indices( fitness , idx );
        for( size_t i=0 ; i<10 ; ++i )
            GP_LOG_LEVEL_MODULE( gp::LogLevel::PROGRESS , gp::MAIN )
                << "Individual " << i << " " << gp::simple( population[ idx[i] ] ) << " : " << fitness[ idx[i] ];
        GP_LOG_LEVEL_MODULE( gp::LogLevel::PROGRESS , gp::MAIN ) << "Finishing Iteration " << i << "!";
    }

    return 0;
}
