/*
 * gpcxx/app/benchmark_problems/vladislavleva.hpp
 * Date: 2015-06-24
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_APP_BENCHMARK_PROBLEMS_VLADISLAVLEVA_HPP_INCLUDED
#define GPCXX_APP_BENCHMARK_PROBLEMS_VLADISLAVLEVA_HPP_INCLUDED

#include <gpcxx/app/generate_uniform_distributed_test_data.hpp>
#include <gpcxx/app/generate_evenly_space_test_data.hpp>

#include <cmath>

namespace gpcxx {

double vladislavleva_func1( double x1 , double x2 ) {
    return std::exp( - ( x1 - 1.0 ) ) / ( 1.2 + ( x2 - 2.5 ) * ( x2 - 2.5 ) ); }

double vladislavleva_func2( double x ) {
    double sx = std::sin( x );
    double cx = std::cos( x );
    return std::exp( - x ) * x * x * x * cx * sx * ( cx * sx * sx - 1.0 ); }
    
double vladislavleva_func3( double x1 , double x2 ) {
    return vladislavleva_func2( x1 ) * ( x2 - 5.0 ); }
    
double vladislavleva_func4( double x1 , double x2 , double x3 , double x4 , double x5 ) {
    auto f = []( double x ) { return ( x - 3.0 ) * ( x - 3.0 ); };
    return 10.0 / ( 5.0 + f( x1 ) + f( x2 ) + f( x3 ) + f( x4 ) + f( x5 ) ); }
    
double vladislavleva_func5( double x1 , double x2 , double x3 ) {
    return 30.0 * ( x1 - 1.0 ) * ( x3 - 1.0 ) / x2 / x2 / ( x1 - 10.0 ); }
    
double vladislavleva_func6( double x1 , double x2 ) {
    return 6.0 * std::sin( x1 ) * std::cos( x1 ); }
    
double vladislavleva_func7( double x1 , double x2 ) {
    return ( x1 - 3.0 ) * ( x2 - 3.0 ) + 2.0 * sin( ( x1 - 4.0 ) * ( x2 -4.0 ) ); }
    
double vladislavleva_func8( double x1 , double x2 ) {
    auto quart = []( double x ) { return x * x * x * x; };
    auto triple = []( double x ) { return x * x * x; };
    return ( quart( x1 - 3.0 ) + triple( x2 - 3.0 ) - ( x2 - 3.0 )  ) / ( quart( x2 - 2.0 ) + 10.0 ); }


template< typename Rng >
auto generate_vladislavleva1( Rng& rng ) {
    return generate_uniform_distributed_test_data< 2 >( rng , 100 , 0.3 , 4.0 , vladislavleva_func1 ); }

auto generate_vladislavleva2( void ) {
    return generate_evenly_spaced_test_data< 1 >( 0.05 , 10.0 , 0.1 , vladislavleva_func2 ); }

auto generate_vladislavleva3( void ) {
    return generate_evenly_spaced_test_data< 2 >( {{ std::make_tuple( 0.05 , 10.0 , 0.1 ) , std::make_tuple( 0.05 , 10.05 , 2.0 ) }} , vladislavleva_func3 ); }

template< typename Rng >
auto generate_vladislavleva4( Rng& rng ) {
    return generate_uniform_distributed_test_data< 5 >( rng , 1024 , 0.05 , 6.05 , vladislavleva_func4 ); }

template< typename Rng >
auto generate_vladislavleva5( Rng& rng ) {
    return generate_uniform_distributed_test_data< 3 >( rng , 300 ,
        {{ std::make_pair( 0.05 , 2.0 ) , std::make_pair( 1.0 , 2.0 ) , std::make_tuple( 0.05 , 2.0 ) }} , vladislavleva_func5 ); }

template< typename Rng >
auto generate_vladislavleva6( Rng& rng ) {
    return generate_uniform_distributed_test_data< 2 >( rng , 30 , 0.1 , 5.9 , vladislavleva_func6 ); }

template< typename Rng >
auto generate_vladislavleva7( Rng& rng ) {
    return generate_uniform_distributed_test_data< 2 >( rng , 300 , 0.05 , 6.05 , vladislavleva_func7 ); }

template< typename Rng >
auto generate_vladislavleva8( Rng& rng ) {
    return generate_uniform_distributed_test_data< 2 >( rng , 50 , 0.05 , 6.05 , vladislavleva_func8 ); }


} // namespace gpcxx


#endif // GPCXX_APP_BENCHMARK_PROBLEMS_VLADISLAVLEVA_HPP_INCLUDED
