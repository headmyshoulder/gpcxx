/*
  gpcxx/tree/intrusive_tree.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GPCXX_TREE_INTRUSIVE_TREE_HPP_DEFINED
#define GPCXX_TREE_INTRUSIVE_TREE_HPP_DEFINED

#include <gpcxx/tree/intrusive_node.hpp>

#include <iostream>
#include <functional>
#include <array>
#include <cmath>
#include <cassert>
#include <algorithm>




namespace gpcxx {



template< typename Node >
class intrusive_tree
{
public:
    
    typedef Node node_type;
    typedef node_type* node_pointer;
    typedef detail::intrusive_cursor< node_type > cursor;
    typedef detail::intrusive_cursor< node_type const > const_cursor;
    typedef size_t size_type;
    
    //
    // construct:
    //
    intrusive_tree( void )
    : m_root( nullptr ) , m_size( 0 ) { }
    
    template< typename InputCursor >
    intrusive_tree( InputCursor subtree )
    : intrusive_tree()
    {
        insert_below( root() , subtree );
    }
    
    intrusive_tree( intrusive_tree const& tree )
    : intrusive_tree()
    {
        if( !tree.empty() )
            insert_below( root() , tree.root() );
    }
    


    
    //
    // destroy
    //
    ~intrusive_tree( void )
    {
        clear();
    }

    
    
    
    
    //
    // copy
    //
    intrusive_tree& operator=( intrusive_tree const& tree )
    {
        if( &tree != this )
        {
            clear();
            if( !tree.empty() )
                insert_below( root() , tree.root() );
        }
        return *this;
    }

    
    
    
    
    size_type size( void ) const
    {
        return m_size;
    }
    
    bool empty( void ) const
    {
        return ( size() == 0 );
    }
    
    cursor root( void )
    {
        return cursor( m_root );
    }
    
    const_cursor root( void ) const
    {
        return const_cursor( m_root );
    }
    
    cursor insert_below( cursor c , node_type const& n )
    {
        ++m_size;
        cursor ret;
        if( c.node() == nullptr )
        {
            m_root = new node_type( n );
            m_root->clear_children();
            ret = cursor( m_root );
        }
        else
        {
            node_type *new_node = new node_type( n );
            new_node->clear_children();
            c.node()->attach_child( new_node );
            new_node->attach_parent( c.node() );
            ret = cursor( new_node );
        }
        for( size_t i=0 ; i<n.size() ; ++i )
            insert_below( ret , *( n.child_node( i ) ) );
        return ret;
    }
    
    template< typename InputCursor >
    cursor insert_below( cursor c , InputCursor other )
    {
        insert_below( c , *( other.node() ) );
    }
    
    void erase( cursor position ) noexcept
    {
        if( position.node() == nullptr ) return;
        node_pointer parent = position.parent_node();
        if( parent != nullptr ) parent->remove_child( position.node() );
        erase_impl( position );
    }
    
    
    void clear( void )
    {
        erase( m_root );
    }


private:
    
    void erase_impl( cursor position ) noexcept
    {
        --m_size;
        for( size_t i=0 ; i<position.size() ; ++i )
            erase_impl( position.children(i) );
        node_pointer ptr = position.node();
        delete ptr;
    }

    node_pointer m_root;
    size_type m_size;
};



} // namespace gpcxx






#endif // GPCXX_TREE_INTRUSIVE_TREE_HPP_DEFINED
