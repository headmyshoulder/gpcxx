/*
 * gpcxx/tree/detail/intrusive_cursor.hpp
 * Date: 2013-10-15
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_TREE_DETAIL_INTRUSIVE_CURSOR_HPP_INCLUDED
#define GPCXX_TREE_DETAIL_INTRUSIVE_CURSOR_HPP_INCLUDED


namespace gpcxx {
namespace detail {

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


} // namespace detail
} // namespace gpcxx


#endif // GPCXX_TREE_DETAIL_INTRUSIVE_CURSOR_HPP_INCLUDED
