/*
 * tree.h
 * Date: 2013-01-25
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

#include <vector>
#include <cstddef>
#include <array>
#include <algorithm>




template< typename T >
struct tree_node
{
    typedef T value_type;

    const static size_t max_arity = 3;

    value_type value;
    size_t arity;
    std::array< tree_node< T >* , max_arity > children;
    tree_node< T > *parent;
    size_t num_elements;
    size_t height;
    size_t level;


    tree_node( T v )
        : value( v ) , arity( 0 ) , children() , parent( 0 ) , num_elements( 0 ) , height( 0 ) , level( 0 )
    {
        std::fill( children.begin() , children.end() , static_cast< tree_node* >( 0 ) );
    }

    tree_node( T v , size_t a )
        : value( v ) , arity( a ) , children() , parent( 0 ) , num_elements( 0 ) , height( 0 ) , level( 0 )
    {
        std::fill( children.begin() , children.end() , static_cast< tree_node* >( 0 ) );
    }

    
    tree_node( T v , tree_node *n1 )
        : value( v ) , arity( 1 ) , children() , parent( 0 ) , num_elements( 0 ) , height( 0 ) , level( 0 )
    {
        std::fill( children.begin() , children.end() , static_cast< tree_node* >( 0 ) );
        children[0] = n1;
    }
    
    tree_node( T v , tree_node *n1 , tree_node *n2 ) 
        : value( v ) , arity( 2 ) , children() , parent( 0 ) , num_elements( 0 ) , height( 0 ) , level( 0 )
    {
        std::fill( children.begin() , children.end() , static_cast< tree_node* >( 0 ) );
        children[0] = n1;
        children[1] = n2;
    }

    tree_node( T v , tree_node *n1 , tree_node *n2 , tree_node *n3 )
        : value( v ) , arity( 3 ) , children() , parent( 0 ) , num_elements( 0 ) , height( 0 ) , level( 0 )
    {
        std::fill( children.begin() , children.end() , static_cast< tree_node* >( 0 ) );
        children[0] = n1;
        children[1] = n2;
        children[2] = n3;
    }


    tree_node( const tree_node &n )
        : value( n.value ) , arity( n.arity ) , children() , num_elements( n.num_elements ) , height( n.height ) , level( n.level )
    {
        std::fill( children.begin() , children.end() , static_cast< tree_node* >( 0 ) );
        for( size_t i=0 ; i<arity ; ++i )
            children[i] = ( ( n.children[i] != 0 ) ? new tree_node( *( n.children[i] ) ) : 0 );
    }

    ~tree_node( void )
    {
        for( size_t i=0 ; i<arity ; ++i ) delete children[i];
    }

    const tree_node& operator=( const tree_node &n )
    {
        value = n.value;
        arity = n.arity;
        std::fill( children.begin() , children.end() , static_cast< tree_node* >( 0 ) );
        for( size_t i=0 ; i<arity ; ++i )
            children[i] = ( ( n.children[i] != 0 ) ? new tree_node( *( n.children[i] ) ) : 0 );
        num_elements = n.num_elements;
        height = n.height;
        level = n.level;
        return *this;
    }

};

template< class T >
size_t get_num_elements( const tree_node< T > *n )
{
    if( n == 0 ) return 0;
    size_t val = 1;
    for( size_t i=0 ; i<n->arity ; ++i ) val += get_num_elements( n->children[i] );
    return val;
}

namespace detail {

    template< class T >
    void fill_parent( tree_node< T > *n , tree_node< T > *parent )
    {
        n->parent = parent;
        for( size_t i=0 ; i<n->arity ; ++i ) fill_parent( n->children[i] , n );
    }
}

template< class T >
class tree
{
public:

    typedef T value_type;
    typedef tree_node< value_type > node_type;

    node_type *m_data;

    tree( void ) : m_data( 0 ) { }
    tree( const tree &t ) : m_data( ( t.m_data != 0 ) ? new node_type( *( t.m_data ) ) : 0 )
    {
        if( m_data != 0 ) detail::fill_parent( m_data , static_cast< node_type* >( 0 ) );
    }
    ~tree( void ) { delete m_data; }
    const tree& operator=( const tree &t )
    {
        delete m_data;
        m_data = ( ( t.m_data != 0 ) ? new node_type( *( t.m_data ) ) : 0 );
        if( m_data != 0 ) detail::fill_parent( m_data , static_cast< node_type* >( 0 )  );
        return *this;
    }
};










#endif // TREE_H_INCLUDED
