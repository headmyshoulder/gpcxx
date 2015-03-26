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


namespace gpcxx {


constexpr auto adjusted_fitness_single = []( auto fitness ) {
    using value_type = decltype( fitness );
    const value_type one = static_cast< value_type >( 1.0 );
    return one / ( one + fitness );
};

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



} // namespace gpcxx


#endif // GPCXX_EVAL_ADJUSTED_FITNESS_HPP_INCLUDED
