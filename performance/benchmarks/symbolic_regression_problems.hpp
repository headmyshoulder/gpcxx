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

#include <boost/math/constants/constants.hpp>
#include <boost/math/special_functions/asinh.hpp>
#include <cmath>


template< typename Rng >
auto generate_koza1( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 1 >( rng , 20 , {{ std::make_pair( -1.0 , 1.0 ) }} , []( double x ) {
        return  x * x * x * x + x * x * x + x * x + x; } ); }

template< typename Rng >
auto generate_koza2( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 1 >( rng , 20 , {{ std::make_pair( -1.0 , 1.0 ) }} , []( double x ) {
            return x * x * x * x * x - 2.0 * x * x * x + x; } ); }

template< typename Rng >
auto generate_koza3( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 1 >( rng , 20 , {{ std::make_pair( -1.0 , 1.0 ) }} , []( double x ) {
        return x * x * x * x * x * x - 2.0 * x * x * x * x + x * x;  } ); }



template< typename Rng >
auto generate_nguyen1( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 1 >( rng , 20 , {{ std::make_pair( -1.0 , 1.0 ) }} , []( double x ) {
        return x * x * x + x * x + x; } ); }

template< typename Rng >
auto generate_nguyen2( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 1 >( rng , 20 , {{ std::make_pair( -1.0 , 1.0 ) }} , []( double x ) {
        return x * x * x * x + x * x * x + x * x + x; } ); }

template< typename Rng >
auto generate_nguyen3( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 1 >( rng , 20 , {{ std::make_pair( -1.0 , 1.0 ) }} , []( double x ) {
        return x * x * x * x * x + x * x * x * x + x * x * x + x * x + x; } ); }

template< typename Rng >
auto generate_nguyen4( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 1 >( rng , 20 , {{ std::make_pair( -1.0 , 1.0 ) }} , []( double x ) {
        return x * x * x * x * x *x + x * x * x * x * x + x * x * x * x + x * x * x + x * x + x; } ); }

template< typename Rng >
auto generate_nguyen5( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 1 >( rng , 20 , {{ std::make_pair( -1.0 , 1.0 ) }} , []( double x ) {
        return sin( x * x ) * cos( x ) - 1.0; } ); }

template< typename Rng >
auto generate_nguyen6( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 1 >( rng , 20 , {{ std::make_pair( -1.0 , 1.0 ) }} , []( double x ) {
        return sin( x ) + sin( x + x * x ); } ); }

template< typename Rng >
auto generate_nguyen7( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 1 >( rng , 20 , {{ std::make_pair( 0.0 , 2.0 ) }} , []( double x ) {
        return log( x + 1.0 ) + log( x * x + 1.0 ); } ); }

template< typename Rng >
auto generate_nguyen8( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 1 >( rng , 20 , {{ std::make_pair( 0.0 , 4.0 ) }} , []( double x ) {
        return sqrt( x ); } ); }

template< typename Rng >
auto generate_nguyen9( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 2 >( rng , 100 , {{ std::make_pair( -1.0 , 1.0 ) , std::make_pair( -1.0 , 1.0 ) }} , []( double x , double y ) {
        return sin( x ) + cos( x * x ); } ); }

template< typename Rng >
auto generate_nguyen10( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 2 >( rng , 100 , {{ std::make_pair( -1.0 , 1.0 ) , std::make_pair( -1.0 , 1.0 ) }} , []( double x , double y ) {
        return 2 * sin( x ) * cos( y ); } ); }



auto generate_pagie1( void ) {
    return gpcxx::generate_evenly_spaced_test_data< 2 >( -5.0 , 5.01 , 0.4 , []( double x , double y ) {
        return 1.0 /  ( 1.0 + 1.0 / ( x * x * x * x ) )  + 1.0 / ( 1.0 + 1.0 / ( y * y * y * y ) );  } );
}

auto generate_pagie2( void ) {
    return gpcxx::generate_evenly_spaced_test_data< 3 >( -5.0 , 5.01 , 0.4 , []( double x , double y , double z ) {
        return 1.0 /  ( 1.0 + 1.0 / ( x * x * x * x ) )  + 1.0 / ( 1.0 + 1.0 / ( y * y * y * y ) ) + 1.0 / ( 1.0 + 1.0 / ( z * z * z *z ) ) ;  } );
}


