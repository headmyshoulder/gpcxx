/*
  gpcxx/tree/basic_tree_fast.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GPCXX_TREE_BASIC_TREE_FAST_HPP_DEFINED
#define GPCXX_TREE_BASIC_TREE_FAST_HPP_DEFINED

#include <gpcxx/tree/detail/basic_node_fast.hpp>
#include <gpcxx/tree/detail/basic_cursor_fast.hpp>
#include <gpcxx/tree/cursor_traits.hpp>

#include <boost/mpl/and.hpp>

#include <memory>
#include <cstddef>
#include <cassert>
#include <queue>


namespace gpcxx {


template< typename T , typename Allocator = std::allocator< T > >
class basic_tree_fast
{
    typedef basic_tree_fast< T , Allocator > self_type;

    static const size_t max_arity = 2;
    typedef detail::basic_node_fast< T , max_arity >                node_type;
    typedef node_type*                                              node_pointer;
    typedef typename Allocator::template rebind< node_type >::other node_allocator_type;
    
    template< typename OtherCursor >
    struct same_value_type
    {
        typedef typename std::is_convertible< typename cursor_value< OtherCursor >::type , T >::type type;
    };
    
    template< typename OtherCursor >
    struct other_cursor_enabler :
        std::enable_if< boost::mpl::and_< is_cursor< OtherCursor > , same_value_type< OtherCursor > >::value >
    {
    };
    
public:
  
    //
    // types:
    //
    typedef T                                                               value_type;
    typedef Allocator                                                       allocator_type;
    typedef value_type&                                                     reference;
    typedef const value_type&                                               const_reference;
    typedef detail::basic_node_cursor_fast< node_type >                     cursor;
    typedef detail::basic_node_cursor_fast< node_type const >               const_cursor;
    typedef size_t                                                          size_type;
    typedef ptrdiff_t                                                       difference_type;
    typedef typename std::allocator_traits< allocator_type >::pointer       pointer;
    typedef typename std::allocator_traits< allocator_type >::const_pointer const_pointer;

    template< typename U > struct rebind
    {
        typedef basic_tree_fast< U, typename Allocator::template rebind<U>::other > other;
    };

    
    
    //
    // construct:
    //
    explicit basic_tree_fast( allocator_type const& allocator = allocator_type() )
    : m_node_allocator( allocator ) , m_header( nullptr ) , m_size( 0 )
    {
    }
    
    template< typename InputCursor , typename Enabler = typename other_cursor_enabler< InputCursor >::type >
    basic_tree_fast( InputCursor subtree , allocator_type const& allocator = allocator_type() )
    : basic_tree_fast( allocator )
    {
        insert_below( root() , subtree );
    }
    
    basic_tree_fast( basic_tree_fast const& tree )
    : basic_tree_fast( tree.get_allocator() )
    {
        if( !tree.empty() )
            insert_below( root() , tree.root() );
    }
    
    basic_tree_fast( basic_tree_fast const& tree , allocator_type const& allocator )
    : basic_tree_fast( allocator )
    {
        if( !tree.empty() )
            insert_below( root() , tree.root() );
    }
    
    basic_tree_fast( basic_tree_fast&& tree )
    : basic_tree_fast( tree.get_allocator() )
    {
        move_impl( std::move( tree ) );
    }
    
    basic_tree_fast( basic_tree_fast&& tree , allocator_type const& allocator )
    : basic_tree_fast( allocator )
    {
        move_impl( std::move( tree ) );
    }

    
    
    
    //
    // destroy:
    //
    ~basic_tree_fast( void )
    {
        clear();
    }

    
    
    
    //
    // copy:
    //
    basic_tree_fast& operator=( basic_tree_fast const& tree )
    {
        if( &tree != this )
        {
            clear();
            if( !tree.empty() )
                insert_below( root() , tree.root() );
        }
        return *this;
    }
    
    basic_tree_fast& operator=( basic_tree_fast&& tree )
    {
        if( &tree != this )
        {
            clear();
            move_impl( std::move( tree ) );
        }
        return *this;
    }
    
    
    
    
    //
    // cursors:
    //
    cursor root() noexcept
    {
        return cursor( m_header );
    }
    
    const_cursor root() const noexcept
    {
        return const_cursor( m_header );
    }
    
    const_cursor croot() const noexcept
    {
        return const_cursor( m_header );
    }

    cursor shoot() noexcept
    {
        return cursor( m_header );
    }

    const_cursor shoot() const noexcept
    {
        return const_cursor( m_header );
    }

    const_cursor cshoot() const noexcept
    {
        return const_cursor( m_header );
    }
    
//     cursor rank_is( size_type n ) noexcept
//     {
//         if( n >= m_size )
//             return shoot();
//         return rank_is_impl< cursor >( root() , n );
//     }
//     
//     const_cursor rank_is( size_type n ) const noexcept
//     {
//         if( n >= m_size )
//             return shoot();
//         return rank_is_impl< const_cursor >( root() , n );
//     }


    
    

    //
    // queries and capacity:
    //
    bool empty( void ) const noexcept
    {
        return m_size == 0;
    }
        
    size_type size( void ) const noexcept
    {
        return m_size;
    }
    
    size_type max_size( void ) const noexcept
    {
        return size_type( -1 );
    }

    allocator_type get_allocator( void ) const noexcept
    {
        return allocator_type( m_node_allocator );
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
    
    cursor insert_below( cursor position , const value_type& val )
    {
        node_pointer new_node = m_node_allocator.allocate( 1 );
        m_node_allocator.construct( new_node , val );
        return insert_below_impl( position , new_node );
    }
       
    cursor insert_below( const_cursor position , value_type &&val )
    {
        node_pointer new_node = m_node_allocator.allocate( 1 );
        *new_node = std::move( val );
        return insert_below_impl( position , new_node );
    }
    
    template< typename InputCursor , typename Enabler = typename other_cursor_enabler< InputCursor >::type >
    cursor insert_below( cursor position , InputCursor subtree )
    {
        cursor p = insert_below( position , *subtree );
        for( size_t i=0 ; i<subtree.size() ; ++i )
            insert_below( p , subtree.children(i) );
        return p;
    }

//     void swap( basic_tree_fast& other )
//     {
//         self_type tmp = std::move( other );
//         other = std::move( *this );
//         *this = std::move( tmp );
//     }
//     
//     void swap_subtrees( cursor c1 , basic_tree_fast& other , cursor c2 )
//     {
//         node_base_pointer parent1 = c1.parent_node();
//         node_base_pointer parent2 = c2.parent_node();
//         node_base_pointer n1 = c1.node();
//         node_base_pointer n2 = c2.node();
//         
//         size_type i1 = parent1->child_index( n1 );
//         size_type i2 = parent2->child_index( n2 );
//         
//         parent1->set_children( i1 , n2 );
//         parent2->set_children( i2 , n1 );
//         
//         size_type num_nodes1 = 0 , num_nodes2 = 0;
//         if( n1 != nullptr )
//         {
//             n1->attach_parent( parent2 );
//             num_nodes1 = n1->count_nodes();
//         }
//         if( n2 != nullptr )
//         {
//             n2->attach_parent( parent1 );
//             num_nodes2 = n2->count_nodes();
//         }
//         m_size = m_size - num_nodes1 + num_nodes2;
//         other.m_size = other.m_size - num_nodes2 + num_nodes1;
//     }


    
    void erase( cursor position ) noexcept
    {
        node_pointer ptr = position.node();
        if( ptr == nullptr ) return;
        if( position.parent_node() != nullptr )
        {
            position.parent_node()->remove_child( ptr );
        }
        erase_impl( position );
    }

    void clear( void ) noexcept
    {
        erase( root() );
    }
    
    
    
    
    //
    // additional stuff for concept correctness:
    //
    // cursor insert( cursor position , const value_type &val );
    // cursor insert( cursor position , value_type &&val );
    // template< typename InputCursor > cursor insert( cursor position , InputCursor subtree );
    // size_type rank_of( const_cursor c ) const;



private:
    
    void erase_impl( cursor position ) noexcept
    {
        --m_size;
        for( size_t i=0 ; i<position.size() ; ++i )
            erase_impl( position.children(i) );
        node_pointer ptr = position.node();
        m_node_allocator.destroy( ptr );
        m_node_allocator.deallocate( ptr , 1 );
    }
    
    cursor insert_below_impl( cursor position , node_pointer new_node )
    {
        ++m_size;
        
        if( position.node() == nullptr )
        {
            assert( m_header == nullptr );
            m_header = new_node;
            new_node->attach_parent( nullptr );
            return cursor( new_node );
        }
        else
        {
            new_node->attach_parent( position.node() );
            position.node()->attach_child( new_node );
            return cursor( new_node );
        }
    }
    
    void move_impl( basic_tree_fast &&tree )
    {
        if( !tree.empty() )
        {
            m_header = tree.m_header;
            m_size = tree.m_size;
            tree.m_size = 0;
            tree.m_header = nullptr;
        }
    }
    
//     template< typename Cursor >
//     Cursor rank_is_impl( Cursor c , size_type remaining ) const
//     {
//         std::queue< Cursor > cursor_queue;
//         cursor_queue.push( c );
// 
//         while( ( remaining != 0 ) && ( !cursor_queue.empty() ) )
//         {
//             Cursor current = cursor_queue.front();
//             for( Cursor i = current.begin() ; i != current.end() ; ++i ) cursor_queue.push( i );
//             cursor_queue.pop();
//             --remaining;
//         }
//         assert( !cursor_queue.empty() );
//         return cursor_queue.front();
//     }

    
    node_allocator_type m_node_allocator;
    node_pointer m_header;
    size_t m_size;
};




/*
//
// compare algorithms:
//
template< typename T , typename Allocator >
bool operator==( basic_tree_fast< T , Allocator > const& x , basic_tree_fast< T , Allocator > const& y )
{
    if( x.size() != y.size() ) return false;
    return detail::cursor_comp( x.root() , y.root() );
}

template< typename T, typename Allocator >
bool operator!=( basic_tree_fast< T , Allocator > const& x , basic_tree_fast< T , Allocator > const& y )
{
    return !( x == y );
}*/

/*
//
// specialized algorithms:
//
template< typename T , typename Allocator >
void swap( basic_tree_fast< T , Allocator >& x , basic_tree_fast< T , Allocator >& y )
{
    x.swap( y );
}

template< typename T , typename Allocator >
void swap_subtrees( basic_tree_fast< T , Allocator >& t1 ,
                    typename basic_tree_fast< T , Allocator >::cursor c1 ,
                    basic_tree_fast< T , Allocator >& t2 ,
                    typename basic_tree_fast< T , Allocator >::cursor c2 )
{
    t1.swap_subtrees( c1 , t2 , c2 );
}*/


} // namespace gpcxx






#endif // GPCXX_TREE_BASIC_TREE_FAST_HPP_DEFINED
