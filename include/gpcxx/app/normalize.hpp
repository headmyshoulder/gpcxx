/*
  gpcxx/app/normalize.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GPCXX_APP_NORMALIZE_HPP_DEFINED
#define GPCXX_APP_NORMALIZE_HPP_DEFINED

#include <gpcxx/eval/regression_fitness.hpp>

#include <utility>
#include <cmath>

namespace gpcxx {


template< typename Vector >
std::pair< typename Vector::value_type , typename Vector::value_type >
normalize( Vector &x )
{
    typedef typename Vector::value_type value_type;
    value_type mean = 0.0 , sq_mean = 0.0;
    for( auto d : x )
    {
        mean += d;
        sq_mean += d * d;
    }
    mean /= double( x.size() );
    sq_mean /= double( x.size() );
    value_type stdev = std::sqrt( sq_mean - mean * mean );
    for( auto &d : x )
    {
        d = ( d - mean ) / stdev;
    }
    return std::make_pair( mean , stdev );
}

template< typename Value , size_t Dim , typename Sequence >
std::array< std::pair< Value , Value > , Dim + 1 >
normalize_regression( regression_training_data< Value , Dim , Sequence > & data )
{
    std::array< std::pair< Value , Value > , Dim > ret;
    ret[0] = normalize( data.y );
    for( size_t i=0 ; i<Dim ; ++i )
        ret[i+1] = normalize( data.x[i] );
    return ret;
}



} // namespace gpcxx

#endif // GPCXX_APP_NORMALIZE_HPP_DEFINED
