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

#include <boost/range/adaptor/transformed.hpp>

#include <vector>
#include <algorithm>

namespace gpcxx {
    
namespace detail
{

    struct hits_single_fn
    {
        template< typename T >
        bool operator()( T x , T tolerance ) const
        {
            T one = static_cast< T >( 1.0 );
            return ( x > ( one - tolerance ) );
        }
    };

    
} // namespace detail

static constexpr detail::hits_single_fn hits_single = detail::hits_single_fn {};
    
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

template< typename AdjustedFitness , typename Value >
auto hits_view( AdjustedFitness const& af , Value tolerance )
{
    return boost::adaptors::transform( af , [tolerance]( auto x ) { return hits_single( x , tolerance ); } );
}





} // namespace gpcxx


#endif // GPCXX_EVAL_HITS_HPP_INCLUDED
