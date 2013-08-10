/*
 * helpers.h
 * Date: 2013-01-28
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef GPCXX_UTIL_CREATE_RANDOM_INDICES_HPP_INCLUDED
#define GPCXX_UTIL_CREATE_RANDOM_INDICES_HPP_INCLUDED

#include <algorithm>

namespace gpcxx {

template< class IndexContainer >
void create_random_indices( IndexContainer &idx , size_t N , size_t n )
{
    idx.resize( N );
    for( size_t i = 0 ; i < N ; ++i ) idx[i];
    std::random_shuffle( idx.begin() , idx.end() );
    idx.resize( n );
}

} // namespace gpcxx

#endif // GPCXX_UTIL_CREATE_RANDOM_INDICES_HPP_INCLUDED
