/*
 * performance/benchmarks/symbolic_regression_problems.hpp
 * Date: 2015-06-11
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_PERFORMANCE_BENCHMARKS_SYMBOLIC_REGRESSION_PROBLEMS_HPP_INCLUDED
#define GPCXX_PERFORMANCE_BENCHMARKS_SYMBOLIC_REGRESSION_PROBLEMS_HPP_INCLUDED


#include <gpcxx/eval/regression_fitness.hpp>

#include <random>
#include <array>
#include <utility>

template< typename Rng , typename F >
gpcxx::regression_training_data< double , 1 > generate_test_data( Rng& rng , size_t  num_of_points , double x_min , double x_max , F f )
{
    gpcxx::regression_training_data< double , 1 > c;
    std::uniform_real_distribution< double > dist( x_min , x_max );
    for( size_t i=0 ; i<num_of_points ; ++i )
    {
        double x = dist( rng );
        c.y.push_back( f( x ) );
        c.x[0].push_back( x );
    }
    return c;
}

template< typename Rng , typename F >
gpcxx::regression_training_data< double , 2 > generate_test_data( Rng& rng , size_t  num_of_points , double x_min , double x_max , double y_min , double y_max , F f )
{
    gpcxx::regression_training_data< double , 2 > c;
    std::uniform_real_distribution< double > dist1( x_min , x_max );
    std::uniform_real_distribution< double > dist2( y_min , y_max );
    for( size_t i=0 ; i<num_of_points ; ++i )
    {
        double x = dist1( rng );
        double y = dist2( rng );
        c.y.push_back( f( x , y ) );
        c.x[0].push_back( x );
        c.x[1].push_back( y );
    }
    return c;
}

template< typename Rng , typename F >
gpcxx::regression_training_data< double , 3 > generate_test_data( Rng& rng , size_t  num_of_points , double x_min , double x_max , double y_min , double y_max , double z_min , double z_max , F f )
{
    gpcxx::regression_training_data< double , 3 > c;
    std::uniform_real_distribution< double > dist1( x_min , x_max );
    std::uniform_real_distribution< double > dist2( y_min , y_max );
    std::uniform_real_distribution< double > dist3( z_min , z_max );
    for( size_t i=0 ; i<num_of_points ; ++i )
    {
        double x = dist1( rng );
        double y = dist2( rng );
        double z = dist3( rng );
        c.y.push_back( f( x , y , z ) );
        c.x[0].push_back( x );
        c.x[1].push_back( y );
        c.x[2].push_back( z );
    }
    return c;
}



template< typename Rng >
gpcxx::regression_training_data< double , 1 > generate_koza1( Rng& rng ) {
    return generate_test_data( rng , 20 , -1.0 , 1.0 , []( double x ) {
        return  x * x * x * x + x * x * x + x * x + x; } ); }

template< typename Rng >
gpcxx::regression_training_data< double , 1 > generate_koza2( Rng& rng ) {
        return generate_test_data( rng , 50 , -1.0 , 1.0 , []( double x ) {
            return x * x * x * x * x - 2.0 * x * x * x + x; } ); }

template< typename Rng >
gpcxx::regression_training_data< double , 1 > generate_koza3( Rng& rng ) {
    return generate_test_data( rng , 50 , -1.0 , 1.0 , []( double x ) {
        return x * x * x * x * x * x - 2.0 * x * x * x * x + x * x;  } ); }
    
template< typename Rng >
gpcxx::regression_training_data< double , 1 > generate_nguyen1( Rng& rng ) {
    return generate_test_data( rng , 20 , -1.0 , 1.0 , []( double x ) {
        return x * x * x + x * x + x; } ); }

template< typename Rng >
gpcxx::regression_training_data< double , 1 > generate_nguyen2( Rng& rng ) {
    return generate_test_data( rng , 20 , -1.0 , 1.0 , []( double x ) {
        return x * x * x * x + x * x * x + x * x + x; } ); }

template< typename Rng >
gpcxx::regression_training_data< double , 1 > generate_nguyen3( Rng& rng ) {
    return generate_test_data( rng , 20 , -1.0 , 1.0 , []( double x ) {
        return x * x * x * x * x + x * x * x * x + x * x * x + x * x + x; } ); }

template< typename Rng >
gpcxx::regression_training_data< double , 1 > generate_nguyen4( Rng& rng ) {
    return generate_test_data( rng , 20 , -1.0 , 1.0 , []( double x ) {
        return x * x * x * x * x *x + x * x * x * x * x + x * x * x * x + x * x * x + x * x + x; } ); }

template< typename Rng >
gpcxx::regression_training_data< double , 1 > generate_nguyen5( Rng& rng ) {
    return generate_test_data( rng , 20 , -1.0 , 1.0 , []( double x ) {
        return sin( x * x ) * cos( x ) - 1.0; } ); }

template< typename Rng >
gpcxx::regression_training_data< double , 1 > generate_nguyen6( Rng& rng ) {
    return generate_test_data( rng , 20 , -1.0 , 1.0 , []( double x ) {
        return sin( x ) + sin( x + x * x ); } ); }

template< typename Rng >
gpcxx::regression_training_data< double , 1 > generate_nguyen7( Rng& rng ) {
    return generate_test_data( rng , 20 , 0.0 , 2.0 , []( double x ) {
        return log( x + 1.0 ) + log( x * x + 1.0 ); } ); }

template< typename Rng >
gpcxx::regression_training_data< double , 1 > generate_nguyen8( Rng& rng ) {
    return generate_test_data( rng , 20 , 0.0 , 4.0 , []( double x ) {
        return sqrt( x ); } ); }


template< typename Rng >
gpcxx::regression_training_data< double , 2 > generate_nguyen9( Rng& rng ) {
    return generate_test_data( rng , 100 , -1.0 , 1.0 , -1.0 , 1.0 , []( double x , double y ) {
        return sin( x ) + cos( x * x ); } ); }

template< typename Rng >
gpcxx::regression_training_data< double , 1 > generate_nguyen10( Rng& rng ) {
    return generate_test_data( rng , 100 , -1.0 , 1.0 , -1.0 , 1.0 , []( double x , double y ) {
        return 2 * sin( x ) * cos( y ); } ); }






#endif // GPCXX_PERFORMANCE_BENCHMARKS_SYMBOLIC_REGRESSION_PROBLEMS_HPP_INCLUDED
