/*
  gpcxx/tree/detail/basic_node_fast.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GPCXX_TREE_DETAIL_BASIC_NODE_FAST_HPP_DEFINED
#define GPCXX_TREE_DETAIL_BASIC_NODE_FAST_HPP_DEFINED

#include <array>
#include <algorithm>
#include <cassert>


namespace gpcxx {

namespace detail {

    
    
template< typename T , size_t MaxArity >
class basic_node_fast
{

public:

    static const size_t max_arity = MaxArity;
    typedef T value_type;
    
    typedef basic_node_fast< value_type , max_arity > node_type;
    typedef node_type* node_pointer;
    typedef node_type const* const_node_pointer;
    typedef node_type& node_reference;
    typedef std::array< node_pointer , max_arity > children_type;
    
    typedef value_type& reference;
    typedef value_type const& const_reference;
    typedef value_type* pointer;
    
    basic_node_fast( value_type data , node_pointer parent = nullptr )
    : m_data( std::move( data ) ) , m_children() , m_parent( parent )
    {
        std::fill( m_children.begin() , m_children.end() , nullptr );
    }

    reference operator*() { return m_data; } 

    const_reference operator*() const { return m_data; } 
    
    
    node_pointer children( size_t i ) noexcept
    {
        return m_children[i];
    }
    
    
    const_node_pointer children( size_t i ) const noexcept
    {
        return m_children[i];
    }
    
    void set_children( size_t i , node_pointer n )
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
    
    
    size_t max_size( void ) const noexcept
    {
        return max_arity;
    }
    
    
    size_t size( void ) const noexcept
    {
        typename children_type::const_iterator end = std::find( m_children.begin() , m_children.end() , nullptr );
        return std::distance( m_children.begin() , end );
    }
    
    
    size_t attach_child( node_pointer child )
    {
        typename children_type::iterator iter = find_free_child_entry();
        assert( iter != m_children.end() );
        *iter = child;
        return std::distance( m_children.begin() , iter );
    }
    
    
    void attach_parent( node_pointer parent )
    {
        m_parent = parent;
    }
    
    void remove_child( node_pointer child )
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
        return std::find_if( m_children.begin() , m_children.end() , []( node_pointer ptr ) { return ptr == nullptr; } );
    }
    
    typename children_type::const_iterator find_free_child_entry( void ) const
    {
        return std::find_if( m_children.begin() , m_children.end() , []( node_pointer ptr ) { return ptr == nullptr; } );
    }
    
    typename children_type::iterator find_child( const_node_pointer child )
    {
        return std::find( m_children.begin() , m_children.end() , child );
    }
    
    typename children_type::const_iterator find_child( const_node_pointer child ) const
    {
        return std::find( m_children.begin() , m_children.end() , child );
    }
    
    value_type m_data;
    children_type m_children;
    node_pointer m_parent;
};







} // namespace detail
} // namespace gpcxx


#endif // GPCXX_TREE_DETAIL_BASIC_NODE_FAST_HPP_DEFINED
