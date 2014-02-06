/*
 * gpcxx/io/detail/read_polish.hpp
 * Date: 2014-02-06
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_IO_DETAIL_READ_POLISH_HPP_INCLUDED
#define GPCXX_IO_DETAIL_READ_POLISH_HPP_INCLUDED

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>


namespace gpcxx {
namespace detail {

template< typename Iter , typename Tree , typename Cursor , typename Mapper >
Iter read_polish( Iter first , Tree &tree , Cursor cursor , Mapper const& mapper )
{
    if( first == Iter {} ) return first;
    
    std::string elem { boost::begin( *first ) , boost::end( *first ) };
    
    auto const& generator = mapper.at( elem );
    auto current = tree.insert_below( cursor , generator.second() );

    for( size_t i=0 ; i<generator.first ; ++i )
        first = read_polish( ++first , tree , current , mapper );
    return first;
}

} // namespace detail
} // namespace gpcxx


#endif // GPCXX_IO_DETAIL_READ_POLISH_HPP_INCLUDED
