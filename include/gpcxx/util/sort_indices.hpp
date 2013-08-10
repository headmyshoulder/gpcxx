/*
 * helpers.h
 * Date: 2013-01-28
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef GPCXX_UTIL_SORT_INDICES_HPP_INCLUDED
#define GPCXX_UTIL_SORT_INDICES_HPP_INCLUDED

#include <algorithm>
#include <cmath>

namespace gpcxx {

template < typename Container , typename IndexContainer >
typename IndexContainer::iterator sort_indices( const Container &v , IndexContainer &idx )
{
    idx.resize( v.size() );
    for( size_t i = 0 ; i != idx.size() ; ++i ) idx[i] = i;

    auto iter = std::partition( idx.begin() , idx.end() ,
                                [&v]( size_t i ) { return std::isfinite( v[i] ); } );
    std::sort( idx.begin() , iter , [&v]( size_t i1 , size_t i2 ) { return ( v[i1] < v[i2] ); } );
    return iter;
}

}

#endif // GPCXX_UTIL_SORT_INDICES_HPP_INCLUDED
