/*
  gpcxx/app/generate_test_data.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GPCXX_APP_GENERATE_TEST_DATA_HPP_DEFINED
#define GPCXX_APP_GENERATE_TEST_DATA_HPP_DEFINED

#include <gpcxx/eval/regression_fitness.hpp>
#include <gpcxx/util/array_unpack.hpp>

#include <random>


namespace gpcxx {



template< typename Value , size_t Dim , typename Sequence , typename Rng , typename Func >
void generate_regression_test_data( regression_training_data< Value , Dim , Sequence > & data , size_t n , Rng &rng , Func f )
{
    std::normal_distribution< Value > dist( 0.0 , 1.0 );
    
    data.y.resize( n );
    for( size_t d=0 ; d<Dim ; ++d )
        data.x[d].resize( n );
    
    for( size_t i=0 ; i<n ; ++i )
    {
        std::array< Value , Dim > arr;
        for( size_t d=0 ; d<Dim ; ++d )
        {
            arr[d] = dist( rng );
            data.x[d][i] = arr[d];
        }
        data.y[i] = array_unpack( arr , f );        
    }
}


} // namespace gpcxx

#endif // GPCXX_APP_GENERATE_TEST_DATA_HPP_DEFINED
