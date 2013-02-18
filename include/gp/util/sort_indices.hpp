/*
 * helpers.h
 * Date: 2013-01-28
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef GP_UTIL_SORT_INDICES_HPP_INCLUDED
#define GP_UTIL_SORT_INDICES_HPP_INCLUDED

#include <algorithm>
#include <cmath>

template < typename Container , typename IndexContainer >
typename Container::iterator sort_indices( const Container &v , IndexContainer &idx )
{
    idx.resize( v.size() );
    for( size_t i = 0 ; i != idx.size() ; ++i ) idx[i] = i;

    typename Container::iterator iter = std::partition( idx.begin() , idx.end() ,
                                                        [&v]( size_t i ) { return std::isfinite( v[i] ); } );
    std::sort( idx.begin() , iter , [&v]( size_t i1 , size_t i2 ) { return ( v[i1] < v[i2] ); } );
    return iter;
}


#endif // GP_UTIL_SORT_INDICES_HPP_INCLUDED
