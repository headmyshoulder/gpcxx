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

#include <gpcxx/app/generate_uniform_distributed_test_data.hpp>
#include <gpcxx/app/generate_evenly_space_test_data.hpp>




template< typename Rng >
auto generate_koza1( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 1 >( rng , 20 , { std::make_pair( -1.0 , 1.0 ) } , []( double x ) {
        return  x * x * x * x + x * x * x + x * x + x; } ); }

template< typename Rng >
auto generate_koza2( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 1 >( rng , 20 , { std::make_pair( -1.0 , 1.0 ) } , []( double x ) {
            return x * x * x * x * x - 2.0 * x * x * x + x; } ); }

template< typename Rng >
auto generate_koza3( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 1 >( rng , 20 , { std::make_pair( -1.0 , 1.0 ) } , []( double x ) {
        return x * x * x * x * x * x - 2.0 * x * x * x * x + x * x;  } ); }



template< typename Rng >
auto generate_nguyen1( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 1 >( rng , 20 , { std::make_pair( -1.0 , 1.0 ) } , []( double x ) {
        return x * x * x + x * x + x; } ); }

template< typename Rng >
auto generate_nguyen2( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 1 >( rng , 20 , { std::make_pair( -1.0 , 1.0 ) } , []( double x ) {
        return x * x * x * x + x * x * x + x * x + x; } ); }

template< typename Rng >
auto generate_nguyen3( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 1 >( rng , 20 , { std::make_pair( -1.0 , 1.0 ) } , []( double x ) {
        return x * x * x * x * x + x * x * x * x + x * x * x + x * x + x; } ); }

template< typename Rng >
auto generate_nguyen4( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 1 >( rng , 20 , { std::make_pair( -1.0 , 1.0 ) } , []( double x ) {
        return x * x * x * x * x *x + x * x * x * x * x + x * x * x * x + x * x * x + x * x + x; } ); }

template< typename Rng >
auto generate_nguyen5( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 1 >( rng , 20 , { std::make_pair( -1.0 , 1.0 ) } , []( double x ) {
        return sin( x * x ) * cos( x ) - 1.0; } ); }

template< typename Rng >
auto generate_nguyen6( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 1 >( rng , 20 , { std::make_pair( -1.0 , 1.0 ) } , []( double x ) {
        return sin( x ) + sin( x + x * x ); } ); }

template< typename Rng >
auto generate_nguyen7( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 1 >( rng , 20 , { std::make_pair( 0.0 , 2.0 ) } , []( double x ) {
        return log( x + 1.0 ) + log( x * x + 1.0 ); } ); }

template< typename Rng >
auto generate_nguyen8( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 1 >( rng , 20 , { std::make_pair( 0.0 , 4.0 ) } , []( double x ) {
        return sqrt( x ); } ); }

template< typename Rng >
auto generate_nguyen9( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 2 >( rng , 100 , { std::make_pair( -1.0 , 1.0 ) , std::make_pair( -1.0 , 1.0 ) } , []( double x , double y ) {
        return sin( x ) + cos( x * x ); } ); }

template< typename Rng >
auto generate_nguyen10( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 2 >( rng , 100 , { std::make_pair( -1.0 , 1.0 ) , std::make_pair( -1.0 , 1.0 ) } , []( double x , double y ) {
        return 2 * sin( x ) * cos( y ); } ); }



auto generate_pagie1( void ) {
    return gpcxx::generate_evenly_spaced_test_data< 2 >( -5.0 , 5.01 , 0.4 , []( double x , double y ) {
        return 1.0 /  ( 1.0 + 1.0 / ( x * x * x * x ) )  + 1.0 / ( 1.0 + 1.0 / ( y * y * y * y ) );  } );
}

auto generate_pagie2( void ) {
    return gpcxx::generate_evenly_spaced_test_data< 3 >( -5.0 , 5.01 , 0.4 , []( double x , double y , double z ) {
        return 1.0 /  ( 1.0 + 1.0 / ( x * x * x * x ) )  + 1.0 / ( 1.0 + 1.0 / ( y * y * y * y ) ) + 1.0 / ( 1.0 + 1.0 / ( z * z * z *z ) ) ;  } );
}






#endif // GPCXX_PERFORMANCE_BENCHMARKS_SYMBOLIC_REGRESSION_PROBLEMS_HPP_INCLUDED
