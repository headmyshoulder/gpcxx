/*
  gp/tree/basic_tree.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GP_TREE_BASIC_TREE_HPP_DEFINED
#define GP_TREE_BASIC_TREE_HPP_DEFINED

#include <gp/tree/detail/basic_node.hpp>
#include <gp/tree/detail/basic_cursor.hpp>
#include <gp/tree/cursor_traits.hpp>

#include <boost/mpl/and.hpp>

#include <memory>
#include <cstddef>
#include <cassert>
#include <queue>


namespace gp {


template< typename T , typename Allocator = std::allocator< T > >
class basic_tree
{
    typedef basic_tree< T , Allocator > self_type;

    static const size_t max_arity = 2;
    typedef detail::basic_node< T , max_arity >                     node_type;
    typedef node_type*                                              node_pointer;
    typedef typename node_type::node_base_type                      node_base_type;
    typedef node_base_type*                                         node_base_pointer;
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
    typedef detail::basic_node_cursor< node_type >                          cursor;
    typedef detail::basic_node_cursor< node_type const >                    const_cursor;
    typedef size_t                                                          size_type;
    typedef ptrdiff_t                                                       difference_type;
    typedef typename std::allocator_traits< allocator_type >::pointer       pointer;
    typedef typename std::allocator_traits< allocator_type >::const_pointer const_pointer;

    template< typename U > struct rebind
    {
        typedef basic_tree< U, typename Allocator::template rebind<U>::other > other;
    };

    
    
    //
    // construct:
    //
    explicit basic_tree( allocator_type const& allocator = allocator_type() )
    : m_node_allocator( allocator ) , m_header() , m_size( 0 )
    {
    }
    
    template< typename InputCursor , typename Enabler = typename other_cursor_enabler< InputCursor >::type >
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
    
    basic_tree( basic_tree const& tree , allocator_type const& allocator )
    : basic_tree( allocator )
    {
        if( !tree.empty() )
            insert_below( root() , tree.root() );
    }
    
    basic_tree( basic_tree&& tree )
    : basic_tree( tree.get_allocator() )
    {
        move_impl( std::move( tree ) );
    }
    
    basic_tree( basic_tree&& tree , allocator_type const& allocator )
    : basic_tree( allocator )
    {
        move_impl( std::move( tree ) );
    }

    
    
    
    //
    // destroy:
    //
    ~basic_tree( void )
    {
        clear();
    }

    
    
    
    //
    // copy:
    //
    basic_tree& operator=( basic_tree const& tree )
    {
        if( &tree != this )
        {
            clear();
            if( !tree.empty() )
                insert_below( root() , tree.root() );
        }
        return *this;
    }
    
    basic_tree& operator=( basic_tree&& tree )
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
        for( InputCursor c = subtree.begin() ; c != subtree.end() ; ++c )
            insert_below( p , c );
        return p;
    }

    void swap( basic_tree& other )
    {
        self_type tmp = std::move( other );
        other = std::move( *this );
        *this = std::move( tmp );
    }
    
    void swap_subtrees( cursor c1 , basic_tree& other , cursor c2 )
    {
        node_base_pointer parent1 = c1.parent_node();
        node_base_pointer parent2 = c2.parent_node();
        node_base_pointer n1 = c1.node();
        node_base_pointer n2 = c2.node();
        
        size_type i1 = parent1->child_index( n1 );
        size_type i2 = parent2->child_index( n2 );
        
        parent1->set_children( i1 , n2 );
        parent2->set_children( i2 , n1 );
        
        size_type num_nodes1 = 0 , num_nodes2 = 0;
        if( n1 != nullptr )
        {
            n1->attach_parent( parent2 );
            num_nodes1 = n1->count_nodes();
        }
        if( n2 != nullptr )
        {
            n2->attach_parent( parent1 );
            num_nodes2 = n2->count_nodes();
        }
        m_size = m_size - num_nodes1 + num_nodes2;
        other.m_size = other.m_size - num_nodes2 + num_nodes1;
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
        m_node_allocator.destroy( ptr );
        m_node_allocator.deallocate( ptr , 1 );
        position.parent_node()->remove_child( ptr );
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
        for( cursor c = position.begin() ; c != position.end() ; ++c )
        {
            erase_impl( c );
        }
        node_pointer ptr = static_cast< node_pointer >( position.node() );
        m_node_allocator.destroy( ptr );
        m_node_allocator.deallocate( ptr , 1 );
    }
    
    cursor insert_below_impl( cursor position , node_pointer new_node )
    {
        ++m_size;
        if( position.node() == nullptr )
        {
            assert( position.parent_node()->size() == 0 );
            
            new_node->attach_parent( position.parent_node() );
            size_type index = position.parent_node()->attach_child( new_node );
            assert( index == 0 );
            return cursor( position.parent_node() , index );
        }
        else
        {
            assert( position.size() < position.max_size() );

            new_node->attach_parent( position.node() );
            size_type index = position.node()->attach_child( new_node );
            return cursor( position.node() , index );
        }
    }
    
    void move_impl( basic_tree &&tree )
    {
        if( !tree.empty() )
        {
            node_base_pointer n = tree.m_header.children( 0 );
            n->attach_parent( &m_header );
            m_header.attach_child( n );
            m_size = tree.m_size;
            
            tree.m_size = 0;
            tree.m_header.set_children( 0 , nullptr );
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
            for( Cursor i = current.begin() ; i != current.end() ; ++i ) cursor_queue.push( i );
            cursor_queue.pop();
            --remaining;
        }
        assert( !cursor_queue.empty() );
        return cursor_queue.front();
    }

    
    node_allocator_type m_node_allocator;
    node_base_type m_header;
    size_t m_size;
};


//
// compare algorithms:
//
template< typename T , typename Allocator >
bool operator==( basic_tree< T , Allocator > const& x , basic_tree< T , Allocator > const& y )
{
    if( x.size() != y.size() ) return false;
    return detail::cursor_comp( x.root() , y.root() );
}

template< typename T, typename Allocator >
bool operator!=( basic_tree< T , Allocator > const& x , basic_tree< T , Allocator > const& y )
{
    return !( x == y );
}


//
// specialized algorithms:
//
template< typename T , typename Allocator >
void swap( basic_tree< T , Allocator >& x , basic_tree< T , Allocator >& y )
{
    x.swap( y );
}

template< typename T , typename Allocator >
void swap_subtrees( basic_tree< T , Allocator >& t1 ,
                    typename basic_tree< T , Allocator >::cursor c1 ,
                    basic_tree< T , Allocator >& t2 ,
                    typename basic_tree< T , Allocator >::cursor c2 )
{
    t1.swap_subtrees( c1 , t2 , c2 );
}


} // namespace gp






#endif // GP_TREE_BASIC_TREE_HPP_DEFINED
