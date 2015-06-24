/*
 * gpcxx/app/benchmark_problems/nguyen.hpp
 * Date: 2015-06-24
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_APP_BENCHMARK_PROBLEMS_NGUYEN_HPP_INCLUDED
#define GPCXX_APP_BENCHMARK_PROBLEMS_NGUYEN_HPP_INCLUDED

#include <gpcxx/app/generate_uniform_distributed_test_data.hpp>

#include <cmath>

namespace gpcxx {

double nguyen_func1( double x ) { return x * x * x + x * x + x; }
double nguyen_func2( double x ) { return x * x * x * x + x * x * x + x * x + x; }
double nguyen_func3( double x ) { return x * x * x * x * x + x * x * x * x + x * x * x + x * x + x; }
double nguyen_func4( double x ) { return x * x * x * x * x *x + x * x * x * x * x + x * x * x * x + x * x * x + x * x + x; }
double nguyen_func5( double x ) { return std::sin( x * x ) * std::cos( x ) - 1.0; }
double nguyen_func6( double x ) { return std::sin( x ) + std::sin( x + x * x ); }
double nguyen_func7( double x ) { return std::log( x + 1.0 ) + std::log( x * x + 1.0 ); }
double nguyen_func8( double x ) { return std::sqrt( x ); }
double nguyen_func9( double x , double y ) { return std::sin( x ) + std::cos( x * x ); }
double nguyen_func10( double x , double y ) { return 2 * std::sin( x ) * std::cos( y ); }


template< typename Rng >
auto generate_nguyen1( Rng& rng ) {
    return generate_uniform_distributed_test_data< 1 >( rng , 20 , {{ std::make_pair( -1.0 , 1.0 ) }} , nguyen_func1 ); }

template< typename Rng >
auto generate_nguyen2( Rng& rng ) {
    return generate_uniform_distributed_test_data< 1 >( rng , 20 , {{ std::make_pair( -1.0 , 1.0 ) }} , nguyen_func2 ); }

template< typename Rng >
auto generate_nguyen3( Rng& rng ) {
    return generate_uniform_distributed_test_data< 1 >( rng , 20 , {{ std::make_pair( -1.0 , 1.0 ) }} , nguyen_func3 ); }

template< typename Rng >
auto generate_nguyen4( Rng& rng ) {
    return generate_uniform_distributed_test_data< 1 >( rng , 20 , {{ std::make_pair( -1.0 , 1.0 ) }} , nguyen_func4 ); }

template< typename Rng >
auto generate_nguyen5( Rng& rng ) {
    return generate_uniform_distributed_test_data< 1 >( rng , 20 , {{ std::make_pair( -1.0 , 1.0 ) }} , nguyen_func5 ); }

template< typename Rng >
auto generate_nguyen6( Rng& rng ) {
    return generate_uniform_distributed_test_data< 1 >( rng , 20 , {{ std::make_pair( -1.0 , 1.0 ) }} , nguyen_func6 ); }

template< typename Rng >
auto generate_nguyen7( Rng& rng ) {
    return generate_uniform_distributed_test_data< 1 >( rng , 20 , {{ std::make_pair( 0.0 , 2.0 ) }} , nguyen_func7 ); }

template< typename Rng >
auto generate_nguyen8( Rng& rng ) {
    return generate_uniform_distributed_test_data< 1 >( rng , 20 , {{ std::make_pair( 0.0 , 4.0 ) }} , nguyen_func8 ); }

template< typename Rng >
auto generate_nguyen9( Rng& rng ) {
    return generate_uniform_distributed_test_data< 2 >( rng , 100 , {{ std::make_pair( -1.0 , 1.0 ) , std::make_pair( -1.0 , 1.0 ) }} , nguyen_func9 ); }

template< typename Rng >
auto generate_nguyen10( Rng& rng ) {
    return generate_uniform_distributed_test_data< 2 >( rng , 100 , {{ std::make_pair( -1.0 , 1.0 ) , std::make_pair( -1.0 , 1.0 ) }} , nguyen_func10 ); }


} // namespace gpcxx


#endif // GPCXX_APP_BENCHMARK_PROBLEMS_NGUYEN_HPP_INCLUDED
