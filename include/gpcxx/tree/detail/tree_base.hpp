/*
 * gpcxx/tree/detail/tree_base.hpp
 * Date: 2015-02-09
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_TREE_DETAIL_TREE_BASE_HPP_INCLUDED
#define GPCXX_TREE_DETAIL_TREE_BASE_HPP_INCLUDED

// 
#include <gpcxx/tree/detail/tree_base_cursor.hpp>
#include <gpcxx/tree/cursor_traits.hpp>
#include <gpcxx/util/exception.hpp>

#include <boost/mpl/and.hpp>

#include <type_traits>
#include <queue>
#include <cassert>
#include <memory>


namespace gpcxx {
namespace detail {

    
    
template< typename Node , typename Allocator >
class tree_base
{
private:
    
    //
    // private types:
    //
    
    using self_type = tree_base< Node , Allocator >;

public:
    using node_type = Node;
    using node_pointer = node_type*;
    using const_node_pointer = node_type const*;
    
private:
    
    using node_base_type = typename node_type::node_base_type;
    using node_base_pointer = node_base_type*;
    
    using node_allocator_type = typename Allocator::template rebind< node_type >::other;
    
//     template< typename OtherCursor >
//     using same_value_type = std::is_convertible< typename cursor_value< OtherCursor >::type , T >;
//     
//     template< typename OtherCursor >
//     using other_cursor_enabler = std::enable_if< is_cursor< OtherCursor >::value && same_value_type< OtherCursor >::value >;


public:
    
    //
    // types:
    //
    
    using value_type = typename node_type::value_type;
    using reference = typename node_type::reference;
    using const_reference = typename node_type::const_reference;
    using allocator_type = Allocator;
    using cursor = tree_base_cursor< node_type >;
    using const_cursor = tree_base_cursor< node_type const >;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = typename std::allocator_traits< allocator_type >::pointer;
    using const_pointer = typename std::allocator_traits< allocator_type >::const_pointer;

    template< typename N >
    struct rebind
    {
        typedef tree_base< N , typename Allocator::template rebind< typename N::value_type >::other > other;
    };
    
    template< typename OtherCursor >
    struct same_value_type
    {
        typedef typename std::is_convertible< typename cursor_value< OtherCursor >::type , value_type >::type type;
    };
    
    template< typename OtherCursor >
    struct other_cursor_enabler :
        std::enable_if< boost::mpl::and_< is_cursor< OtherCursor > , same_value_type< OtherCursor > >::value >
    {
    };


    
    
    //
    // construct:
    //
    explicit tree_base( allocator_type const& allocator = allocator_type() )
    : m_node_allocator( allocator ) , m_header() , m_size( 0 )
    {
    }
    
    template< typename InputCursor , typename Enabler = typename other_cursor_enabler< InputCursor >::type >
    tree_base( InputCursor subtree , allocator_type const& allocator = allocator_type() )
    : tree_base( allocator )
    {
        insert_below( root() , subtree );
    }
    
    tree_base( tree_base const& tree )
    : tree_base( tree.get_allocator() )
    {
        if( !tree.empty() )
            insert_below( root() , tree.root() );
    }
    
    tree_base( tree_base const& tree , allocator_type const& allocator )
    : tree_base( allocator )
    {
        if( !tree.empty() )
            insert_below( root() , tree.root() );
    }
    
    tree_base( tree_base&& tree )
    : tree_base( tree.get_allocator() )
    {
        move_impl( std::move( tree ) );
    }
    
    tree_base( tree_base&& tree , allocator_type const& allocator )
    : tree_base( allocator )
    {
        move_impl( std::move( tree ) );
    }

    
    
    
    //
    // destroy:
    //
    ~tree_base( void )
    {
        clear();
    }

    
    
    //
    // copy:
    //
    tree_base& operator=( tree_base const& tree )
    {
        if( &tree != this )
        {
            clear();
            if( !tree.empty() )
                insert_below( root() , tree.root() );
        }
        return *this;
    }
    
    tree_base& operator=( tree_base&& tree )
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
    
    size_type height( void ) const
    {
        return croot().height();
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
    
    template< typename InputCursor >
    void assign( cursor position , InputCursor subtree )
    {
        if( position.node() == nullptr ) return;
        if( subtree.node() == nullptr ) return;
        
        while( ! position.empty() )
            erase( position.begin() );
        
//         for( cursor c = position.begin() ; c != position.end() ; ++c )
//         {
//             erase_impl( c );
//             static_cast< node_pointer >( position.node() )->set_child_node( c - position.begin() , nullptr );
//         }
        *position = *subtree;
        for( const_cursor c = subtree.begin() ; c!= subtree.end() ; ++c )
        {
            insert_below( position , c );
        }
    }
    
    cursor insert_below( const_cursor position , const value_type& val )
    {
        node_pointer new_node = m_node_allocator.allocate( 1 );
        m_node_allocator.construct( new_node , val );
        return insert_below_impl( position , new_node );
    }
    
    cursor insert_below( const_cursor position , value_type &&val )
    {
        node_pointer new_node = m_node_allocator.allocate( 1 );
        m_node_allocator.construct( new_node , std::move( val ) );
        return insert_below_impl( position , new_node );
    }
    
    template< typename InputCursor , typename Enabler = typename other_cursor_enabler< InputCursor >::type >
    cursor insert_below( const_cursor position , InputCursor subtree )
    {
        cursor p = insert_below( position , *subtree );
        for( InputCursor c = subtree.begin() ; c != subtree.end() ; ++c )
            insert_below( p , c );
        return p;
    }
    
    template< typename ... Args >
    cursor emplace_below( const_cursor position , Args&& ... args )
    {
        node_pointer new_node = m_node_allocator.allocate( 1 );
        m_node_allocator.construct( new_node , std::forward< Args >( args ) ... );
        return insert_below_impl( position , new_node );
    }
    
    cursor insert( const_cursor position , value_type const& val )
    {
        node_pointer new_node = m_node_allocator.allocate( 1 );
        m_node_allocator.construct( new_node , val );
        return insert_impl( position , new_node );
    }
    
    cursor insert( const_cursor position , value_type&& val )
    {
        node_pointer new_node = m_node_allocator.allocate( 1 );
        m_node_allocator.construct( new_node , std::move( val ) );
        return insert_impl( position , new_node );
    }
    
    template< typename InputCursor , typename Enabler = typename other_cursor_enabler< InputCursor >::type >
    cursor insert( const_cursor position , InputCursor subtree )
    {
        cursor p = insert( position , *subtree );
        for( InputCursor c = subtree.begin() ; c != subtree.end() ; ++c )
            insert_below( p , c );
        return p;
    }
    
    template< typename ... Args >
    cursor emplace( const_cursor position , Args&& ... args )
    {
        node_pointer new_node = m_node_allocator.allocate( 1 );
        m_node_allocator.construct( new_node , std::forward< Args >( args ) ... );
        return insert_impl( position , new_node );
    }

    cursor insert_above( const_cursor position , value_type const& val )
    {
        node_pointer new_node = m_node_allocator.allocate( 1 );
        m_node_allocator.construct( new_node , val );
        return insert_above_impl( position , new_node );
    }
    
    cursor insert_above( const_cursor position , value_type&& val )
    {
        node_pointer new_node = m_node_allocator.allocate( 1 );
        m_node_allocator.construct( new_node , val );
        return insert_above_impl( position , new_node );
    }

    
    void swap( tree_base& other )
    {
        self_type tmp = std::move( other );
        other = std::move( *this );
        *this = std::move( tmp );
    }
    
    void swap_subtrees( cursor c1 , tree_base& other , cursor c2 )
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
    
    
    
    void erase( const_cursor position ) noexcept
    {
        if( position.node() == nullptr ) return;
        
        --m_size;
        
        for( const_cursor c = position.begin() ; c != position.end() ; ++c )
        {
            erase_impl( c );
        }
        node_pointer ptr = const_cast< node_pointer >( static_cast< const_node_pointer >( position.node() ) );
        const_cast< node_pointer >( static_cast< const_node_pointer >( position.parent_node() ) )->remove_child( ptr );        
        m_node_allocator.destroy( ptr );
        m_node_allocator.deallocate( ptr , 1 );
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
    
    void erase_impl( const_cursor position ) noexcept
    {
        --m_size;
        for( const_cursor c = position.begin() ; c != position.end() ; ++c )
        {
            erase_impl( c );
        }
        node_pointer ptr = const_cast< node_pointer >( static_cast< const_node_pointer >( position.node() ) );
        m_node_allocator.destroy( ptr );
        m_node_allocator.deallocate( ptr , 1 );
    }
    
    cursor insert_below_impl( const_cursor position , node_pointer new_node )
    {
        ++m_size;
        if( position.node() == nullptr )
        {
            return insert_into_empty_node( position , new_node );
        }
        else
        {
            if( position.size() >= position.max_size() )
                throw tree_exception( "Max size of node reached." );
            
            node_base_pointer node = const_cast< node_base_pointer >( position.node() );
            new_node->set_parent_node( node );
            size_type index = node->attach_child( new_node );
            return cursor( node , index );
        }
    }
    
    cursor insert_impl( const_cursor position , node_pointer new_node )
    {
        ++m_size;
        if( position.node() == nullptr )
        {
            return insert_into_empty_node( position , new_node );
        }
        else
        {
            if( position.parent().size() >= position.parent().max_size() )
                throw tree_exception( "Max size of node reached." );
            if( position.is_root() )
                throw tree_exception( "Could not insert node in front of the root node." );

            node_base_pointer parent_node = const_cast< node_base_pointer >( position.parent_node() );
            new_node->set_parent_node( parent_node );
            parent_node->insert_child( position.pos() , new_node );
            return cursor( parent_node , position.pos() );
        }
    }
    
    cursor insert_above_impl( const_cursor position , node_pointer new_node )
    {
        ++m_size;
        if( position.node() == nullptr )
        {
            return insert_into_empty_node( position , new_node );
        }
        else
        {
            node_base_pointer node = const_cast< node_base_pointer >( position.node() );
            node_base_pointer parent_node = const_cast< node_base_pointer >( position.parent_node() );
            node->set_parent_node( new_node );
            new_node->set_parent_node( parent_node );
            parent_node->set_child_node( position.pos() , new_node );
            new_node->attach_child( node );
            return cursor { parent_node , position.pos() };
        }
    }
    
    cursor insert_into_empty_node( const_cursor position , node_pointer new_node )
    {
            node_base_pointer parent = const_cast< node_base_pointer >( position.parent_node() );
            assert( parent->size() == 0 );
            
            new_node->set_parent_node( parent );
            size_type index = parent->attach_child( new_node );
            assert( index == 0 );
            return cursor( parent , index );
    }
    
    void move_impl( tree_base&& tree )
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
            for( Cursor i = current.begin() ; i != current.end() ; ++i ) cursor_queue.push( i );
            cursor_queue.pop();
            --remaining;
        }
        assert( !cursor_queue.empty() );
        return cursor_queue.front();
    }
    
    
private:

    //
    // members:
    //
    node_allocator_type m_node_allocator;
    node_base_type m_header;
    size_t m_size;
};



//
// compare algorithms:
//
template< typename Node , typename Allocator >
bool operator==( tree_base< Node , Allocator > const& x , tree_base< Node , Allocator > const& y )
{
    if( x.size() != y.size() ) return false;
    return detail::cursor_equal( x.root() , y.root() );
}

template< typename Node , typename Allocator >
bool operator!=( tree_base< Node , Allocator > const& x , tree_base< Node , Allocator > const& y )
{
    return !( x == y );
}


//
// specialized algorithms:
//
template< typename Node , typename Allocator >
void swap( tree_base< Node , Allocator >& x , tree_base< Node , Allocator >& y )
{
    x.swap( y );
}

template< typename Node , typename Allocator >
void swap_subtrees( tree_base< Node , Allocator >& t1 ,
                    typename tree_base< Node , Allocator >::cursor c1 ,
                    tree_base< Node , Allocator >& t2 ,
                    typename tree_base< Node , Allocator >::cursor c2 )
{
    t1.swap_subtrees( c1 , t2 , c2 );
}




} // namespace detail
} // namespace gpcxx


#endif // GPCXX_TREE_DETAIL_TREE_BASE_HPP_INCLUDED
