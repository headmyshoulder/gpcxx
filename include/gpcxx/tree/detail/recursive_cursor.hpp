/*
 * gpcxx/tree/detail/recursive_cursor.hpp
 * Date: 2013-09-13
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_TREE_DETAIL_RECURSIVE_CURSOR_HPP_INCLUDED
#define GPCXX_TREE_DETAIL_RECURSIVE_CURSOR_HPP_INCLUDED


#include <gpcxx/tree/detail/node_helpers.hpp>
#include <gpcxx/tree/cursor_traits.hpp>

#include <boost/iterator/iterator_facade.hpp>
#include <boost/iterator/iterator_concepts.hpp>


namespace gpcxx {

template< typename T > class recursive_tree;

namespace detail {


template< typename Node >
class recursive_node_cursor 
{
    
    template< typename U > friend class recursive_node_cursor;
    template< typename T > friend class recursive_tree;

    
    typedef Node node_type;
    typedef node_type* node_pointer;
    typedef typename std::remove_const< Node >::type real_node_type;    
    
    typedef typename node_base_getter< Node >::type node_base;
    typedef node_base* node_base_pointer;
    typedef typename std::remove_const< node_base >::type real_node_base;
    typedef real_node_base* real_node_base_pointer;
    typedef real_node_base const* const_real_node_base_pointer;
    

   
public:
    
    // requirements from container concept via cursor concept
    // typedef typename base_type::reference const_reference;
    typedef size_t size_type;
    typedef typename node_value_getter< Node >::type value_type;
    typedef value_type & reference;
    typedef value_type * pointer;
    
    // requirements from cursor concept
    typedef recursive_node_cursor< node_type > cursor;
    typedef recursive_node_cursor< real_node_type const > const_cursor;

    // typedef ascending_random_access_cursor_tag type;
    
private:
    
    template< typename OtherNode >
    struct other_node_enabler : public std::enable_if< std::is_convertible< typename OtherNode::value_type* , value_type* >::value >
    {
    };
    
    
public:

    
    recursive_node_cursor( node_base_pointer node = nullptr )
    : m_node( node ) { }

   template< typename OtherNode , typename Enabler = typename other_node_enabler< OtherNode >::type >
    recursive_node_cursor( recursive_node_cursor< OtherNode > const& other )
    : m_node( other.m_node ) {}
    
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


    cursor parent( void )
    {
        assert( m_node->parent() != nullptr );
        return cursor( m_node->parent() );
    }

    const_cursor parent( void ) const
    {
        assert( m_node->parent() != nullptr );
        return cursor( m_node->parent() );
    }

    cursor children( size_type i )
    {
        return cursor( & ( boost::get< node_type >( m_node->data() ).m_children[ i ] ) );
    }

    const_cursor children( size_type i ) const
    {
        return const_cursor( & ( boost::get< node_type >( m_node->data() ).m_children[ i ] ) );
    }

    size_type size( void ) const noexcept
    {
        return ( m_node->data().which() == 0 ) ? 0 : boost::get< node_type >( m_node->data() ).size();
    }

    size_type max_size( void ) const noexcept
    {
        return m_node->max_size();
    }

    size_type height( void ) const noexcept
    {
        if( m_node->data().which() == 0 ) return 0;
        node_type const& n = boost::get< node_type >( m_node->data() );
        return n.height();
    }

    size_type level( void ) const noexcept
    {
        if( m_node->data().which() == 0 ) return 0;
        if( m_node->parent() == nullptr ) return 0;
        return 1 + parent().level();
    }
    
    reference operator*( void ) const
    {
        assert( m_node->data().which() != 0 );
        return boost::get< node_type >( m_node->data() ).m_value;
    }
    
    pointer operator->( void ) const
    {
        assert( m_node->data().which() != 0 );
        return &( boost::get< node_type >( m_node->data() ).m_value );
    }



public:

    node_base_pointer parent_node( void ) noexcept
    {
        return m_node->parent();
    }

    const node_base_pointer parent_node( void ) const noexcept
    {
        return m_node->parent();
    }

    node_base_pointer node( void ) noexcept
    {
        return m_node;
    }

    const node_base_pointer node( void ) const noexcept
    {
        return m_node;
    }
    
    bool equal( recursive_node_cursor const& other) const
    {
        return ( other.m_node == m_node );
    }



private:


    reference dereference() const
    {
        assert( m_node->data().which() != 0 );
        return boost::get< node_type >( m_node->data() ).m_value;
    }
    
    
    node_base_pointer m_node;
};

template< typename Node >
bool operator==( recursive_node_cursor< Node > const &c1 , recursive_node_cursor< Node > const &c2 )
{
    return c1.equal( c2 );
}

// template< typename Node1 , typename Node2 >
// bool cursor_comp( recursive_node_cursor< Node1 > const& c1 , recursive_node_cursor< Node2 > const& c2 )
// {
//     if( c1.size() != c2.size() ) return false;
//     if( *c1 != *c2 ) return false;
//     for( size_t i=0 ; i<c1.size() ; ++i )
//     {
//         if( !cursor_comp( c1.children(i) , c2.children(i) ) ) return false;
//     }
//     return true;
// }


} // namespace detail

template< typename Node >
struct is_cursor< detail::recursive_node_cursor< Node > > : public std::true_type { };



} // namespace gpcxx





#endif // GPCXX_TREE_DETAIL_RECURSIVE_CURSOR_HPP_INCLUDED
