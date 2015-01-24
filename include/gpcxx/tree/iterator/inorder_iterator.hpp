/*
 * gpcxx/tree/iterator/inorder_iterator.hpp
 * Date: 2015-01-22
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_TREE_ITERATOR_INORDER_ITERATOR_HPP_INCLUDED
#define GPCXX_TREE_ITERATOR_INORDER_ITERATOR_HPP_INCLUDED

#include <gpcxx/tree/iterator/iterator_base.hpp>


namespace gpcxx {

struct inorder_policy
{
    template< typename Cursor >
    static bool successor( Cursor &c )
    {
        
        return true;
    }
    
    template< typename Cursor >
    static bool predecessor( Cursor &c )
    {
        return true;
    }
    
    template< typename Cursor >
    static bool first( Cursor &c )
    {
        while( c.size() != 0 )
        {
            c = c.children( 0 );
        }
        return true;
    }
    
    template< typename Cursor >
    static bool last( Cursor &c )
    {
        return true;
    }
};

template< typename Cursor >
using inorder_iterator = iterator_base< Cursor , inorder_policy >;

template< typename Cursor >
inorder_iterator< Cursor > begin_inorder( Cursor c )
{
    inorder_policy::first( c );
    return inorder_iterator< Cursor > { c };
}

template< typename Cursor >
inorder_iterator< Cursor > end_inorder( Cursor c )
{
    inorder_policy::last( c );
    return inorder_iterator< Cursor > { c };
}




} // namespace gpcxx


#endif // GPCXX_TREE_ITERATOR_INORDER_ITERATOR_HPP_INCLUDED
