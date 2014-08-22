/*
 * gpcxx/tree/intrusive_node.hpp
 * Date: 2013-10-14
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_TREE_INTRUSIVE_NODE_HPP_INCLUDED
#define GPCXX_TREE_INTRUSIVE_NODE_HPP_INCLUDED

#include <array>
#include <algorithm>
#include <cassert>

    
namespace gpcxx {

namespace detail
{
    template< typename Node >
    struct intrusive_cursor;
}

template< typename Node >
class intrusive_tree;

template< typename Node , size_t Arity = 2 >
class intrusive_node
{
    template< typename T >
    friend class intrusive_tree;
    
    template< typename T >
    friend class detail::intrusive_cursor;
    
public:
    
    static const size_t max_arity = Arity;
    typedef Node node_type;
    typedef node_type* node_pointer;
    typedef node_type const* const_node_pointer;
    typedef detail::intrusive_cursor< node_type > cursor;
    typedef detail::intrusive_cursor< node_type const > const_cursor;    
    typedef std::array< node_pointer , max_arity > children_type;
    
    
    
    
    intrusive_node( node_type *parent = nullptr ) noexcept
    : m_parent( parent ) , m_children()
    {
        clear_children();
    }

    cursor children( size_t i ) noexcept;
    
    const_cursor children( size_t i ) const  noexcept;

    
    size_t max_size( void ) const noexcept
    {
        return max_arity;
    }
    
    
    size_t size( void ) const noexcept
    {
        typename children_type::const_iterator end = std::find( m_children.begin() , m_children.end() , nullptr );
        return std::distance( m_children.begin() , end );
    }

    size_t count_nodes( void ) const noexcept
    {
        size_t count = 1;
        typename children_type::const_iterator iter = m_children.begin();
        typename children_type::const_iterator end = m_children.begin() + size();
        for( ; iter != end ; ++iter )
        {
            count += ( (*iter)->count_nodes() );
        }
        return count;
    }

protected:
    
    void clear_children( void ) noexcept
    {
        std::fill( m_children.begin() , m_children.end() , nullptr );
    }

    
    node_pointer child_node( size_t i ) noexcept
    {
        return m_children[i];
    }
    
    
    const_node_pointer child_node( size_t i ) const noexcept
    {
        return m_children[i];
    }
    
    
    void set_children( size_t i , node_pointer n ) noexcept
    {
        m_children[i] = n;
    }
    
    
    node_pointer parent( void ) noexcept
    {
        return m_parent;
    }
    
    
    const_node_pointer parent( void ) const noexcept
    {
        return m_parent;
    }

    size_t child_index( const_node_pointer child ) const
    {
        return std::distance( m_children.begin() , find_child( child ) );
    }
    
    
    size_t attach_child( node_pointer child ) noexcept
    {
        typename children_type::iterator iter = find_free_child_entry();
        assert( iter != m_children.end() );
        *iter = child;
        return std::distance( m_children.begin() , iter );
    }
    
    
    void attach_parent( node_pointer parent ) noexcept
    {
        m_parent = parent;
    }
    
    void remove_child( node_pointer child ) noexcept
    {
        typename children_type::iterator iter = find_child( child );
        typename children_type::iterator end = m_children.begin() + size();
        
        assert( iter != m_children.end() );
        
        std::copy( iter + 1 , end-- , iter );
        *end = nullptr;
    }
    
    void replace_child( node_pointer old_child , node_pointer new_child ) noexcept
    {
        typename children_type::iterator iter = find_child( old_child );
        assert( iter != m_children.end() );
        *iter = new_child;
    }
    
  
    
protected:
    
    typename children_type::iterator find_free_child_entry( void ) noexcept
    {
        return std::find_if( m_children.begin() , m_children.end() , []( const_node_pointer ptr ) { return ptr == nullptr; } );
    }
    
    typename children_type::const_iterator find_free_child_entry( void ) const noexcept
    {
        return std::find_if( m_children.begin() , m_children.end() , []( const_node_pointer ptr ) { return ptr == nullptr; } );
    }
    
    typename children_type::iterator find_child( const_node_pointer child ) noexcept
    {
        return std::find( m_children.begin() , m_children.end() , child );
    }
    
    typename children_type::const_iterator find_child( const_node_pointer child ) const noexcept
    {
        return std::find( m_children.begin() , m_children.end() , child );
    }
    
    node_pointer m_parent;
    children_type m_children;
};


} // namespace gpcxx

#include <gpcxx/tree/detail/intrusive_cursor.hpp>


namespace gpcxx {

template< typename Node , size_t Arity >
typename intrusive_node< Node , Arity >::cursor intrusive_node< Node , Arity >::children( size_t i ) noexcept
{
    return cursor( m_children[ i ] );
}
    
template< typename Node , size_t Arity >
typename intrusive_node< Node , Arity >::const_cursor intrusive_node< Node , Arity >::children( size_t i ) const noexcept
{
    return const_cursor( m_children[i] );
}



} // namespace gpcxx


#endif // GPCXX_TREE_INTRUSIVE_NODE_HPP_INCLUDED
