/*
 * linked_node.hpp
 * Date: 2013-02-03
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef LINKED_NODE_HPP_INCLUDED
#define LINKED_NODE_HPP_INCLUDED

#include <gp/core/node.hpp>

#include <array>


namespace gp {

    template< class T >
    struct linked_node
    {
        typedef T value_type;
        typedef T* child_type;
        const static size_t max_arity = 3;

        size_t arity;
        std::array< linked_node< T >* , max_arity > children;
        linked_node< T > *parent;
        value_type value;
        size_t num_elements;
        size_t height;
        size_t level;


        linked_node( T v )
            : value( v ) , arity( 0 ) , children() , parent( 0 ) , num_elements( 0 ) , height( 0 ) , level( 0 )
        {
            std::fill( children.begin() , children.end() , static_cast< linked_node* >( 0 ) );
        }

        linked_node( T v , size_t a )
            : value( v ) , arity( a ) , children() , parent( 0 ) , num_elements( 0 ) , height( 0 ) , level( 0 )
        {
            std::fill( children.begin() , children.end() , static_cast< linked_node* >( 0 ) );
        }

    
        linked_node( T v , linked_node *n1 )
            : value( v ) , arity( 1 ) , children() , parent( 0 ) , num_elements( 0 ) , height( 0 ) , level( 0 )
        {
            std::fill( children.begin() , children.end() , static_cast< linked_node* >( 0 ) );
            children[0] = n1;
        }
    
        linked_node( T v , linked_node *n1 , linked_node *n2 ) 
            : value( v ) , arity( 2 ) , children() , parent( 0 ) , num_elements( 0 ) , height( 0 ) , level( 0 )
        {
            std::fill( children.begin() , children.end() , static_cast< linked_node* >( 0 ) );
            children[0] = n1;
            children[1] = n2;
        }

        linked_node( T v , linked_node *n1 , linked_node *n2 , linked_node *n3 )
            : value( v ) , arity( 3 ) , children() , parent( 0 ) , num_elements( 0 ) , height( 0 ) , level( 0 )
        {
            std::fill( children.begin() , children.end() , static_cast< linked_node* >( 0 ) );
            children[0] = n1;
            children[1] = n2;
            children[2] = n3;
        }


        linked_node( const linked_node &n )
            : value( n.value ) , arity( n.arity ) , children() , num_elements( n.num_elements ) , height( n.height ) , level( n.level )
        {
            std::fill( children.begin() , children.end() , static_cast< linked_node* >( 0 ) );
            for( size_t i=0 ; i<arity ; ++i )
                children[i] = ( ( n.children[i] != 0 ) ? new linked_node( *( n.children[i] ) ) : 0 );
        }

        ~linked_node( void )
        {
            for( size_t i=0 ; i<arity ; ++i ) delete children[i];
        }

        const linked_node& operator=( const linked_node &n )
        {
            value = n.value;
            arity = n.arity;
            std::fill( children.begin() , children.end() , static_cast< linked_node* >( 0 ) );
            for( size_t i=0 ; i<arity ; ++i )
                children[i] = ( ( n.children[i] != 0 ) ? new linked_node( *( n.children[i] ) ) : 0 );
            num_elements = n.num_elements;
            height = n.height;
            level = n.level;
            return *this;
        }
    };

    template< class T >
    struct arity_impl< linked_node< T > >
    {
        static inline size_t apply( const T &t ) { return t.arity; }
    };

    template< class T >
    struct num_elements_impl< linked_node< T > >
    {
        static inline size_t apply( const T &t ) { return t.num_elements; }
    };
    
    template< size_t I , class T >
    struct children_impl< I , linked_node< T > >
    {
        typedef linked_node< T >* type;
        static inline type apply( const T &t ) { return t.children[I]; }
    };

    template< size_t I , class T >
    struct children_impl< I , linked_node< T > const >
    {
        typedef linked_node< T > const* type;
        static inline type apply( const T &t ) { return t.children[I]; }
    };

    template< class T >
    struct parent_impl< linked_node< T > >
    {
        typedef linked_node< T >* type;
        static inline type apply( const T &t ) { return t.parent; }
    };

    template< class T >
    struct parent_impl< linked_node< T > const >
    {
        typedef linked_node< T > const* type;
        static inline type apply( const T &t ) { return t.parent; }
    };

} // namespace gp

#endif // LINKED_NODE_HPP_INCLUDED
