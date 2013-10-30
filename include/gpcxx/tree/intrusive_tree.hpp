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

#include <functional>
#include <array>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <queue>



namespace gpcxx {



template< typename Node >
class intrusive_tree
{
    typedef intrusive_tree< Node > self_type;
    
public:
    
    typedef Node node_type;
    typedef node_type* node_pointer;
    typedef detail::intrusive_cursor< node_type > cursor;
    typedef detail::intrusive_cursor< node_type const > const_cursor;
    typedef size_t size_type;
    typedef node_type value_type;
    
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
    
    intrusive_tree( intrusive_tree&& tree )
    : intrusive_tree()
    {
        move_impl( std::move( tree ) );
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

    intrusive_tree& operator=( intrusive_tree&& tree )
    {
        if( &tree != this )
        {
            clear();
            move_impl( std::move( tree ) );
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
    
    const_cursor croot( void ) const
    {
        return root();
    }
    
    cursor shoot( void )
    {
        return cursor( nullptr );
    }
    
    const_cursor shoot( void ) const
    {
        return const_cursor( nullptr );
    }
    
    const_cursor cshoot( void ) const
    {
        return shoot();
    }


    
    cursor rank_is( size_type n ) noexcept
    {
        if( n >= m_size )
            return shoot();
        return rank_is_impl< cursor >( root() , n );
    }
    
    const_cursor rank_is( size_type n ) const noexcept
    {
        if( n >= m_size )
            return shoot();
        return rank_is_impl< const_cursor >( root() , n );
    }

    
    //
    // modifiers:
    //
    template< typename InputCursor >
    void assign( InputCursor subtree )
    {
        clear();
        insert_below( root() , subtree );
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
        m_root = nullptr;
    }
    
    void swap( intrusive_tree& other )
    {
        self_type tmp = std::move( other );
        other = std::move( *this );
        *this = std::move( tmp );
    }
    
    void swap_subtrees( cursor c1 , intrusive_tree& other , cursor c2 )
    {
        node_pointer n1 = c1.node();
        node_pointer n2 = c2.node();
    
        long num_nodes1 = 0 , num_nodes2 = 0;
        if( ( n1 == nullptr ) && ( n2 == nullptr ) )
        {
            return;
        }
        else if( n1 == nullptr )
        {
            num_nodes2 = n2->count_nodes();
            swap_subtree_impl1( *this , other , n2 );
        }
        else if( n2 == nullptr )
        {
            num_nodes1 = n1->count_nodes();
            swap_subtree_impl1( other , *this , n1 );
        }
        else
        {
            num_nodes1 = n1->count_nodes();
            num_nodes2 = n2->count_nodes();
            swap_subtree_impl2( *this , n1 , other , n2 );
        }
        m_size = m_size - ( num_nodes1 - num_nodes2 );
        other.m_size = other.m_size - ( num_nodes2 - num_nodes1 );
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
    
    void move_impl( intrusive_tree &&tree )
    {
        if( !tree.empty() )
        {
            m_root = tree.m_root;
            m_size = tree.m_size;
            tree.m_root = nullptr;
            tree.m_size = 0;
        }
    }
    
    static void swap_subtree_impl1( intrusive_tree &t1 , intrusive_tree &t2 , node_pointer n2 )
    {
        t1.m_root = n2;
        node_pointer p2 = n2->parent();
        if( p2 != nullptr )
        {
            n2->parent()->remove_child( n2 );
        }
        else
        {
            t2.m_root = nullptr;
        }
        n2->attach_parent( nullptr );
    }
    
    static void swap_subtree_impl2( intrusive_tree &t1 , node_pointer n1 , intrusive_tree &t2 , node_pointer n2 )
    {
        node_pointer p1 = n1->parent();
        node_pointer p2 = n2->parent();
        
        if( ( p1 == nullptr ) && ( p2 == nullptr ) )
        {
            t1.m_root = n2;
            t2.m_root = n1;
        }
        else if( p1 == nullptr )
        {
            t1.m_root = n2;
            p2->remove_child( n2 );
            n2->attach_parent( nullptr );
        }
        else if( p2 == nullptr )
        {
            t2.m_root = n1;
            p1->remove_child( n1 );
            n1->attach_parent( nullptr );
        }
        else
        {
            * ( p1->find_child( n1 ) ) = n2;
            * ( p2->find_child( n2 ) ) = n1;
            n2->attach_parent( p1 );
            n1->attach_parent( p2 );
        }
    }

    
    template< typename Cursor >
    Cursor rank_is_impl( Cursor c , size_type remaining ) const
    {
        std::queue< Cursor > cursor_queue;
        cursor_queue.push( c );

        while( ( remaining != 0 ) && ( !cursor_queue.empty() ) )
        {
            Cursor current = cursor_queue.front();
            for( size_t i=0 ; i<current.size() ; ++i ) cursor_queue.push( current.children( i ) );
            cursor_queue.pop();
            --remaining;
        }
        assert( !cursor_queue.empty() );
        return cursor_queue.front();
    }

    node_pointer m_root;
    size_type m_size;
};

//
// compare algorithms:
//
template< typename T >
bool operator==( intrusive_tree< T > const& x , intrusive_tree< T > const& y )
{
    if( x.size() != y.size() ) return false;
    return detail::cursor_comp( x.root() , y.root() );
}

template< typename T >
bool operator!=( intrusive_tree< T > const& x , intrusive_tree< T > const& y )
{
    return !( x == y );
}


//
// specialized algorithms:
//
template< typename T >
void swap( intrusive_tree< T >& x , intrusive_tree< T >& y )
{
    x.swap( y );
}

template< typename T >
void swap_subtrees( intrusive_tree< T >& t1 ,
                    typename intrusive_tree< T >::cursor c1 ,
                    intrusive_tree< T >& t2 ,
                    typename intrusive_tree< T >::cursor c2 )
{
    t1.swap_subtrees( c1 , t2 , c2 );
}



} // namespace gpcxx






#endif // GPCXX_TREE_INTRUSIVE_TREE_HPP_DEFINED
