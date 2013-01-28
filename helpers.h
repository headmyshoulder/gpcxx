/*
 * helpers.h
 * Date: 2013-01-28
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef HELPERS_H_INCLUDED
#define HELPERS_H_INCLUDED

#include <vector>
#include <cmath>

template <typename T>
std::vector< size_t >::iterator sort_indexes( const std::vector< T > &v , std::vector< size_t > &idx )
{
    idx.resize( v.size() );
    for( size_t i = 0 ; i != idx.size() ; ++i ) idx[i] = i;

    std::vector< size_t >::iterator iter = std::partition( idx.begin() , idx.end() , [&v]( size_t i ) { return isfinite( v[i] ); } );
    sort( idx.begin() , iter , [&v]( size_t i1 , size_t i2 ) { return ( v[i1] < v[i2] ); } );
    return iter;
}

void create_random_indexes( std::vector< size_t > &idx , size_t N , size_t n )
{
    idx.resize( N );
    for( size_t i = 0 ; i < N ; ++i ) idx[i] = i;
    random_shuffle( idx.begin() , idx.end() );
    idx.resize( n );
}

#endif // HELPERS_H_INCLUDED
