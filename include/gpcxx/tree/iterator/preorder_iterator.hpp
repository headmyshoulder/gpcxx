/*
 * gpcxx/tree/iterator/preorder_iterator.hpp
 * Date: 2015-01-22
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_TREE_ITERATOR_PREORDER_ITERATOR_HPP_INCLUDED
#define GPCXX_TREE_ITERATOR_PREORDER_ITERATOR_HPP_INCLUDED

#include <gpcxx/tree/iterator/iterator_base.hpp>


#include <map>
#include <iostream>
using namespace std;


namespace gpcxx {

    

struct preorder_policy
{
    template< typename Cursor >
    static bool successor( Cursor &c )
    {
        if( c.size() > 0 )
        {
            c = c.children( 0 );
            return true;
        }
        while( true )
        {
            if( c.is_root() )
            {
                ++c;
                return true;
            }
            
            Cursor d = c;
            ++d;
            if( d != c.parent().end() )
            {
                c = d;
                return true;
            }
            c = c.parent();
        }
    }
    
    template< typename Cursor >
    static bool predecessor( Cursor &c )
    {
        return true;
    }
    
    template< typename Cursor >
    static bool first( Cursor &c )
    {
        return true;
    }
    
    template< typename Cursor >
    static bool last( Cursor &c )
    {
        ++c;
        return true;
    }
};


template< typename Cursor >
using preorder_iterator = iterator_base< Cursor , preorder_policy >;

template< typename Cursor >
preorder_iterator< Cursor > begin_preorder( Cursor c )
{
    preorder_policy::first( c );
    return preorder_iterator< Cursor > { c };
}

template< typename Cursor >
preorder_iterator< Cursor > end_preorder( Cursor c )
{
    preorder_policy::last( c );
    return preorder_iterator< Cursor > { c };
}


} // namespace gpcxx


#endif // GPCXX_TREE_ITERATOR_PREORDER_ITERATOR_HPP_INCLUDED