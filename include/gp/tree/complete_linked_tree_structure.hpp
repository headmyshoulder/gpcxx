/*
 * complete_linked_tree_structure.hpp
 * Date: 2013-01-26
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef COMPLETE_LINKED_TREE_STRUCTURE_HPP_INCLUDED
#define COMPLETE_LINKED_TREE_STRUCTURE_HPP_INCLUDED

#include <gp/tree/linked_node_tree.hpp>


namespace gp {


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

template< class Tree >
void complete_linked_tree_structure( Tree &t )
{
    typedef typename Tree::node_type node_type;
    if( t.data() != 0  )
        detail::complete_linked_tree_structure_impl( t.data() , static_cast< node_type* >( 0 ) , 0 );
}

} // namespace gp


#endif // COMPLETE_LINKED_TREE_STRUCTURE_H_INCLUDED
