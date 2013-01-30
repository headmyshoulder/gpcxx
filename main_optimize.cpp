/*
 * main_optimize.cpp
 * Date: 2013-01-28
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include "genetic_optimizer.h"

#include <iostream>
#include <random>

#define tab "\t"

using namespace std;

typedef genetic_optimizer::vector_t vector_t;

template< class Rng , class Func >
void generate_test_data( vector_t &y , vector_t &x1 , vector_t &x2 , vector_t &x3 , size_t n , Rng &rng , Func f )
{
    std::normal_distribution< double > dist1( 0.0 , 1.0 ) , dist2( 1.0 , 3.0 ) , dist3( -1.0 , 2.0 );
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
    std::mt19937 rng;


    vector_t x1 , x2 , x3 , y;
    generate_test_data( y , x1 , x2 , x3 , 10000 , rng , []( double x1 , double x2 , double x3 ) { return x1 + x2 - 0.333 * x3; } );

    genetic_optimizer optimizer( 1024 , 2 , 12 , 0.05 , 0.3 , 0.65 );
    optimizer.calc_fitness( y , x1 , x2 , x3 );
    for( size_t i=0 ; i<100 ; ++i )
    {
        optimizer.iterate( y , x1 , x2 , x3 );
        cout << "Iteration " << i + 1 << endl;
        optimizer.report_population( cout );
    }
    // genetic_optimizer::fitness_vector_t fitness = optimizer.fitness_vector();
    // sort( fitness.begin() , fitness.end() );
    // for( auto f : fitness ) cout << f << endl;

    return 0;
}
