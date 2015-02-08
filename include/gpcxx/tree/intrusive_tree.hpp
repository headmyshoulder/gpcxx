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
#include <gpcxx/tree/detail/intrusive_cursor.hpp>
#include <gpcxx/tree/cursor_traits.hpp>

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

    template< typename OtherCursor >
    struct same_value_type
    {
        typedef typename std::is_convertible< typename cursor_value< OtherCursor >::type , Node >::type type;
    };

    template< typename OtherCursor >
    struct other_cursor_enabler :
        std::enable_if< boost::mpl::and_< is_cursor< OtherCursor > , same_value_type< OtherCursor > >::value >
    {
    };

    
public:
    
    using node_type = Node;
    using node_pointer = node_type* ;
    using cursor = detail::intrusive_cursor< node_type >;
    using const_cursor = detail::intrusive_cursor< node_type const >;
    using node_base_type = typename node_type::node_base_type;
    using node_base_pointer = node_base_type*;
    using size_type = size_t;
    using value_type = node_type;
    
    //
    // construct:
    //
    intrusive_tree( void )
    : m_header{} , m_size{ 0 } { }
    
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

    
    cursor insert_below( cursor position , node_type const& n )
    {
        node_pointer new_node = new node_type { n };
        new_node->clear_children(); // put into copy ctor
        return insert_below_impl( position , new_node );
    }
    
    template< typename InputCursor , typename Enabler = typename other_cursor_enabler< InputCursor >::type >
    cursor insert_below( cursor position , InputCursor subtree )
    {
        cursor p = insert_below( position , *subtree );
        for( InputCursor c = subtree.begin() ; c != subtree.end() ; ++c )
        {
            insert_below( p , c );
        }
        return p;        
    }
    
    void erase( cursor position ) noexcept
    {
        --m_size;
        if( position.node() == nullptr ) return;
        
        for( cursor c = position.begin() ; c != position.end() ; ++c )
        {
            erase_impl( c );
        }
        node_pointer ptr = static_cast< node_pointer >( position.node() );
        position.parent_node()->remove_child( ptr );        
        delete ptr;
    }

    
    void clear( void )
    {
        erase( root() );
    }
    
    void swap( intrusive_tree& other )
    {
        self_type tmp = std::move( other );
        other = std::move( *this );
        *this = std::move( tmp );
    }
    
    void swap_subtrees( cursor c1 , intrusive_tree& other , cursor c2 )
    {
        node_base_pointer parent1 = c1.parent_node();
        node_base_pointer parent2 = c2.parent_node();
        node_base_pointer n1 = c1.node();
        node_base_pointer n2 = c2.node();
        
        size_type i1 = parent1->child_index( n1 );
        size_type i2 = parent2->child_index( n2 );
        
        parent1->set_child_node( i1 , n2 );
        parent2->set_child_node( i2 , n1 );
        
        long num_nodes1 = 0 , num_nodes2 = 0;
        if( n1 != nullptr )
        {
            n1->set_parent_node( parent2 );
            num_nodes1 = n1->count_nodes();
        }
        if( n2 != nullptr )
        {
            n2->set_parent_node( parent1 );
            num_nodes2 = n2->count_nodes();
        }
        m_size = ( long( m_size ) - num_nodes1 + num_nodes2 );
        other.m_size = ( long( other.m_size ) - num_nodes2 + num_nodes1 );
    }




private:
    
    void erase_impl( cursor position ) noexcept
    {
        --m_size;
        for( cursor c = position.begin() ; c != position.end() ; ++c )
        {
            erase_impl( c );
        }
        node_pointer ptr = static_cast< node_pointer >( position.node() );
        delete ptr;
    }
    
    cursor insert_below_impl( cursor position , node_pointer new_node )
    {
        ++m_size;
        if( position.node() == nullptr )
        {
            assert( position.parent_node()->size() == 0 );
            
            new_node->set_parent_node( position.parent_node() );
            size_type index = position.parent_node()->attach_child( new_node );
            assert( index == 0 );
            return cursor( position.parent_node() , index );
        }
        else
        {
            assert( position.size() < position.max_size() );

            new_node->set_parent_node( position.node() );
            size_type index = position.node()->attach_child( new_node );
            return cursor( position.node() , index );
        }
    }
    
    void move_impl( intrusive_tree &&tree )
    {
        if( !tree.empty() )
        {
            node_base_pointer n = tree.m_header.child_node( 0 );
            n->set_parent_node( &m_header );
            m_header.attach_child( n );
            m_size = tree.m_size;
            
            tree.m_size = 0;
            tree.m_header.set_child_node( 0 , nullptr );
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

    // node_pointer m_root;
    node_base_type m_header;
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

// TODO: Implement comparison operators
// TODO: Implement hash specialization



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


// namespace std
// {
//     template< typename T >
//     class hash< intrusive_tree< T > >
//     {
//         size_t operator()( intrusive_tree< T > const &t ) const
//         {
//             return t.hash();
//         }
//     };
// }



#endif // GPCXX_TREE_INTRUSIVE_TREE_HPP_DEFINED
