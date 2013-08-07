/*
  statistics.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef STATISTICS_HPP_DEFINED
#define STATISTICS_HPP_DEFINED

#include <utility>
#include <cmath>



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






#endif // STATISTICS_HPP_DEFINED
