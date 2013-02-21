/*
 * main_optimize.cpp
 * Date: 2013-01-28
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include <gp/ga/ga1.hpp>
#include <gp/tree/linked_node_tree.hpp>

#include <iostream>
#include <random>
#include <vector>

#define tab "\t"

using namespace std;

typedef std::vector< double > vector_t;



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

std::pair< double , double > normalize( vector_t &x )
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


void normalize( vector_t &x1 , vector_t &x2 , vector_t &x3 )
{
    auto p1 = normalize( x1 );
    auto p2 = normalize( x2 );
    auto p3 = normalize( x3 );
    cout << p1.first << " " << p1.second << endl;
    cout << p2.first << " " << p2.second << endl;
    cout << p3.first << " " << p3.second << endl;
}


struct context
{
    vector_t x1 , x2 , x3 , y;
};

typedef gp::linked_node_tree< char > tree_type;



int main( int argc , char *argv[] )
{
    std::mt19937 rng;

    context c;
    generate_test_data( c.y , c.x1 , c.x2 , c.x3 , 10000 , rng ,
                        []( double x1 , double x2 , double x3 ) { return x1 + x2 - x3; } );
//    generate_test_data( y , x1 , x2 , x3 , 10000 , rng , []( double x1 , double x2 , double x3 ) { return x1 + x2 - 0.3 * x3; } );
//    normalize( x1 , x2 , x3 );

    
    size_t population_size = 200;
    double elite_rate = 0.001;
    double mutation_rate = 0.2;
    double crossover_rate = 06;
    gp::genetic_evolver1< tree_type , context , std::mt19937 > evolver( elite_rate , mutation_rate , crossover_rate , rng );
    std::vector< double > fitness( population_size , 0.0 );
    std::vector< tree_type > population( population_size );

    // evolver.fitness_function() = abc;
    // evolver.mutation_function() = xyz;
    // evolver.crossover_function() = abc;
    // evolver.random_individual_function() = abc;

//    initialize( population , context );
    for( size_t i=0 ; i<200 ; ++i )
    {
        evolver.next_generation( population , fitness , c );
        // report_population( population , cout );
        // report_statistics( population , cout );
    }

    return 0;
}
