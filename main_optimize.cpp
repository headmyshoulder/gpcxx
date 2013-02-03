/*
 * main_optimize.cpp
 * Date: 2013-01-28
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include "genetic_optimizer.h"
#include "genetic_optimizer2.h"

#include <iostream>
#include <random>

#define tab "\t"

using namespace std;

typedef genetic_optimizer::vector_t vector_t;

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


int main( int argc , char *argv[] )
{
    std::mt19937 rng;


    vector_t x1 , x2 , x3 , y;
    generate_test_data( y , x1 , x2 , x3 , 10000 , rng , []( double x1 , double x2 , double x3 ) { return x1 + x2 - x3; } );
//    generate_test_data( y , x1 , x2 , x3 , 10000 , rng , []( double x1 , double x2 , double x3 ) { return x1 + x2 - 0.3 * x3; } );
//    normalize( x1 , x2 , x3 );
    

    genetic_optimizer2 optimizer( 1024 * 8 , 2 , 15 , 0.005 , 0.15 , 0.65 );
    optimizer.calc_fitness( y , x1 , x2 , x3 );
    cout << "Iteration 0" << endl;
    optimizer.report_population( cout );
    optimizer.report_statistics( cout );
    for( size_t i=0 ; i<200 ; ++i )
    {
        optimizer.iterate( y , x1 , x2 , x3 );
        cout << "Iteration " << i + 1 << endl;
        optimizer.report_population( cout );
        optimizer.report_statistics( cout );
//        break;
    }


    // genetic_optimizer optimizer( 1024 , 2 , 12 , 0.05 , 0.3 , 0.65 );
    // optimizer.calc_fitness( y , x1 , x2 , x3 );
    // for( size_t i=0 ; i<100 ; ++i )
    // {
    //     optimizer.iterate( y , x1 , x2 , x3 );
    //     cout << "Iteration " << i + 1 << endl;
    //     optimizer.report_population( cout );
    // }


    return 0;
}
