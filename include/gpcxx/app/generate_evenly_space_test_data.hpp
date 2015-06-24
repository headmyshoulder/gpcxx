/*
 * gpcxx/app/generate_evenly_space_test_data.hpp
 * Date: 2015-06-14
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_APP_GENERATE_EVENLY_SPACE_TEST_DATA_HPP_INCLUDED
#define GPCXX_APP_GENERATE_EVENLY_SPACE_TEST_DATA_HPP_INCLUDED

#include <gpcxx/eval/regression_fitness.hpp>
#include <gpcxx/util/array_unpack.hpp>

#include <algorithm>
#include <tuple>

namespace gpcxx {

namespace generate_detail {

template< size_t Dim , typename F , typename Value >
void iterate( gpcxx::regression_training_data< Value , Dim >& data , std::array< Value , Dim >& arr , std::array< std::tuple< Value , Value , Value > , Dim > const& params , F f , size_t iteration )
{
    if( iteration == Dim )
    {
        data.y.push_back( array_unpack( arr , f ) );
        for( size_t j=0 ; j<Dim ; ++j ) data.x[j].push_back( arr[j] );
    }
    else
    {
        for( Value x = std::get< 0 >( params[iteration] ) ; x <= std::get< 1 >( params[iteration] ) ; x += std::get< 2 >( params[iteration] ) )
        {
            arr[ iteration ] = x;
            iterate( data , arr , params , f , iteration + 1 );
        }
    }
}


} // namespace generate_detail

template< size_t Dim , typename F , typename Value = double >
auto generate_evenly_spaced_test_data( std::array< std::tuple< Value , Value , Value > , Dim > const& params , F f )
{
    gpcxx::regression_training_data< Value , Dim > data;
    
    std::array< Value , Dim > arr;
    generate_detail::iterate< Dim , F , Value >( data , arr , params , f , 0 );
    return data;
}

template< size_t Dim , typename F , typename Value = double >
auto generate_evenly_spaced_test_data( Value min , Value max , Value stepsize , F f )
{
    std::array< std::tuple< Value , Value , Value > , Dim > params;
    std::for_each( params.begin() , params.end() , [min,max,stepsize]( auto& x ) {
        std::get< 0 >( x ) = min;
        std::get< 1 >( x ) = max;
        std::get< 2 >( x ) = stepsize; } );
    return generate_evenly_spaced_test_data< Dim , F , Value >( params , f );
}



} // namespace gpcxx


#endif // GPCXX_APP_GENERATE_EVENLY_SPACE_TEST_DATA_HPP_INCLUDED
