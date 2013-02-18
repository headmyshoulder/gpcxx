/*
 * linked_node_tree.hpp
 * Date: 2013-02-03
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef LINKED_NODE_TREE_HPP_INCLUDED
#define LINKED_NODE_TREE_HPP_INCLUDED

#include <gp/tree/linked_node.hpp>

namespace gp {


    template< class T >
    struct linked_node_tree
    {
        typedef T value_type;
        typedef linked_node< T >* node_type;

        node_type data;
    };







} // namespace gp

#endif // LINKED_NODE_TREE_HPP_INCLUDED
