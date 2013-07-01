/*
  gp/tree/basic_tree.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GP_TREE_BASIC_TREE_HPP_DEFINED
#define GP_TREE_BASIC_TREE_HPP_DEFINED

#include <gp/tree/detail/node.hpp>
#include <gp/tree/detail/cursor.hpp>

#include <memory>
#include <cstddef>

#include <vector>

namespace gp {


template< typename T , typename Allocator = std::allocator< T > >
class basic_tree
{
    typedef basic_tree< T , Allocator > self_type;

    static const size_t max_arity = 2;
    typedef detail::node< T , max_arity >                           node_type;
    typedef node_type*                                              node_pointer;
    typedef typename node_type::node_base_type                      node_base_type;
    typedef node_base_type*                                         node_base_pointer;
    typedef typename Allocator::template rebind< node_type >::other node_allocator_type;
    
public:
  
    // types:
    typedef T                                                               value_type;
    typedef Allocator                                                       allocator_type;
    typedef value_type&                                                     reference;
    typedef const value_type&                                               const_reference;
    typedef detail::node_cursor< node_type >                                cursor;
    typedef detail::node_cursor< node_type const >                          const_cursor;
    typedef size_t                                                          size_type;
    typedef ptrdiff_t                                                       difference_type;
    typedef typename std::allocator_traits< allocator_type >::pointer       pointer;
    typedef typename std::allocator_traits< allocator_type >::const_pointer const_pointer;

    template< typename U > struct rebind
    {
        typedef basic_tree< U, typename Allocator::template rebind<U>::other > other;
    };

    
    
    
    // construct:
    explicit basic_tree( allocator_type const& allocator = allocator_type() )
    : m_node_allocator( allocator ) , m_header() 
    {
    }
    
    template< typename InputCursor >
    basic_tree( InputCursor subtree , allocator_type const& allocator = allocator_type() )
    : basic_tree( allocator )
    {
        insert_below( root() , subtree );
    }
    
    basic_tree( basic_tree const& tree )
    : basic_tree( tree.get_allocator() )
    {
        if( !tree.empty() )
            insert_below( root() , tree.root() );
    }
    
    basic_tree( basic_tree const& tree , allocator_type const& allocator = allocator_type() )
    : basic_tree( allocator )
    {
        if( !tree.empty() )
            insert_below( root() , tree.root() );
    }
    
    basic_tree( basic_tree&& tree )
    : m_node_allocator( tree.get_allocator() , 
    
    basic_tree( basic_tree&& tree , allocator_type const& allocator = allocator_type() )
    : basic_tree( allocator )
    {
        m_header.children().swap( tree.m_header.children() );
    }

    
    
    
    // destroy:
    ~basic_tree( void )
    {
        clear();
    }

    
    
    
    // copy:
    basic_tree& operator=( basic_tree const& tree )
    {
        // TODO : implement
        return *this;
    }
    
    
    basic_tree& operator=( basic_tree&& tree )
    {
        // TODO : implement
        return *this;
    }
    
    
    
    
    
    
    template< typename InputCursor >
    void assign( InputCursor subtree )
    {
        // TODO : implement
    }
    
    allocator_type get_allocator( void ) const noexcept
    {
        return allocator_type( m_node_allocator );
    }
 
    
    
    
    // cursors:
    cursor root() noexcept
    {
        return cursor( &m_header , 0 );
    }
    
    const_cursor root() const noexcept
    {
        return const_cursor( &m_header , 0 );
    }
    
    const_cursor croot() const noexcept
    {
        return const_cursor( &m_header , 0 );
    }
    
    cursor shoot() noexcept
    {
        return cursor( &m_header , 1 );
    }
    
    const_cursor shoot() const noexcept
    {
        return const_cursor( &m_header , 1 );
    }
    
    const_cursor cshoot() const noexcept
    {
        return const_cursor( &m_header , 1 );
    }
    
    
    

    // capacity:
    bool empty( void ) const noexcept
    {
        // TODO : implement
    }
        
    size_type size( void ) const noexcept
    {
        // TODO : implement
    }
    
    size_type max_size( void ) const noexcept
    {
        return size_type( -1 );
    }
 
 
 
 
    // modifiers:
    cursor insert_below( const_cursor position , const value_type& val )
    {
        node_pointer new_node = m_node_allocator.allocate( 1 );
        m_node_allocator.construct( new_node , val );
        
        node_base_pointer parent_node = position.parent_node();


//         node_pointer p_node = m_node_alloc.allocate( 1 , nullptr );
//         *p_node = node_type( val );
//         detail::attach(pos.parent_node(), pos.child_node(), p_node, p_node->m_children[pos.index()]);
//         return pos; 
    }
    
    cursor insert_below( const_cursor position , value_type &&val )
    {
        // TODO : implement
    }

    template< typename InputCursor >
    cursor insert_below( const_cursor position , InputCursor subtree )
    {
        // TODO : implement
    }


    void swap( basic_tree& other )
    {
        // TODO : implement
    }
    
    void clear( cursor position )
    {
        // TODO : implement
    }

    void clear( void )
    {
        // TODO : implement
    }

private:
    
    node_allocator_type m_node_allocator;
    node_base_type m_header;
};



// compare algorithms:
template< typename T , typename Allocator >
bool operator==( basic_tree< T , Allocator > const& x , basic_tree< T , Allocator > const& y );
template< typename T, typename Allocator >
bool operator!=( basic_tree< T , Allocator > const& x , basic_tree< T , Allocator > const& y );

// specialized algorithms:
template< typename T , typename Allocator >
void swap( basic_tree< T , Allocator >& x , basic_tree< T , Allocator >& y );


} // namespace gp






#endif // GP_TREE_BASIC_TREE_HPP_DEFINED
