/*
 * gpcxx/app/benchmark_problems/koza.hpp
 * Date: 2015-06-24
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_APP_BENCHMARK_PROBLEMS_KOZA_HPP_INCLUDED
#define GPCXX_APP_BENCHMARK_PROBLEMS_KOZA_HPP_INCLUDED

#include <gpcxx/app/generate_uniform_distributed_test_data.hpp>



namespace gpcxx {

inline double koza_func1( double x ) { return x * x * x * x + x * x * x + x * x + x; }

inline double koza_func2( double x ) { return x * x * x * x * x - 2.0 * x * x * x + x; }

inline double koza_func3( double x ) { return x * x * x * x * x * x - 2.0 * x * x * x * x + x * x;  }


template< typename Rng >
auto generate_koza1( Rng& rng ) {
    return generate_uniform_distributed_test_data< 1 >( rng , 20 , -1.0 , 1.0 , koza_func1 ); }

template< typename Rng >
auto generate_koza2( Rng& rng ) {
    return generate_uniform_distributed_test_data< 1 >( rng , 20 , -1.0 , 1.0 , koza_func2 ); }

template< typename Rng >
auto generate_koza3( Rng& rng ) {
    return generate_uniform_distributed_test_data< 1 >( rng , 20 , -1.0 , 1.0 , koza_func3 ); }



} // namespace gpcxx


#endif // GPCXX_APP_BENCHMARK_PROBLEMS_KOZA_HPP_INCLUDED
