/*
 * helpers.h
 * Date: 2013-01-28
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef GP_UTIL_CREATE_RANDOM_INDICES_HPP_INCLUDED
#define GP_UTIL_CREATE_RANDOM_INDICES_HPP_INCLUDED

#include <algorithm>

template< class IndexContainer >
void create_random_indices( IndexContainer &idx , size_t N , size_t n )
{
    idx.resize( N );
    for( size_t i = 0 ; i < N ; ++i ) idx[i];
    std::random_shuffle( idx.begin() , idx.end() );
    idx.resize( n );
}

#endif // GP_UTIL_CREATE_RANDOM_INDICES_HPP_INCLUDED