namespace keijzer_functions {
    
auto eq4 = []( double x ) { return 0.3 * x * sin( 2.0 * boost::math::double_constants::pi * x ); };

auto eq5 = []( double x ) {
    double sx = sin( x );
    double cx = cos( x );
    return x * x * x * exp( - x ) * cx * sx * ( sx * sx * cx - 1.0 );
};

auto eq6 = []( double x , double y , double z ) { return 30.0 * x * z / ( x - 10.0 ) / y / y; };

auto eq7 = []( double x ) {
    assert( x >= 1.0 );
    double n = static_cast< double >( static_cast< int >( x ) );
    return n * ( n - 1 ) / 2.0;
};

auto eq8 = []( double x ) { return std::log( x );};

auto eq9 = []( double x ) { return std::sqrt( x ); };

auto eq10 = []( double x ) { return std::asinh( x ); };

auto eq11 = []( double x , double y ) { return std::pow( x , y ); };

auto eq12 = []( double x , double y ) { return x * y + sin( ( x -1.0 ) * ( y-1.0 ) ); };

auto eq13 = []( double x , double y ) { return x * x * x * x - x * x * x + 0.5 * y * y - y; };

auto eq14 = []( double x , double y ) { return 6.0 * sin( x ) * cos( x ); };

auto eq15 = []( double x , double y ) { return 8.0 / ( 2.0 + x * x + y * y ); };

auto eq16 = []( double x , double y ) { return x * x * x / 3.0 + y * y * y / 2.0 - y - x; };

} // namespace keijzer_functions


auto generate_keijzer1( void ) {
    return gpcxx::generate_evenly_spaced_test_data< 1 >( -1.0 , 1.0 , 0.1 , keijzer_functions::eq4 );
}

auto generate_keijzer2( void ) {
    return gpcxx::generate_evenly_spaced_test_data< 1 >( -2.0 , 2.0 , 0.1 , keijzer_functions::eq4 );
}

auto generate_keijzer3( void ) {
    return gpcxx::generate_evenly_spaced_test_data< 1 >( -3.0 , 3.0 , 0.1 , keijzer_functions::eq4 );
}

auto generate_keijzer4( void ) {
    return gpcxx::generate_evenly_spaced_test_data< 1 >( 0.0 , 10.0 , 0.05  , keijzer_functions::eq5 );
}

template< typename Rng >
auto generate_keijzer5( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 3 >( rng , 1000 ,
        {{ std::make_pair( -1.0 , 1.0 ) , std::make_pair( 1.0 , 2.0 ) , std::make_pair( -1.0 , 1.0 )  }} ,
        keijzer_functions::eq6 );
}

auto generate_keijzer6( void ) {
    return gpcxx::generate_evenly_spaced_test_data< 1 >( 1.0 , 50.0 , 1.0 , keijzer_functions::eq7 );
}

auto generate_keijzer7( void ) {
    return gpcxx::generate_evenly_spaced_test_data< 1 >( 1.0 , 100.0 , 1.0 , keijzer_functions::eq8 );
}

auto generate_keijzer8( void ) {
    return gpcxx::generate_evenly_spaced_test_data< 1 >( 0.0 , 100.0 , 1.0 , keijzer_functions::eq9 );
}
    
auto generate_keijzer9( void ) {
    return gpcxx::generate_evenly_spaced_test_data< 1 >( 0.0 , 100.0 , 1.0 , keijzer_functions::eq10 );
}

template< typename Rng >
auto generate_keijzer10( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 2 >( rng , 100 ,
        {{ std::make_pair( 0.0 , 1.0 ) , std::make_pair( 0.0 , 1.0 ) }} ,
        keijzer_functions::eq11 );
}

template< typename Rng >
auto generate_keijzer11( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 2 >( rng , 20 ,
        {{ std::make_pair( -3.0 , 3.0 ) , std::make_pair( -3.0 , 3.0 ) }} ,
        keijzer_functions::eq12 );
}

template< typename Rng >
auto generate_keijzer12( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 2 >( rng , 20 ,
        {{ std::make_pair( -3.0 , 3.0 ) , std::make_pair( -3.0 , 3.0 ) }} ,
        keijzer_functions::eq13 );
}

template< typename Rng >
auto generate_keijzer13( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 2 >( rng , 20 ,
        {{ std::make_pair( -3.0 , 3.0 ) , std::make_pair( -3.0 , 3.0 ) }} ,
        keijzer_functions::eq14 );
}

template< typename Rng >
auto generate_keijzer14( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 2 >( rng , 20 ,
        {{ std::make_pair( -3.0 , 3.0 ) , std::make_pair( -3.0 , 3.0 ) }} ,
        keijzer_functions::eq15 );
}

template< typename Rng >
auto generate_keijzer15( Rng& rng ) {
    return gpcxx::generate_uniform_distributed_test_data< 2 >( rng , 20 ,
        {{ std::make_pair( -3.0 , 3.0 ) , std::make_pair( -3.0 , 3.0 ) }} ,
        keijzer_functions::eq16 );
}









#endif // GPCXX_PERFORMANCE_BENCHMARKS_SYMBOLIC_REGRESSION_PROBLEMS_HPP_INCLUDED
