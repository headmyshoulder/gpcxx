/*
 * gpcxx/app/generate_random_test_data.hpp
 * Date: 2015-06-15
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_APP_GENERATE_RANDOM_TEST_DATA_HPP_INCLUDED
#define GPCXX_APP_GENERATE_RANDOM_TEST_DATA_HPP_INCLUDED

#include <gpcxx/eval/regression_fitness.hpp>
#include <gpcxx/util/array_unpack.hpp>

#include <boost/fusion/functional/invocation/invoke.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/adapted/std_tuple.hpp>


namespace gpcxx {
    
namespace detail {

template< typename T >
struct factory
{
    template< typename... Args >
    T operator()( Args&&... args ) const
    {
        return T( std::forward< Args >( args ) ... );
    }
};
    
}


template< size_t Dim , typename Dist , typename Rng , typename Params , typename Func , typename Value = double >
auto generate_random_test_data( Rng& rng , size_t num_of_points , Params const& params , Func f )
{
    std::array< Dist , Dim > dists;
    for( size_t i=0 ; i<Dim ; ++i )
        dists[i] = boost::fusion::invoke( detail::factory< Dist >() , params[i] );
    
    regression_training_data< Value , Dim > data;
    for( size_t i=0 ; i<num_of_points ; ++i )
    {
        std::array< Value , Dim > arr;
        for( size_t d=0 ; d<Dim ; ++d )
        {
            arr[d] = dists[d]( rng );
            data.x[d][i] = arr[d];
        }
        data.y[i] = array_unpack( arr , f );        
    }

    return data;
}


} // namespace gpcxx


#endif // GPCXX_APP_GENERATE_RANDOM_TEST_DATA_HPP_INCLUDED
