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

#include <gpcxx/eval/regression_fitness.hpp>
#include <gpcxx/util/array_unpack.hpp>

#include <array>
#include <utility>
#include <random>
#include <algorithm>



namespace gpcxx {

template< size_t Dim , typename Rng , typename F , typename Value = double >
auto generate_uniform_distributed_test_data( Rng& rng , size_t num_of_points , std::array< std::pair< Value , Value > , Dim > const& bounds , F f )
{
    gpcxx::regression_training_data< Value , Dim > c;
    for( size_t j=0 ; j<num_of_points ; ++j )
    {
        std::array< Value , Dim > arr;
        for( size_t i=0 ; i<Dim ; ++i )
        {
            std::uniform_real_distribution< Value > dist( bounds[i].first , bounds[i].second );
            arr[i] = dist( rng );
        }
        c.y.push_back( gpcxx::array_unpack( arr , f ) );
        for( size_t i=0 ; i<Dim ; ++i )
            c.x[i].push_back( arr[i] );
    }
    return c;
}

template< size_t Dim , typename Rng , typename F , typename Value = double >
auto generate_uniform_distributed_test_data( Rng& rng , size_t num_of_points , Value min , Value max , F f )
{
    std::array< std::pair< Value , Value > , Dim > bounds;
    std::for_each( bounds.begin() , bounds.end() , [min,max]( auto& bound ) { bound.first = min; bound.second = max; } );
    return generate_uniform_distributed_test_data< Dim , Rng , F , Value >( rng , num_of_points , bounds , f );
}

template< size_t Dim , typename Rng , typename F , typename Value = double >
auto generate_uniform_distributed_test_data( Rng& rng , size_t num_of_points , F f )
{
    return generate_uniform_distributed_test_data< Dim , Rng , F , Value >( rng , num_of_points , -1.0 , 1.0 , f );
}




} // namespace gpcxx


#endif // GPCXX_APP_GENERATE_UNIFORM_DISTRIBUTED_TEST_DATA_HPP_INCLUDED
