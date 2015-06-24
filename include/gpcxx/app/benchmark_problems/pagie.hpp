/*
 * gpcxx/app/benchmark_problems/pagie.hpp
 * Date: 2015-06-24
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_APP_BENCHMARK_PROBLEMS_PAGIE_HPP_INCLUDED
#define GPCXX_APP_BENCHMARK_PROBLEMS_PAGIE_HPP_INCLUDED

#include <gpcxx/app/generate_evenly_spaced_test_data.hpp>

namespace gpcxx {
    
double pagie_func1( double x , double y ) {
    return 1.0 /  ( 1.0 + 1.0 / ( x * x * x * x ) )  + 1.0 / ( 1.0 + 1.0 / ( y * y * y * y ) );  }
        
double pagie_func2( double x , double y , double z ) {
    return 1.0 /  ( 1.0 + 1.0 / ( x * x * x * x ) )  + 1.0 / ( 1.0 + 1.0 / ( y * y * y * y ) ) + 1.0 / ( 1.0 + 1.0 / ( z * z * z *z ) ); }



auto generate_pagie1( void ) {
    return generate_evenly_spaced_test_data< 2 >( -5.0 , 5.01 , 0.4 , pagie_func1 ); }

auto generate_pagie2( void ) {
    return generate_evenly_spaced_test_data< 3 >( -5.0 , 5.01 , 0.4 , pagie_func2 ); }



} // namespace gpcxx


#endif // GPCXX_APP_BENCHMARK_PROBLEMS_PAGIE_HPP_INCLUDED
