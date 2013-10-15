/*
 * gpcxx/tree/detail/intrusive_node.h
 * Date: 2013-10-14
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_TREE_DETAIL_INTRUSIVE_NODE_H_INCLUDED
#define GPCXX_TREE_DETAIL_INTRUSIVE_NODE_H_INCLUDED

#include <array>
#include <algorithm>
#include <cassert>

namespace gpcxx {

template< typename Node >
struct intrusive_cursor;


template< typename Node >
class intrusive_node
{
public:
    
    static const size_t max_arity = 2;
    typedef Node node_type;
    typedef node_type* node_pointer;
    typedef node_type const* const_node_pointer;
    typedef intrusive_cursor< node_type > cursor;
    typedef intrusive_cursor< node_type const > const_cursor;    
    typedef std::array< node_pointer , max_arity > children_type;
    
    
    
    
    intrusive_node( node_type *parent = nullptr ) noexcept
    : m_parent( parent ) , m_children()
    {
        clear_children();
    }
    
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
    
    cursor children( size_t i ) noexcept;
    
    const_cursor children( size_t i ) const  noexcept;
    
    void set_children( size_t i , node_type* n ) noexcept
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

    size_t child_index( node_type const* child ) const
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
    
    
    size_t attach_child( node_type *child ) noexcept
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



template< class Node >
class intrusive_cursor
{
public:
    
    typedef Node node_type;
    typedef intrusive_cursor< node_type > cursor;
    typedef intrusive_cursor< node_type const > const_cursor;
    typedef node_type* node_pointer;
    typedef node_type const* const_node_pointer;
    typedef node_type& node_reference;
    typedef node_type const& const_node_reference;
    typedef size_t size_type;


    intrusive_cursor( node_pointer node = nullptr ) noexcept
    : m_node( node ) { }

//    template< typename OtherNode , typename Enabler = typename other_node_enabler< OtherNode >::type >
//     basic_node_cursor( basic_node_cursor< OtherNode > const& other )
//     : m_node( other.m_node ) , m_pos( other.m_pos ) {}
    
    // emtpy()
    // swap()
    // operator==
    // operator!=
    
    
     

    // a.begin() // cursor
    // a.end() // cursor
    // a.cbegin() const_cursor
    // a.cend() const_cursor
    // a.parity() size_type   (std::distance(b.begin(), a) if b is a's parent.)
    // a.parent() const_cursor / cursor
    
    
    
//     cursor begin( void )
//     {
//         return cursor( child_node( 0 ) );
//     }
// 
//     const_cursor begin( void ) const
//     {
//         return cbegin();
//     }
//     
//     const_cursor cbegin( void ) const
//     {
//         return const_cursor( child_node( 0 ) );
//     }
//     
//     cursor end( void )
//     {
//         return cursor( child_node( node()->size() ) );
//     }
//     
//     const_cursor end( void ) const
//     {
//         return cend();
//     }
//     
//     const_cursor cend( void ) const
//     {
//         return const_cursor( child_node( node()->size() ) );
//     }
    
    
    
    cursor parent( void )
    {
        return cursor( parent_node() );
    }

    const_cursor parent( void ) const
    {
        return const_cursor( parent_node() );
    }


    cursor children( size_type i )
    {
        return cursor( child_node( i ) );
    }
    
    const_cursor children( size_type i ) const
    {
        return const_cursor( child_node( i ) );
    }
    
    
    
    size_type size( void ) const noexcept
    {
        return m_node->size();
    }
    
    size_type max_size( void ) const noexcept
    {
        return m_node->max_size();
    }
    
    size_type height( void ) const noexcept
    {
        if( node() == nullptr ) return 0;
        
        size_type h = 0;
        for( size_t i=0 ; i<node()->size() ; ++i )
            h = std::max( h , node()->children(i).height() );
        return 1 + h;

    }

    size_type level( void ) const noexcept
    {
        if( node() == nullptr ) return 0;
        if( parent_node() == nullptr ) return 0;
        return 1 + parent().level();
    }

    node_reference operator*( void )
    {
        return *m_node;
    }
    
    const_node_reference operator*( void ) const
    {
        return *m_node;
    }
    
    node_pointer operator->( void )
    {
        return m_node;
    }
    
    const_node_pointer operator->( void ) const
    {
        return m_node;
    }

    

public:

    node_pointer parent_node( void ) noexcept
    {
        return m_node->parent();
    }

    const_node_pointer parent_node( void ) const noexcept
    {
        return m_node->parent();
    }

    node_pointer node( void ) noexcept
    {
        return m_node;
    }

    const_node_pointer node( void ) const noexcept
    {
        return m_node;
    }
    
    node_pointer child_node( size_t i ) noexcept
    {
        return m_node->child_node( i );
    }
    
    const_node_pointer child_node( size_t i ) const noexcept
    {
        return m_node->child_node( i );
    }


private:

//     void increment( void )
//     {
//         ++m_pos;
//     }
//     
//     void decrement( void )
//     {
//         --m_pos;
//     }
//     
//     void advance( typename base_type::difference_type n )
//     {
//         m_pos += n;
//     }
//     
//     typename base_type::difference_type distance_to( basic_node_cursor const& other ) const
//     {
//         // TODO: implement
//     }
// 
//     bool equal( basic_node_cursor const& other) const
//     {
//         return ( other.m_node == m_node ) && ( other.m_pos == m_pos );
//     }
// 
//     typename base_type::reference dereference() const
//     {
//         return **static_cast< node_pointer >( m_node->children( m_pos ) );
//     }
    
    node_pointer m_node;
};


template< typename Node >
typename intrusive_node< Node >::cursor intrusive_node< Node >::children( size_t i ) noexcept
{
    return cursor( m_children[ i ] );
}
    
template< typename Node >
typename intrusive_node< Node >::const_cursor intrusive_node< Node >::children( size_t i ) const noexcept
{
    return const_cursor( m_children[i] );
}



} // namespace gpcxx


#endif // GPCXX_TREE_DETAIL_INTRUSIVE_NODE_H_INCLUDED
