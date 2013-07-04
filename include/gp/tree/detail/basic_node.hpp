/*
  gp/tree/detail/basic_node.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GP_TREE_DETAIL_BASIC_NODE_HPP_DEFINED
#define GP_TREE_DETAIL_BASIC_NODE_HPP_DEFINED

#include <array>
#include <algorithm>
#include <cassert>


namespace gp {
namespace detail {

    
    
template< size_t MaxArity >
class basic_node_base
{
public:

    static const size_t max_arity = MaxArity;
    typedef basic_node_base< max_arity > node_base_type;
    typedef std::array< node_base_type* , max_arity > children_type;
    
    
    basic_node_base( node_base_type *parent = nullptr ) noexcept
    : m_parent( parent ) , m_children()
    {
        for( size_t i=0 ; i<max_arity ; ++i ) m_children[i] = nullptr;
    }
    
    
    node_base_type* children( size_t i ) noexcept
    {
        return m_children[i];
    }
    
    
    node_base_type const* children( size_t i ) const noexcept
    {
        return m_children[i];
    }
    
    
    node_base_type* parent( void ) noexcept
    {
        return m_parent;
    }
    
    
    node_base_type const* parent( void ) const noexcept
    {
        return m_parent;
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
    
    
    void attach_parent( node_base_type *parent )
    {
        m_parent = parent;
    }
    
    void remove_child( node_base_type *child )
    {
        typename children_type::iterator iter = find_child( child );
        typename children_type::iterator end = m_children.begin() + size();
        
        assert( iter != m_children.end() );
        
        std::copy( iter + 1 , end-- , iter );
        *end = nullptr;
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




template< typename T , size_t MaxArity >
class basic_node : public basic_node_base< MaxArity >
{
public:
    
    typedef T value_type;
    typedef basic_node< value_type , MaxArity > node_type;
    typedef basic_node_base< MaxArity > node_base_type;
    typedef node_type* node_pointer;
    typedef node_type& node_reference;
    
    typedef value_type& reference;
    typedef value_type const& const_reference;
    typedef value_type* pointer;

    reference operator*() { return m_data; } 

    const_reference operator*() const { return m_data; } 
    
    basic_node( value_type data , node_base_type *parent = nullptr ) : node_base_type( parent ) ,  m_data( std::move( data ) ) {}
    
    
 
private:

    value_type m_data;
};





} // namespace detail
} // namespace gp


#endif // GP_TREE_DETAIL_BASIC_NODE_HPP_DEFINED
