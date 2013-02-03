/*
 * linked_node_tree.hpp
 * Date: 2013-02-03
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef LINKED_NODE_TREE_HPP_INCLUDED
#define LINKED_NODE_TREE_HPP_INCLUDED

#include <gp/trees/linked_node.hpp>
#include <gp/core/tree.hpp>

namespace gp {


    template< class T >
    struct linked_node_tree
    {
        typedef T value_type;
        typedef linked_node< T >* node_type;

        node_type data;
    };


    template< class T >
    struct root_node_impl< linked_node_tree< T > >
    {
        typedef typename linked_node_tree< T >::node_type type;
        static inline type apply( linked_node_tree< T > &n ) { return n.data; }
    };

    template< class T >
    struct root_node_impl< const linked_node_tree< T > >
    {
        typedef typename linked_node_tree< const T >::node_type const type;
        static inline type apply( linked_node_tree< T > &n ) { return n.data; }
    };





} // namespace gp

#endif // LINKED_NODE_TREE_HPP_INCLUDED
