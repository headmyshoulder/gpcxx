/*
 * gpcxx/app/generate_uniform_distributed_test_data.hpp
 * Date: 2015-06-14
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_APP_GENERATE_UNIFORM_DISTRIBUTED_TEST_DATA_HPP_INCLUDED
#define GPCXX_APP_GENERATE_UNIFORM_DISTRIBUTED_TEST_DATA_HPP_INCLUDED

#include <gpcxx/app/generate_random_test_data.hpp>
#include <gpcxx/util/array_unpack.hpp>

#include <array>
#include <utility>
#include <random>
#include <algorithm>



namespace gpcxx {

template< size_t Dim , typename Rng , typename Params , typename F , typename Value = double >
auto generate_uniform_distributed_test_data( Rng& rng , size_t num_of_points , Params const& bounds , F f )
{
    return generate_random_test_data< Dim , std::uniform_real_distribution< Value > , Rng , Params , F , Value >( rng , num_of_points , bounds , f );
}

template< size_t Dim , typename Rng , typename F , typename Value = double >
auto generate_uniform_distributed_test_data( Rng& rng , size_t num_of_points , Value min , Value max , F f )
{
    using param_type = std::array< std::pair< Value , Value > , Dim >;
    param_type bounds;
    for( auto& bound : bounds )
    {
        bound.first = min;
        bound.second = max;
    }
    return generate_uniform_distributed_test_data< Dim , Rng , param_type , F , Value >( rng , num_of_points , bounds , f );
}

template< size_t Dim , typename Rng , typename F , typename Value = double >
auto generate_uniform_distributed_test_data( Rng& rng , size_t num_of_points , F f )
{
    return generate_uniform_distributed_test_data< Dim , Rng , F , Value >( rng , num_of_points , -1.0 , 1.0 , f );
}




} // namespace gpcxx


#endif // GPCXX_APP_GENERATE_UNIFORM_DISTRIBUTED_TEST_DATA_HPP_INCLUDED
