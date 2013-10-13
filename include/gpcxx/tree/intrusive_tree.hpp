/*
  gpcxx/tree/intrusive_tree.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GPCXX_TREE_INTRUSIVE_TREE_HPP_DEFINED
#define GPCXX_TREE_INTRUSIVE_TREE_HPP_DEFINED

#include <iostream>
#include <functional>
#include <array>
#include <cmath>
#include <cassert>
#include <algorithm>


// debug stuff
#include <iostream>
using namespace std;


namespace gpcxx {
    


template< typename Node >
struct intrusive_cursor;


template< typename Node >
class intrusive_node
{
public:
    
    static const size_t max_arity = 2;
    typedef Node node_type;
    typedef node_type* node_pointer;
    typedef node_type const* const_node_pointer;
    typedef intrusive_cursor< node_type > cursor;
    typedef intrusive_cursor< node_type const > const_cursor;    
    typedef std::array< node_pointer , max_arity > children_type;
    
    
    
    //
    // debug:
    //
    std::string get_indent( size_t indent , std::string const& str = "  " )
    {
        std::string ret = "";
        for( size_t i=0 ; i<indent ; ++i ) ret += str;
        return ret;
    }
    
    void inspect( size_t indent = 0 )
    {
        cout << get_indent( indent ) << this << " : parent : " << m_parent << ", children";
        for( size_t i=0 ; i<max_arity ; ++i ) cout << " " << m_children[i];
        cout << "\n";
        
        for( size_t i=0 ; i<max_arity ; ++i )
            if( m_children[i] != nullptr ) m_children[i]->inspect( indent + 1 );
    }
    
    
    
    
    intrusive_node( node_type *parent = nullptr ) noexcept
    : m_parent( parent ) , m_children()
    {
        clear_children();
    }
    
    void clear_children( void )
    {
        std::fill( m_children.begin() , m_children.end() , nullptr );
    }

    
    
    
    node_pointer child_node( size_t i ) noexcept
    {
        return m_children[i];
    }
    
    
    const_node_pointer child_node( size_t i ) const noexcept
    {
        return m_children[i];
    }
    
    cursor children( size_t i );
    
    const_cursor children( size_t i ) const;
    
    void set_children( size_t i , node_type* n )
    {
        m_children[i] = n;
    }
    
    
    node_pointer parent( void ) noexcept
    {
        return m_parent;
    }
    
    
    const_node_pointer parent( void ) const noexcept
    {
        return m_parent;
    }

    size_t child_index( node_type const* child ) const
    {
        return std::distance( m_children.begin() , find_child( child ) );
    }
    
    
    size_t max_size( void ) const noexcept
    {
        return max_arity;
    }
    
    
    size_t size( void ) const noexcept
    {
        typename children_type::const_iterator end = std::find( m_children.begin() , m_children.end() , nullptr );
        return std::distance( m_children.begin() , end );
    }
    
    
    size_t attach_child( node_type *child )
    {
        typename children_type::iterator iter = find_free_child_entry();
        assert( iter != m_children.end() );
        *iter = child;
        return std::distance( m_children.begin() , iter );
    }
    
    
    void attach_parent( node_pointer parent )
    {
        m_parent = parent;
    }
    
    void remove_child( node_pointer child )
    {
        typename children_type::iterator iter = find_child( child );
        typename children_type::iterator end = m_children.begin() + size();
        
        assert( iter != m_children.end() );
        
        std::copy( iter + 1 , end-- , iter );
        *end = nullptr;
    }
    
    size_t count_nodes( void ) const
    {
        size_t count = 1;
        typename children_type::const_iterator iter = m_children.begin();
        typename children_type::const_iterator end = m_children.begin() + size();
        for( ; iter != end ; ++iter )
        {
            count += ( (*iter)->count_nodes() );
        }
        return count;
    }

    
protected:
    
    typename children_type::iterator find_free_child_entry( void )
    {
        return std::find_if( m_children.begin() , m_children.end() , []( const_node_pointer ptr ) { return ptr == nullptr; } );
    }
    
    typename children_type::const_iterator find_free_child_entry( void ) const
    {
        return std::find_if( m_children.begin() , m_children.end() , []( const_node_pointer ptr ) { return ptr == nullptr; } );
    }
    
    typename children_type::iterator find_child( const_node_pointer child )
    {
        return std::find( m_children.begin() , m_children.end() , child );
    }
    
    typename children_type::const_iterator find_child( const_node_pointer child ) const
    {
        return std::find( m_children.begin() , m_children.end() , child );
    }
    
    node_pointer m_parent;
    children_type m_children;
};



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


    intrusive_cursor( node_pointer node = nullptr )
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


template< typename Node >
typename intrusive_node< Node >::cursor intrusive_node< Node >::children( size_t i )
{
    return cursor( m_children[ i ] );
}
    
template< typename Node >
typename intrusive_node< Node >::const_cursor intrusive_node< Node >::children( size_t i ) const
{
    return const_cursor( m_children[i] );
}


template< typename Node >
class intrusive_tree
{
public:
    
    typedef Node node_type;
    typedef node_type* node_pointer;
    typedef intrusive_cursor< node_type > cursor;
    typedef intrusive_cursor< node_type const > const_cursor;
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
