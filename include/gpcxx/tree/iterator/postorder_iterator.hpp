/*
 * gpcxx/tree/iterator/postorder_iterator.hpp
 * Date: 2015-01-22
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_TREE_ITERATOR_POSTORDER_ITERATOR_HPP_INCLUDED
#define GPCXX_TREE_ITERATOR_POSTORDER_ITERATOR_HPP_INCLUDED

#include <gpcxx/tree/iterator/iterator_base.hpp>
#include <gpcxx/tree/cursor_traits.hpp>

#include <type_traits>

namespace gpcxx {


struct postorder_policy
{
    template< typename Cursor >
    static bool successor( Cursor &c )
    {
        if( c.is_root() )
        {
            ++c;
            return true;
        }
        else
        {
            Cursor d = c;
            ++d;
            if( c.parent().end() != d )
            {
                c = d;
                return first( c );
            }
            else
            {
                c = c.parent();
                return true;
            }
        }
    }
    
    template< typename Cursor >
    static bool predecessor( Cursor &c )
    {
        if( c.is_shoot() )
        {
            --c;
            return true;
        }
        if( !c.empty() )
        {
            c = c.children( c.size() - 1 );
            return true;
        }
        else
        {
            while( true )
            {
                if( c.parent().begin() == c )
                {
                    c = c.parent();
                }
                else
                {
                    --c;
                    return true;
                }
            }
        }
        
    }
    
    template< typename Cursor >
    static bool first( Cursor &c )
    {
        while( !c.empty() )
        {
            c = c.children( 0 );
        }
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
// postorder iterator type
//
template< typename Cursor >
using postorder_iterator = iterator_base< Cursor , postorder_policy >;



//
// iterator from cursor
//
template< typename Cursor , typename Enable = typename std::enable_if< is_cursor< Cursor >::value >::type >
auto begin_postorder( Cursor c )
{
    postorder_policy::first( c );
    return postorder_iterator< Cursor > { c };
}

template< typename Cursor , typename Enable = typename std::enable_if< is_cursor< Cursor >::value >::type >
auto end_postorder( Cursor c )
{
    postorder_policy::last( c );
    return postorder_iterator< Cursor > { c };
}



//
// iterator from tree
//
template< typename Tree >
auto begin_postorder( Tree& t )
{
    return begin_postorder( t.root() );
}

template< typename Tree >
auto end_postorder( Tree& t )
{
    return end_postorder( t.root() );
}

template< typename Tree >
auto cbegin_postorder( Tree const& t )
{
    return begin_postorder( t.croot() );
}

template< typename Tree >
auto cend_postorder( Tree const& t )
{
    return end_postorder( t.croot() );
}




} // namespace gpcxx


#endif // GPCXX_TREE_ITERATOR_POSTORDER_ITERATOR_HPP_INCLUDED
