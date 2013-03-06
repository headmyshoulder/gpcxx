/*
 * linked_node.hpp
 * Date: 2013-02-03
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef LINKED_NODE_HPP_INCLUDED
#define LINKED_NODE_HPP_INCLUDED

#include <array>
#include <cstddef>


namespace gp {


template< class T >
struct node_base
{
    const static size_t max_arity = 4;
    typedef T* node_ptr;

    size_t arity;
    std::array< node_ptr , max_arity > children;
    node_ptr parent;


};

template< class T >
struct linked_node
{
    typedef T value_type;
    typedef T* child_type;
    const static size_t max_arity = 4;

    size_t arity;
    std::array< linked_node< T >* , max_arity > children;
    linked_node< T > *parent;
    value_type value;
    size_t num_elements;
    size_t height;
    size_t level;

    linked_node( void )
        : value() , arity( 0 ) , parent( 0 ) , num_elements( 0 ) , height( 0 ) , level( 0 )
    {
        std::fill( children.begin() , children.end() , nullptr );
    }

    linked_node( T v )
        : value( v ) , arity( 0 ) , children() , parent( 0 ) , num_elements( 0 ) , height( 0 ) , level( 0 )
    {
        std::fill( children.begin() , children.end() , nullptr );
    }

    linked_node( T v , size_t a )
        : value( v ) , arity( a ) , children() , parent( 0 ) , num_elements( 0 ) , height( 0 ) , level( 0 )
    {
        std::fill( children.begin() , children.end() , nullptr );
    }
    
    linked_node( T v , linked_node *n1 )
        : value( v ) , arity( 1 ) , children() , parent( 0 ) , num_elements( 0 ) , height( 0 ) , level( 0 )
    {
        std::fill( children.begin() , children.end() , nullptr );
        children[0] = n1;
    }
    
    linked_node( T v , linked_node *n1 , linked_node *n2 ) 
        : value( v ) , arity( 2 ) , children() , parent( 0 ) , num_elements( 0 ) , height( 0 ) , level( 0 )
    {
        std::fill( children.begin() , children.end() , nullptr );
        children[0] = n1;
        children[1] = n2;
    }

    linked_node( T v , linked_node *n1 , linked_node *n2 , linked_node *n3 )
        : value( v ) , arity( 3 ) , children() , parent( 0 ) , num_elements( 0 ) , height( 0 ) , level( 0 )
    {
        std::fill( children.begin() , children.end() , nullptr );
        children[0] = n1;
        children[1] = n2;
        children[2] = n3;
    }

    linked_node( linked_node && ) = delete ;

    linked_node( const linked_node &n )
        : value( n.value ) , arity( n.arity ) , children() , parent( 0 ) , num_elements( n.num_elements ) , height( n.height ) , level( n.level )
    {
        std::fill( children.begin() , children.end() , nullptr );
        for( size_t i=0 ; i<arity ; ++i )
            children[i] = ( ( n.children[i] != 0 ) ? new linked_node( *( n.children[i] ) ) : nullptr );
    }

    ~linked_node( void )
    {
        for( size_t i=0 ; i<arity ; ++i ) delete children[i];
    }

    const linked_node& operator=( linked_node && ) = delete;

    const linked_node& operator=( const linked_node &n )
    {
        value = n.value;
        arity = n.arity;
        std::fill( children.begin() , children.end() , nullptr );
        for( size_t i=0 ; i<arity ; ++i )
            children[i] = ( ( n.children[i] != 0 ) ? new linked_node( *( n.children[i] ) ) : nullptr );
        num_elements = n.num_elements;
        height = n.height;
        level = n.level;
        return *this;
    }
};

namespace detail {

template< class T >
void complete_linked_tree_structure_impl( linked_node< T > *n , linked_node< T > *parent , size_t level )
{
    n->parent = parent;

    size_t elems = 0;

    size_t subtree_height = 0;
    for( size_t i=0 ; i<n->arity ; ++i )
    {
        complete_linked_tree_structure_impl( n->children[i] , n , level + 1 );
        elems += n->children[i]->num_elements;
        subtree_height = std::max( subtree_height , n->children[i]->height );
    }
    n->num_elements = 1 + elems;
    n->height = 1 + subtree_height;
    n->level = level;
}

} // namespace detail



} // namespace gp

#endif // LINKED_NODE_HPP_INCLUDED
