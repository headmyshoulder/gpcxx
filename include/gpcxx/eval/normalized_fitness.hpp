/*
 * gpcxx/eval/normalized_fitness.hpp
 * Date: 2015-03-26
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_EVAL_NORMALIZED_FITNESS_HPP_INCLUDED
#define GPCXX_EVAL_NORMALIZED_FITNESS_HPP_INCLUDED

#include <algorithm>
#include <numeric>


namespace gpcxx {

template< typename AdjustedFitness >
void normalize_fitness( AdjustedFitness& fitness )
{
    auto sum = std::accumulate( fitness.begin() , fitness.end() , static_cast< typename AdjustedFitness::value_type >( 0.0 ) );
    std::for_each( fitness.begin() , fitness.end() , [sum]( auto& x ) { x /= sum; } );
}




} // namespace gpcxx


#endif // GPCXX_EVAL_NORMALIZED_FITNESS_HPP_INCLUDED
