/*
 * gpcxx/app/benchmark_problems/korns.hpp
 * Date: 2015-06-24
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_APP_BENCHMARK_PROBLEMS_KORNS_HPP_INCLUDED
#define GPCXX_APP_BENCHMARK_PROBLEMS_KORNS_HPP_INCLUDED

#include <gpcxx/app/generate_uniform_distributed_test_data.hpp>

#include <cmath>


namespace gpcxx {

double korns_func1( double x0 , double x1 , double x2 , double x3 , double x4 ) {
    return 1.57 + 24.3 * x3; }

double korns_func2( double x0 , double x1 , double x2 , double x3 , double x4 ) {
    return 0.23 + 14.2 * ( x3 + x1 ) / 3.0 / x4; }

double korns_func3( double x0 , double x1 , double x2 , double x3 , double x4 ) {
    return -5.41 + 4.9 * ( x3 - x0 + x1 / x4 ) / ( 3.0 * x4 ); }

double korns_func4( double x0 , double x1 , double x2 , double x3 , double x4 ) {
    return -2.3 + 0.13 * std::sin( x2 ); }

double korns_func5( double x0 , double x1 , double x2 , double x3 , double x4 ) {
    return 3.0 + 2.13 * std::log( x4 ); }

double korns_func6( double x0 , double x1 , double x2 , double x3 , double x4 ) {
    return 1.3 + 0.13 * std::sqrt( x0 ); }

double korns_func7( double x0 , double x1 , double x2 , double x3 , double x4 ) {
    return 213.80940889 - 213.80940889 * exp( -0.54723748542 * x0 ); }

double korns_func8( double x0 , double x1 , double x2 , double x3 , double x4 ) {
    return 6.87 + 11.0 * std::sqrt( 7.23 * x0 * x3 * x4 ); }

double korns_func9( double x0 , double x1 , double x2 , double x3 , double x4 ) {
    return std::sqrt( x0 ) / std::log( x1 ) * std::exp( x2 ) / x3 / x3; }

double korns_func10( double x0 , double x1 , double x2 , double x3 , double x4 ) {
    return 0.81 + 24.3 * ( 2.0 * x1 + 3.0 * x2 * x3 ) / ( 4.0 * x3 * x3 * x3 + 5.0 * x4 * x4 * x4 * x4 ); }

double korns_func11( double x0 , double x1 , double x2 , double x3 , double x4 ) {
    return 6.87 + 11.0 * cos( 7.23 * x0 * x0 * x0 ); }

double korns_func12( double x0 , double x1 , double x2 , double x3 , double x4 ) {
    return 2.0 - 2.1 * cos( 9.8 * x0 ) * sin( 1.3 * x4 ); }

double korns_func13( double x0 , double x1 , double x2 , double x3 , double x4 ) {
    return 32.0 - 3.0 * tan( x0 ) / tan( x1 ) * tan( x2 ) / tan( x3 ); }

double korns_func14( double x0 , double x1 , double x2 , double x3 , double x4 ) {
    return 22.0 - 4.2 * ( cos( x0 ) - tan( x1 ) ) * tanh(x2) / sin( x3 ) ; }

double korns_func15( double x0 , double x1 , double x2 , double x3 , double x4 ) {
    return 12.0 - 6.0 * tan( x0 ) / exp( x1 ) * ( log( x2 ) - tan( x3 ) ); }


#define GPCXX_GENERATE_KORNS( I )                                        \
template< typename Rng >                                                 \
auto generate_korns ## I ( Rng& rng ) {                                      \
    return generate_uniform_distributed_test_data< 5 >( rng , 1000 ,     \
        {{ std::make_pair( -50 , 50.0 )                                  \
         , std::make_pair( -50 , 50.0 )                                  \
         , std::make_pair( -50 , 50.0 )                                  \
         , std::make_pair( -50 , 50.0 )                                  \
         , std::make_pair( -50 , 50.0 ) }} , korns_func ## I ); }
         
GPCXX_GENERATE_KORNS(  1 )
GPCXX_GENERATE_KORNS(  2 )
GPCXX_GENERATE_KORNS(  3 )
GPCXX_GENERATE_KORNS(  4 )
GPCXX_GENERATE_KORNS(  5 )
GPCXX_GENERATE_KORNS(  6 )
GPCXX_GENERATE_KORNS(  7 )
GPCXX_GENERATE_KORNS(  8 )
GPCXX_GENERATE_KORNS(  9 )
GPCXX_GENERATE_KORNS( 10 )
GPCXX_GENERATE_KORNS( 11 )
GPCXX_GENERATE_KORNS( 12 )
GPCXX_GENERATE_KORNS( 13 )
GPCXX_GENERATE_KORNS( 14 )
GPCXX_GENERATE_KORNS( 15 )
         




} // namespace gpcxx


#endif // GPCXX_APP_BENCHMARK_PROBLEMS_KORNS_HPP_INCLUDED
