/*
  gpcxx/app/generate_normal_distributed_test_data.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GPCXX_APP_GENERATE_NORMAL_DISTRUBUTED_TEST_DATA_HPP_DEFINED
#define GPCXX_APP_GENERATE_NORMAL_DISTRUBUTED_TEST_DATA_HPP_DEFINED

#include <gpcxx/eval/regression_fitness.hpp>
#include <gpcxx/util/array_unpack.hpp>

#include <random>


namespace gpcxx {


template< size_t Dim , typename Rng , typename Func , typename Value = double >
auto generate_normal_distributed_test_data( Rng &rng , size_t num_of_points , Func f )
{
    regression_training_data< Value , Dim > data;
    
    std::normal_distribution< Value > dist( 0.0 , 1.0 );
    
    for( size_t i=0 ; i<num_of_points ; ++i )
    {
        std::array< Value , Dim > arr;
        for( size_t d=0 ; d<Dim ; ++d )
        {
            arr[d] = dist( rng );
            data.x[d][i] = arr[d];
        }
        data.y[i] = array_unpack( arr , f );        
    }
    return data;
}


} // namespace gpcxx

#endif // GPCXX_APP_GENERATE_NORMAL_DISTRUBUTED_TEST_DATA_HPP_DEFINED
