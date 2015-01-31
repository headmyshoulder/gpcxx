/*
 * gpcxx/tree/detail/node_base.hpp
 * Date: 2015-01-31
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_TREE_DETAIL_NODE_BASE_HPP_INCLUDED
#define GPCXX_TREE_DETAIL_NODE_BASE_HPP_INCLUDED

#include <array>
#include <algorithm>
#include <cassert>



namespace gpcxx {
namespace detail {
    
template< size_t MaxArity >
class node_base
{

public:

    static const size_t max_arity = MaxArity;
    typedef node_base< max_arity > node_base_type;
    typedef std::array< node_base_type* , max_arity > children_type;
    
    
    node_base( node_base_type *parent = nullptr ) noexcept
    : m_parent( parent ) , m_children()
    {
        std::fill( m_children.begin() , m_children.end() , nullptr );
    }
    
    
    node_base_type* child_node( size_t i ) noexcept
    {
        return m_children[i];
    }
    
    
    node_base_type const* child_node( size_t i ) const noexcept
    {
        return m_children[i];
    }
    
    void set_child_node( size_t i , node_base_type* n ) noexcept
    {
        m_children[i] = n;
    }
    
    
    node_base_type* parent_node( void ) noexcept
    {
        return m_parent;
    }
    
    
    node_base_type const* parent_node( void ) const noexcept
    {
        return m_parent;
    }
    
    void set_parent_node( node_base_type *parent ) noexcept
    {
        m_parent = parent;
    }


    size_t child_index( node_base_type const* child ) const
    {
        return std::distance( m_children.begin() , find_child( child ) );
    }
    
    
    size_t max_size( void ) const noexcept
    {
        return max_arity;
    }
    
    
    size_t size( void ) const noexcept
    {
        typename children_type::const_iterator end = std::find( m_children.begin() , m_children.end() , nullptr );
        return std::distance( m_children.begin() , end );
    }
    
    
    size_t attach_child( node_base_type *child )
    {
        typename children_type::iterator iter = find_free_child_entry();
        assert( iter != m_children.end() );
        *iter = child;
        return std::distance( m_children.begin() , iter );
    }
    
    
    void remove_child( node_base_type *child )
    {
        typename children_type::iterator iter = find_child( child );
        typename children_type::iterator end = m_children.begin() + size();
        
        assert( iter != m_children.end() );
        
        std::copy( iter + 1 , end-- , iter );
        *end = nullptr;
    }
    
    size_t count_nodes( void ) const
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
    
    typename children_type::iterator find_free_child_entry( void )
    {
        return std::find_if( m_children.begin() , m_children.end() , []( node_base_type *ptr ) { return ptr == nullptr; } );
    }
    
    typename children_type::const_iterator find_free_child_entry( void ) const
    {
        return std::find_if( m_children.begin() , m_children.end() , []( node_base_type *ptr ) { return ptr == nullptr; } );
    }
    
    typename children_type::iterator find_child( node_base_type const* child )
    {
        return std::find( m_children.begin() , m_children.end() , child );
    }
    
    typename children_type::const_iterator find_child( node_base_type const* child ) const
    {
        return std::find( m_children.begin() , m_children.end() , child );
    }
    
    node_base_type *m_parent;
    children_type m_children;
};


} // namespace detail
} // namespace gpcxx


#endif // GPCXX_TREE_DETAIL_NODE_BASE_HPP_INCLUDED
