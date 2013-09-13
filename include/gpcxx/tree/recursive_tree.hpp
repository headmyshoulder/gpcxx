/*
 * gpcxx/tree/recursive_tree.hpp
 * Date: 2013-09-13
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_TREE_RECURSIVE_TREE_HPP_INCLUDED
#define GPCXX_TREE_RECURSIVE_TREE_HPP_INCLUDED

#include <gpcxx/tree/detail/recursive_node.hpp>
#include <gpcxx/tree/detail/recursive_cursor.hpp>
#include <gpcxx/tree/cursor_traits.hpp>

#include <boost/mpl/and.hpp>

#include <memory>
#include <cstddef>
#include <cassert>
#include <queue>

namespace gpcxx {
    

    
    
template< typename T >
class recursive_tree
{
    typedef recursive_tree< T > self_type;

    static const size_t max_arity = 2;
    typedef detail::recursive_node< T >                             node_type;
    typedef detail::recursive_node_base< T >                        node_base_type;
    typedef node_base_type*                                         node_base_pointer;
    
//     typedef detail::basic_node< T , max_arity >                     node_type;
//     typedef node_type*                                              node_pointer;
//     typedef typename node_type::node_base_type                      node_base_type;
//     typedef node_base_type*                                         node_base_pointer;
    
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
    typedef value_type&                                                     reference;
    typedef const value_type&                                               const_reference;
    typedef detail::recursive_node_cursor< node_type >                      cursor;
    typedef detail::recursive_node_cursor< node_type const >                const_cursor;
    typedef size_t                                                          size_type;
    typedef ptrdiff_t                                                       difference_type;
    typedef value_type *                                                    pointer;
    typedef value_type const *                                              const_pointer;

    template< typename U > struct rebind
    {
        typedef recursive_tree< U > other;
    };

    
    
    //
    // construct:
    //
    recursive_tree( void )
    : m_header() , m_size( 0 )
    {
    }
    
//     template< typename InputCursor , typename Enabler = typename other_cursor_enabler< InputCursor >::type >
//     recursive_tree( InputCursor subtree )
//     : recursive_tree()
//     {
//         insert_below( root() , subtree );
//     }
//     
//     recursive_tree( recursive_tree const& tree )
//     : recursive_tree()
//     {
//         if( !tree.empty() )
//             insert_below( root() , tree.root() );
//     }
//     
//     recursive_tree( recursive_tree&& tree )
//     : recursive_tree()
//     {
//         move_impl( std::move( tree ) );
//     }
// 
//     
//     
//     
//     //
//     // destroy:
//     //
//     ~recursive_tree( void )
//     {
//         clear();
//     }
// 
//     
//     
//     
//     //
//     // copy:
//     //
//     recursive_tree& operator=( recursive_tree const& tree )
//     {
//         if( &tree != this )
//         {
//             clear();
//             if( !tree.empty() )
//                 insert_below( root() , tree.root() );
//         }
//         return *this;
//     }
//     
//     recursive_tree& operator=( recursive_tree&& tree )
//     {
//         if( &tree != this )
//         {
//             clear();
//             move_impl( std::move( tree ) );
//         }
//         return *this;
//     }
    
    
    
    
    //
    // cursors:
    //
    cursor root() noexcept
    {
//         return cursor( &m_header , 0 );
    }
    
    const_cursor root() const noexcept
    {
//         return const_cursor( &m_header , 0 );
    }
    
    const_cursor croot() const noexcept
    {
//         return const_cursor( &m_header , 0 );
    }

    cursor shoot() noexcept
    {
//         return cursor( &m_header , 1 );
    }

    const_cursor shoot() const noexcept
    {
//         return const_cursor( &m_header , 1 );
    }

    const_cursor cshoot() const noexcept
    {
//         return const_cursor( &m_header , 1 );
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


 
 
 
//     //
//     // modifiers:
//     //
//     template< typename InputCursor >
//     void assign( InputCursor subtree )
//     {
//         clear();
//         insert_below( root() , subtree );
//     }
//     
    cursor insert_below( cursor position , const value_type& val )
    {
//         node_pointer new_node = new node_type( val );
//         return insert_below_impl( position , new_node );
    }
       
    cursor insert_below( const_cursor position , value_type &&val )
    {
//         node_pointer new_node = new node_type();
//         *new_node = std::move( val );
//         return insert_below_impl( position , new_node );
    }
//     
//     template< typename InputCursor , typename Enabler = typename other_cursor_enabler< InputCursor >::type >
//     cursor insert_below( cursor position , InputCursor subtree )
//     {
//         cursor p = insert_below( position , *subtree );
//         for( InputCursor c = subtree.begin() ; c != subtree.end() ; ++c )
//             insert_below( p , c );
//         return p;
//     }
// 
//     void swap( recursive_tree& other )
//     {
//         self_type tmp = std::move( other );
//         other = std::move( *this );
//         *this = std::move( tmp );
//     }
//     
//     void swap_subtrees( cursor c1 , recursive_tree& other , cursor c2 )
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
// 
// 
//     
//     void erase( cursor position ) noexcept
//     {
//         --m_size;
//         if( position.node() == nullptr ) return;
//         
//         for( cursor c = position.begin() ; c != position.end() ; ++c )
//         {
//             erase_impl( c );
//         }
//         node_pointer ptr = static_cast< node_pointer >( position.node() );
//         delete ptr;
//         position.parent_node()->remove_child( ptr );
//     }
// 
//     void clear( void ) noexcept
//     {
//         erase( root() );
//     }
    
    
    
    
    //
    // additional stuff for concept correctness:
    //
    // cursor insert( cursor position , const value_type &val );
    // cursor insert( cursor position , value_type &&val );
    // template< typename InputCursor > cursor insert( cursor position , InputCursor subtree );
    // size_type rank_of( const_cursor c ) const;



private:
    
//     void erase_impl( cursor position ) noexcept
//     {
//         --m_size;
//         for( cursor c = position.begin() ; c != position.end() ; ++c )
//         {
//             erase_impl( c );
//         }
//         node_pointer ptr = static_cast< node_pointer >( position.node() );
//         delete ptr;
//     }
//     
//     cursor insert_below_impl( cursor position , node_pointer new_node )
//     {
//         ++m_size;
//         if( position.node() == nullptr )
//         {
//             assert( position.parent_node()->size() == 0 );
//             
//             new_node->attach_parent( position.parent_node() );
//             size_type index = position.parent_node()->attach_child( new_node );
//             assert( index == 0 );
//             return cursor( position.parent_node() , index );
//         }
//         else
//         {
//             assert( position.size() < position.max_size() );
// 
//             new_node->attach_parent( position.node() );
//             size_type index = position.node()->attach_child( new_node );
//             return cursor( position.node() , index );
//         }
//     }
//     
//     void move_impl( recursive_tree &&tree )
//     {
//         if( !tree.empty() )
//         {
//             node_base_pointer n = tree.m_header.children( 0 );
//             n->attach_parent( &m_header );
//             m_header.attach_child( n );
//             m_size = tree.m_size;
//             
//             tree.m_size = 0;
//             tree.m_header.set_children( 0 , nullptr );
//         }
//     }
//     
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

    
    node_base_type m_header;
    size_t m_size;
};


// //
// // compare algorithms:
// //
// template< typename T >
// bool operator==( recursive_tree< T > const& x , recursive_tree< T > const& y )
// {
//     if( x.size() != y.size() ) return false;
//     return detail::cursor_comp( x.root() , y.root() );
// }
// 
// template< typename T >
// bool operator!=( recursive_tree< T > const& x , recursive_tree< T > const& y )
// {
//     return !( x == y );
// }
// 
// 
// //
// // specialized algorithms:
// //
// template< typename T >
// void swap( recursive_tree< T >& x , recursive_tree< T >& y )
// {
//     x.swap( y );
// }
// 
// template< typename T >
// void swap_subtrees( recursive_tree< T >& t1 , typename recursive_tree< T >::cursor c1 ,
//                     recursive_tree< T >& t2 , typename recursive_tree< T >::cursor c2 )
// {
//     t1.swap_subtrees( c1 , t2 , c2 );
// }
// 



} // namespace gpcxx


#endif // GPCXX_TREE_RECURSIVE_TREE_HPP_INCLUDED
