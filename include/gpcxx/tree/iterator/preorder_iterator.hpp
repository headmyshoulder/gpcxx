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
#include <gpcxx/tree/cursor_traits.hpp>

#include <type_traits>



namespace gpcxx {

    

struct preorder_policy
{
    template< typename Cursor >
    static bool successor( Cursor &c )
    {
        if( c.empty() )
        {
            while( !c.is_root() )
            {
                Cursor d = c;
                ++d;
                if( d != c.parent().end() )
                {
                    c = d;
                    return true;
                }
                c = c.parent();
            }
            ++c;
            return true;
        }
        else
        {
            c = c.children( 0 );
            return true;
        }
    }
    
    template< typename Cursor >
    static bool predecessor( Cursor &c )
    {
        if( !c.is_shoot() && ( c.parent().begin() == c ) )
        {
            c = c.parent();
            return true;
        }
        else
        {
            --c;
            while( !c.empty() )
            {
                c = c.children( c.size() - 1 );
            }
            return true;
        }
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

//
// preorder iterator type
//
template< typename Cursor >
using preorder_iterator = iterator_base< Cursor , preorder_policy >;


//
// iterator from cursor
//
template< typename Cursor , typename Enable = typename std::enable_if< is_cursor< Cursor >::value >::type >
auto begin_preorder( Cursor c )
{
    preorder_policy::first( c );
    return preorder_iterator< Cursor > { c };
}

template< typename Cursor , typename Enable = typename std::enable_if< is_cursor< Cursor >::value >::type >
auto end_preorder( Cursor c )
{
    preorder_policy::last( c );
    return preorder_iterator< Cursor > { c };
}



//
// iterator from tree
//
template< typename Tree >
auto begin_preorder( Tree& t )
{
    return begin_preorder( t.root() );
}

template< typename Tree >
auto end_preorder( Tree& t )
{
    return end_preorder( t.root() );
}

template< typename Tree >
auto cbegin_preorder( Tree const& t )
{
    return begin_preorder( t.croot() );
}

template< typename Tree >
auto cend_preorder( Tree const& t )
{
    return end_preorder( t.croot() );
}





} // namespace gpcxx


#endif // GPCXX_TREE_ITERATOR_PREORDER_ITERATOR_HPP_INCLUDED
