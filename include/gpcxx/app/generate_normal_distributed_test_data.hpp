/*
  gpcxx/app/generate_normal_distributed_test_data.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GPCXX_APP_GENERATE_NORMAL_DISTRUBUTED_TEST_DATA_HPP_DEFINED
#define GPCXX_APP_GENERATE_NORMAL_DISTRUBUTED_TEST_DATA_HPP_DEFINED

#include <gpcxx/app/generate_random_test_data.hpp>
#include <gpcxx/eval/regression_fitness.hpp>
#include <gpcxx/util/array_unpack.hpp>

#include <random>
#include <array>


namespace gpcxx {
    
template< size_t Dim , typename Rng , typename Func , typename Params , typename Value = double >
auto generate_normal_distributed_test_data( Rng &rng , size_t num_of_points , Params const& params , Func f )
{
    return generate_random_test_data< Dim , std::normal_distribution< Value > >( rng , num_of_points , params , f );
}


template< size_t Dim , typename Rng , typename Func , typename Value = double >
auto generate_normal_distributed_test_data( Rng& rng , size_t num_of_points , Value mean , Value stddev , Func f )
{
    using param_type = std::array< std::pair< Value , Value > , Dim >;
    param_type params;
    for( auto& param : params )
    {
        param.first = mean;
        param.second = stddev;
    }
    return generate_normal_distributed_test_data< Dim , Rng , Func , param_type , Value >( rng , num_of_points , params , f );
}


template< size_t Dim , typename Rng , typename Func , typename Value = double >
auto generate_normal_distributed_test_data( Rng& rng , size_t num_of_points , Func f )
{
    return generate_normal_distributed_test_data< Dim , Rng , Func , Value >( rng , num_of_points , 0.0 , 1.0 , f );
}


} // namespace gpcxx

#endif // GPCXX_APP_GENERATE_NORMAL_DISTRUBUTED_TEST_DATA_HPP_DEFINED
