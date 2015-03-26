/*
 * gpcxx/eval/adjusted_fitness.hpp
 * Date: 2015-03-25
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_EVAL_ADJUSTED_FITNESS_HPP_INCLUDED
#define GPCXX_EVAL_ADJUSTED_FITNESS_HPP_INCLUDED

#include <algorithm>
#include <vector>


namespace gpcxx {


namespace detail
{

    struct adjusted_fitness_single_fn
    {
        template< typename T >
        T operator()( T t ) const
        {
            const T one = static_cast< T >( 1.0 );
            return one / ( one + t );
        }
    };

    
} // namespace detail

static constexpr detail::adjusted_fitness_single_fn adjusted_fitness_single = detail::adjusted_fitness_single_fn {};


template< typename StandardizedFitness , typename AdjustedFitness >
void adjusted_fitness( StandardizedFitness const& sf , AdjustedFitness& af )
{
    af.resize( sf.size() );
    std::transform( sf.begin() , sf.end() , af.begin() , adjusted_fitness_single );
}

template< typename StandardizedFitness >
std::vector< typename StandardizedFitness::value_type > adjusted_fitness( StandardizedFitness const& sf )
{
    std::vector< typename StandardizedFitness::value_type > af;
    adjusted_fitness( sf , af );
    return af;
}


/*
VERSIONS

view = adjusted_fitness_view( sf );  // view
af = adjusted_fitness( sf );         // return transformed fitness
adjust_fitness( sf );                // transform inplace
adjust_fitness( sf , af );           // transform out-of-place

*/




} // namespace gpcxx


#endif // GPCXX_EVAL_ADJUSTED_FITNESS_HPP_INCLUDED
