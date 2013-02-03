/*
 * node.h
 * Date: 2013-02-03
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef TREE_NODE_H_INCLUDED
#define TREE_NODE_H_INCLUDED


#include <gp/core/node_traits.hpp>


namespace gp
{
    /*
      template< class Node , class Enabler = void > struct arity_impl;
      template< class Node , class Enabler = void > struct num_elements_impl;
      template< size_t I , class Node , class Enabler = void > struct children_impl;
      template< class Node , class Enabler = void > struct parent_impl;
      template< class Node , class Enabler = void > struct children_iterator_impl;
    */

    /*
     * Arity Access
     */
    template< class Node , class Enabler = void > struct arity_impl;

    template< class Node >
    struct arity_impl< Node , typename boost::enable_if< typename gp::is_pointer_type< Node >::type >::type >
    {
        static inline size_t apply( Node n ) { return arity_impl< typename gp::unwrap_pointer< Node >::type  >::apply( *n ); }
    };

    template< class Node >
    size_t arity( const Node &n )
    {
        return arity_impl< Node >::arity( n );
    }






    /*
     * Num of elements access
     *
     * rename later to number_sub_nodes
     */
    template< class Node , class Enabler = void > struct num_elements_impl;

    template< class Node >
    struct num_elements_impl< Node , typename boost::enable_if< typename gp::is_pointer_type< Node >::type >::type >
    {
        static inline size_t apply( Node n ) { return num_elements_impl< typename gp::unwrap_pointer< Node >::type  >::apply( *n ); }
    };

    template< class Node >
    size_t num_elements( const Node &n )
    {
        return num_elements_impl< Node >::num_elements( n );
    }





    /*
     * Indexed children access
     */
    template< size_t I , class Node , class Enabler = void > struct children_impl;

    template< size_t I , class Node >
    struct children_impl< I , Node , typename boost::enable_if< typename gp::is_pointer_type< Node >::type >::type >
    {
        typedef typename unwrap_pointer< Node >::type elem_type;
        typedef typename node_traits::child< Node >::type result_type;
        static inline result_type apply( Node n ) { return children_impl< I , elem_type >::apply( *n ); }
    };

    template< size_t I , class Node >
    typename node_traits::child< Node >::type children( Node &n )
    {
        return children_impl< I , Node >::children( n );
    }





    /*
     * Parent access
     */
    template< class Node , class Enabler = void > struct parent_impl;

    template< class Node >
    struct parent_impl< Node , typename boost::enable_if< typename gp::is_pointer_type< Node >::type >::type >
    {
        typedef typename unwrap_pointer< Node >::type elem_type;
        typedef typename node_traits::parent< Node >::type result_type;
        static inline result_type apply( Node n ) { return parent_impl< elem_type >::apply( *n ); }
    };

    template< class Node >
    typename node_traits::parent< Node >::type parent( Node &n )
    {
        return parent_impl< Node >::parent( n );
    }





    /*
     * Access to an iterator of the children of the nodes
     */
    // template< class Node , class Enabler = void > struct children_iterator_impl;

    // template< class Node , typename boost::enable_if< typename gp::is_pointer_type< Node >::type >::type >
    // struct children_iterator_impl
    // {
    //     typedef typename gp::unwrap_pointer< Node >::type elem_type;
    //     typedef typename children_iterator_impl< elem_type >::type type;
    //     static inline type begin( Node &n ) { return children_iterator_impl< Node >::begin( n ); }
    //     static inline type end( Node &n ) { return children_iterator_impl< Node >::end( n ); }
    // };

    // template< class Node >
    // typename children_iterator_impl< Node >::type children_begin( const Node& n )
    // {
    //     return children_begin_impl< Node >::begin( n );
    // }

    // template< class Node >
    // typename children_iterator_impl< Node >::type children_end( const Node& n )
    // {
    //     return children_begin_impl< Node >::end( n );
    // }



    // // factory function
    // template< class Node >
    // Node create_node( size_t arity )
    // {
        
    // }

} // namespace stuff




#endif // TREE_NODE_H_INCLUDED
