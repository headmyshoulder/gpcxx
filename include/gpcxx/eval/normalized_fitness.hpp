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
#include <iterator>


namespace gpcxx {

template< typename AdjustedFitness , typename NormalizedFitness >
void normalized_fitness( AdjustedFitness const& af , NormalizedFitness& nf )
{
    nf.resize( af.size() );
    auto sum = std::accumulate( af.begin() , af.end() , static_cast< typename AdjustedFitness::value_type >( 0.0 ) );
    std::transform( std::begin( af ) , std::end( af ) , std::begin( nf ) , [sum]( auto& x ) { return x / sum; } );
}

template< typename AdjustedFitness >
std::vector< typename AdjustedFitness::value_type > normalized_fitness( AdjustedFitness const& af )
{
    std::vector< typename AdjustedFitness::value_type > nf;
    normalized_fitness( af , nf );
    return nf;
}




} // namespace gpcxx


#endif // GPCXX_EVAL_NORMALIZED_FITNESS_HPP_INCLUDED
