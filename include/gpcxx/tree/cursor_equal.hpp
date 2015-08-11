/*
 * gpcxx/tree/cursor_equal.hpp
 * Date: 2015-08-11
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_TREE_CURSOR_EQUAL_HPP_INCLUDED
#define GPCXX_TREE_CURSOR_EQUAL_HPP_INCLUDED

#include <cstddef>

namespace gpcxx {

template< typename Cursor1 , typename Cursor2 >
bool cursor_equal( Cursor1 const& c1 , Cursor2 const& c2 )
{
    if( c1.size() != c2.size() ) return false;
    if( *c1 != *c2 ) return false;
    for( size_t i=0 ; i<c1.size() ; ++i )
    {
        if( !cursor_equal( c1.children(i) , c2.children(i) ) ) return false;
    }
    return true;
}


} // namespace gpcxx


#endif // GPCXX_TREE_CURSOR_EQUAL_HPP_INCLUDED
