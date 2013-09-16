/*
 * gpcxx/tree/detail/recursive_node.hpp
 * Date: 2013-09-13
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_TREE_DETAIL_RECURSIVE_NODE_HPP_INCLUDED
#define GPCXX_TREE_DETAIL_RECURSIVE_NODE_HPP_INCLUDED

#include <boost/variant.hpp>

#include <array>
#include <algorithm>
#include <cassert>

namespace gpcxx {
namespace detail {

    
template< typename T , size_t MaxArity = 2 > struct recursive_node;
struct nil {};



template< typename T >
struct recursive_node_base
{
    typedef T value_type;
    typedef boost::variant<
        nil 
      , boost::recursive_wrapper< recursive_node< value_type > >
        > type;
        
    typedef recursive_node_base< value_type > node_base_type;
    typedef recursive_node< value_type > node_type;

    type m_node = { nil() };
    node_base_type* m_parent = { nullptr };
    
    size_t remove_child( node_base_type *ptr ) noexcept
    {
        assert( m_node.which() != 0 );
        return boost::get< node_type >( m_node ).remove_child( ptr );
    }
    
    size_t num_children( void ) const noexcept;
};



template< typename T , size_t MaxArity >
struct recursive_node
{
    static const size_t max_arity = MaxArity;
    typedef T value_type;
    typedef recursive_node< value_type , max_arity > node_type;
    typedef recursive_node_base< T > node_base_type;
    typedef node_base_type* node_base_pointer;
    typedef node_type* node_pointer;
    typedef node_type& node_reference;
    typedef value_type& reference;
    typedef value_type const& const_reference;
    typedef value_type* pointer;
    
    typedef std::array< node_base_type , max_arity > children_type;
    typedef typename children_type::iterator children_iterator;
    typedef typename children_type::const_iterator const_children_iterator;

    recursive_node( value_type const &value ) : m_value( value ) , m_children{ node_base_type() , node_base_type() }  { }
    recursive_node( value_type && value ) : m_value( std::move( value ) ) , m_children{ node_base_type() , node_base_type() } { }

    value_type m_value = {};
    children_type m_children = { node_base_type() , node_base_type() };

    node_base_pointer insert_below( value_type const &value )
    {
        children_iterator iter = find_free_child_entry();
        assert( iter != m_children.end() );
        iter->m_node = node_type( value );
        return iter;
    }

    size_t max_size( void ) const noexcept
    {
        return max_arity;
    }

    size_t size( void ) const noexcept
    {
        const_children_iterator end = std::find_if( m_children.begin() , m_children.end() , []( node_base_type const& n ) -> bool { return n.m_node.which() == 0 ; } );
        return std::distance( m_children.begin() , end );
    }
    
    size_t num_nodes( void ) const noexcept
    {
        size_t s = 1;
        for( size_t i=0 ; i<max_size() ; ++i )
            s += m_children[i].num_children();
        return s;
    }
   
    children_iterator find_free_child_entry( void ) noexcept
    {
        return std::find_if( m_children.begin() , m_children.end() , []( node_base_type &n ) -> bool { return n.m_node.which() == 0 ; } );
    }
    
    const_children_iterator find_free_child_entry( void ) const  noexcept
    {
        return std::find_if( m_children.begin() , m_children.end() , []( node_base_type const& n ) -> bool { return n.m_node.which() == 0 ; } );
    }
    
    size_t remove_child( node_base_pointer ptr )
    {
        assert( ptr != nullptr );

        size_t size = ptr->num_children();

        children_iterator iter = find_child( ptr );
        children_iterator end = find_free_child_entry();

        assert( iter != m_children.end() );

        std::copy( iter + 1 , end-- , iter );
        iter->m_node = nil();

        return size;
    }
    
    size_t height( void ) const noexcept
    {
        size_t h = 0;
        const_children_iterator end = find_free_child_entry();
        for( const_children_iterator s = m_children.begin() ; s != end ; ++s )
        {
            assert( s->m_node.which() != 0 ); 
            node_type const& n = boost::get< node_type >( s->m_node );
            h = std::max( h , n.height() );
        }
        return 1 + h;
    }
    
    const_children_iterator find_child( node_base_pointer ptr ) const noexcept
    {
        return std::find_if( m_children.begin() , m_children.end() , [ptr]( node_base_type const& n ) -> bool { return ( (&n) == ptr ); } );
    }

    children_iterator find_child( node_base_pointer ptr ) noexcept
    {
        return std::find_if( m_children.begin() , m_children.end() , [ptr]( node_base_type const& n ) -> bool { return ( (&n) == ptr ); } );
    }
};

template< typename T >
size_t recursive_node_base< T >::num_children( void ) const noexcept
{
    return m_node.which() == 0 ? 0 : boost::get< node_type >( m_node ).num_nodes();
}



} // namespace detail
} // namespace gpcxx


#endif // GPCXX_TREE_DETAIL_RECURSIVE_NODE_HPP_INCLUDED
