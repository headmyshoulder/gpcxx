/*
 * gpcxx/app/benchmark_problems/keijzer.hpp
 * Date: 2015-06-24
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_APP_BENCHMARK_PROBLEMS_KEIJZER_HPP_INCLUDED
#define GPCXX_APP_BENCHMARK_PROBLEMS_KEIJZER_HPP_INCLUDED

#include <gpcxx/app/generate_evenly_spaced_test_data.hpp>
#include <gpcxx/app/generate_uniform_distributed_test_data.hpp>
#include <gpcxx/util/assert.hpp>

#include <boost/math/constants/constants.hpp>
#include <cmath>


namespace gpcxx {

    
double keijzer_func4( double x ) { return 0.3 * x * sin( 2.0 * boost::math::double_constants::pi * x ); }

double keijzer_func5( double x ) {
    double sx = sin( x );
    double cx = cos( x );
    return x * x * x * exp( - x ) * cx * sx * ( sx * sx * cx - 1.0 );
}

double keijzer_func6( double x , double y , double z ) { return 30.0 * x * z / ( x - 10.0 ) / y / y; }

double keijzer_func7( double x ) {
    GPCXX_ASSERT( x >= 1.0 );
    double n = static_cast< double >( static_cast< int >( x ) );
    return n * ( n - 1 ) / 2.0;
}

double keijzer_func8( double x ) { return std::log( x ); }

double keijzer_func9( double x ) { return std::sqrt( x ); }

double keijzer_func10( double x ) { return std::asinh( x ); }

double keijzer_func11( double x , double y ) { return std::pow( x , y ); }

double keijzer_func12( double x , double y ) { return x * y + sin( ( x -1.0 ) * ( y-1.0 ) ); }

double keijzer_func13( double x , double y ) { return x * x * x * x - x * x * x + 0.5 * y * y - y; }

double keijzer_func14( double x , double y ) { return 6.0 * sin( x ) * cos( x ); }

double keijzer_func15( double x , double y ) { return 8.0 / ( 2.0 + x * x + y * y ); }

double keijzer_func16( double x , double y ) { return x * x * x / 3.0 + y * y * y / 2.0 - y - x; }




auto generate_keijzer1( void ) {
    return gpcxx::generate_evenly_spaced_test_data< 1 >( -1.0 , 1.0 , 0.1 , keijzer_func4 );
}

auto generate_keijzer2( void ) {
    return gpcxx::generate_evenly_spaced_test_data< 1 >( -2.0 , 2.0 , 0.1 , keijzer_func4 );
}

auto generate_keijzer3( void ) {
    return gpcxx::generate_evenly_spaced_test_data< 1 >( -3.0 , 3.0 , 0.1 , keijzer_func4 );
}

auto generate_keijzer4( void ) {
    return gpcxx::generate_evenly_spaced_test_data< 1 >( 0.0 , 10.0 , 0.05  , keijzer_func5 );
}

template< typename Rng >
auto generate_keijzer5( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 3 >( rng , 1000 ,
        std::array< std::pair< double , double > , 3 >{{ std::make_pair( -1.0 , 1.0 ) , std::make_pair( 1.0 , 2.0 ) , std::make_pair( -1.0 , 1.0 )  }} ,
        keijzer_func6 );
}

auto generate_keijzer6( void ) {
    return gpcxx::generate_evenly_spaced_test_data< 1 >( 1.0 , 50.0 , 1.0 , keijzer_func7 );
}

auto generate_keijzer7( void ) {
    return gpcxx::generate_evenly_spaced_test_data< 1 >( 1.0 , 100.0 , 1.0 , keijzer_func8 );
}

auto generate_keijzer8( void ) {
    return gpcxx::generate_evenly_spaced_test_data< 1 >( 0.0 , 100.0 , 1.0 , keijzer_func9 );
}
    
auto generate_keijzer9( void ) {
    return gpcxx::generate_evenly_spaced_test_data< 1 >( 0.0 , 100.0 , 1.0 , keijzer_func10 );
}

template< typename Rng >
auto generate_keijzer10( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 2 >( rng , 100 , 0.0 , 1.0 , keijzer_func11 );
}

template< typename Rng >
auto generate_keijzer11( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 2 >( rng , 20 , -3.0 , 3.0 , keijzer_func12 );
}

template< typename Rng >
auto generate_keijzer12( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 2 >( rng , 20 , -3.0 , 3.0 , keijzer_func13 );
}

template< typename Rng >
auto generate_keijzer13( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 2 >( rng , 20 , -3.0 , 3.0 , keijzer_func14 );
}

template< typename Rng >
auto generate_keijzer14( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 2 >( rng , 20 , -3.0 , 3.0 , keijzer_func15 );
}

template< typename Rng >
auto generate_keijzer15( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 2 >( rng , 20 , -3.0 , 3.0 , keijzer_func16 );
}




} // namespace gpcxx


#endif // GPCXX_APP_BENCHMARK_PROBLEMS_KEIJZER_HPP_INCLUDED
