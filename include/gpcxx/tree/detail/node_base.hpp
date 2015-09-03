/*
 * node_base.hpp
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

#include <gpcxx/util/assert.hpp>

#include <memory>
#include <vector>
#include <array>
#include <algorithm>

namespace gpcxx {
namespace detail {

class ascending_node_base
{
public:
    
    ascending_node_base( ascending_node_base* p = nullptr ) noexcept
    : m_parent( p ) {}
    
    ascending_node_base* parent_node( void ) noexcept
    {
        return m_parent;
    }
    
    ascending_node_base const* parent_node( void ) const  noexcept
    {
        return m_parent;
    }
    
    void set_parent_node( ascending_node_base* parent ) noexcept
    {
        m_parent = parent;
    }


    
protected:
    
    ascending_node_base* m_parent;
};


// TODO: A contructor with an allocator argument is needed.
template< typename Allocator = std::allocator< void* > >
class descending_vector_node
{
    using allocator_type = Allocator;
    using node_pointer = descending_vector_node< allocator_type >*;
    using const_node_pointer = descending_vector_node< allocator_type > const*;
    using real_allocator_type = typename allocator_type::template rebind< node_pointer >::other;
    using container_type = std::vector< node_pointer , real_allocator_type >;
    
public:
    
    size_t max_size( void ) const noexcept
    {
        return m_children.max_size();
    }
    
    
    size_t size( void ) const noexcept
    {
        return m_children.size();
    }
    
    
    size_t attach_child( node_pointer child )
    {
        m_children.push_back( child );
        return m_children.size() - 1;
    }
    
    void insert_child( size_t i , node_pointer child )
    {
        m_children.insert( m_children.begin() + i , child );
    }
    
    
    void remove_child( const_node_pointer child )
    {
        auto iter = std::find( m_children.begin() , m_children.end() , child );
        GPCXX_ASSERT( iter != m_children.end() );
        m_children.erase( iter );
    }
    
    bool valid_child( size_t i ) const
    {
        return ( i < m_children.size() );
    }
    
    bool invalid_child( size_t i ) const
    {
        return ( ! valid_child( i ) );
    }
    
    container_type& get_children( void ) 
    {
        return m_children;
    }
    
    container_type const& get_children( void ) const
    {
        return m_children;
    }


    
protected:
    
    container_type m_children;
};

template< size_t MaxArity >
class descending_array_node
{
    static const size_t max_arity = MaxArity;
    using node_pointer = descending_array_node< max_arity >*;
    using const_node_pointer = descending_array_node< max_arity > const*;
    using container_type = std::array< node_pointer , max_arity >;
    
public:
    
    descending_array_node( void )
    : m_children()
    {
        std::fill( m_children.begin() , m_children.end() , nullptr );
    }

    
    size_t max_size( void ) const noexcept
    {
        return max_arity;
    }
    
    
    size_t size( void ) const noexcept
    {
        auto end = std::find( m_children.begin() , m_children.end() , nullptr );
        return std::distance( m_children.begin() , end );
    }
    
    
    size_t attach_child( node_pointer child )
    {
        auto iter = find_free_child_entry();
        GPCXX_ASSERT( iter != m_children.end() );
        *iter = child;
        return std::distance( m_children.begin() , iter );
    }
    
    void insert_child( size_t i , node_pointer child )
    {
        GPCXX_ASSERT( size() < max_size() );
        auto end = m_children.begin() + size() + 1;
        std::rotate( m_children.begin() + i , end - 1 , end );
        m_children[i] = child;
    }
    
    
    void remove_child( const_node_pointer child )
    {
        auto iter = std::find( m_children.begin() , m_children.end() ,  child );
        GPCXX_ASSERT( iter != m_children.end() );
        auto end = m_children.begin() + size();
        
        GPCXX_ASSERT( iter != m_children.end() );
        
        std::copy( iter + 1 , end-- , iter );
        *end = nullptr;
    }
    
    bool valid_child( size_t i ) const
    {
        return ! invalid_child( i );
    }
    
    bool invalid_child( size_t i ) const
    {
        GPCXX_ASSERT( i < max_arity );
        return ( m_children[i] == nullptr );
    }
    
    container_type& get_children( void ) 
    {
        return m_children;
    }
    
    container_type const& get_children( void ) const
    {
        return m_children;
    }
    

    
private:
    
    typename container_type::iterator find_free_child_entry( void )
    {
        return std::find_if( m_children.begin() , m_children.end() , []( auto* ptr ) { return ptr == nullptr; } );
    }
    
    typename container_type::const_iterator find_free_child_entry( void ) const
    {
        return std::find_if( m_children.begin() , m_children.end() , []( auto* ptr ) { return ptr == nullptr; } );
    }


    
protected:
    
    container_type m_children;
};

template< typename DescendingNode >
struct node_base : public DescendingNode , ascending_node_base
{
    // types
    
    using self_type = node_base< DescendingNode >;
    using node_base_pointer = self_type*;
    using const_node_base_pointer = self_type const*;
    
    
    // construct
    node_base( node_base* parent = nullptr )
    : DescendingNode() , ascending_node_base( parent )
    { }
    
    // HIER GEHTS WEITER
    node_base_pointer child_node( size_t i ) noexcept
    {
        return static_cast< node_base_pointer >( this->m_children[i] );
    }
    
    
    const_node_base_pointer child_node( size_t i ) const noexcept
    {
        return static_cast< const_node_base_pointer >( this->m_children[i] );
    }
    
    void set_child_node( size_t i , node_base_pointer n ) noexcept
    {
        this->m_children[i] = n;
    }
    
    
    node_base_pointer parent_node( void ) noexcept
    {
        return static_cast< node_base_pointer >( m_parent );
    }
    
    
    const_node_base_pointer parent_node( void ) const noexcept
    {
        return static_cast< const_node_base_pointer >( m_parent );
    }
    
    void set_parent_node( node_base_pointer parent ) noexcept
    {
        m_parent = parent;
    }


    size_t child_index( const_node_base_pointer child ) const
    {
        return std::distance( this->m_children.begin() , find_child( child ) );
    }
    
    

    size_t count_nodes( void ) const noexcept
    {
        size_t count = 1;
        auto iter = this->m_children.begin();
        auto last = this->m_children.begin() + this->size();
        for( ; iter != last ; )
        {
            count += ( static_cast< const_node_base_pointer >(*iter++)->count_nodes() );
        }
        return count;
    }
    
    size_t height( void ) const noexcept
    {
        size_t h = 0;
        auto iter = this->m_children.begin();
        auto last = this->m_children.begin() + this->size();
        for( ; iter != last ; )
            h = std::max( h , static_cast< const_node_base_pointer >(*iter++)->height() );
        return 1 + h;
    }
    
    size_t level( void ) const noexcept
    {
        if( m_parent == nullptr ) return 0;
        return 1 + parent_node()->level();
    }
    
protected:
    
    auto find_child( const_node_base_pointer child )
    {
        return std::find( this->m_children.begin() , this->m_children.end() , child );
    }
    
    auto find_child( const_node_base_pointer child ) const
    {
        return std::find( this->m_children.begin() , this->m_children.end() , child );
    }

};



} // namespace detail
} // namespace gpcxx



#endif // GPCXX_TREE_DETAIL_NODE_BASE_HPP_INCLUDED
