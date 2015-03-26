/*
 * gpcxx/eval/hits.hpp
 * Date: 2015-03-26
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_EVAL_HITS_HPP_INCLUDED
#define GPCXX_EVAL_HITS_HPP_INCLUDED

#include <vector>
#include <algorithm>

namespace gpcxx {


constexpr auto hits_single = []( auto x , auto tolerance ) -> bool {
    using value_type = decltype( tolerance );
    return x > ( static_cast< value_type >( 1.0 ) - tolerance ); };
    
template< typename AdjustedFitness , typename Hits , typename Value >
void hits( AdjustedFitness const& af , Hits& hits , Value tolerance )
{
    hits.resize( af.size() );
    std::transform( af.begin() , af.end() , hits.begin() , [tolerance]( auto x ) { return hits_single( x , tolerance ); } );
}

template< typename AdjustedFitness , typename Value >
std::vector< bool > hits( AdjustedFitness const& af , Value tolerance )
{
    std::vector< bool > h;
    hits( af , h , tolerance );
    return h;
}





} // namespace gpcxx


#endif // GPCXX_EVAL_HITS_HPP_INCLUDED
