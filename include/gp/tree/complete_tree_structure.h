/*
 * complete_tree_structure.h
 * Date: 2013-01-26
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef COMPLETE_TREE_STRUCTURE_H_INCLUDED
#define COMPLETE_TREE_STRUCTURE_H_INCLUDED

namespace detail
{
    template< class T >
    void complete_tree_structure_impl( tree_node< T > *n , tree_node< T > *parent , size_t level )
    {
        n->parent = parent;

        size_t elems = 0;

        size_t subtree_height = 0;
        for( size_t i=0 ; i<n->arity ; ++i )
        {
            complete_tree_structure_impl( n->children[i] , n , level + 1 );
            elems += n->children[i]->num_elements;
            subtree_height = std::max( subtree_height , n->children[i]->height );
        }
        n->num_elements = 1 + elems;
        n->height = 1 + subtree_height;
        n->level = level;
    }
}

template< class Tree >
void complete_tree_structure( Tree &t )
{
    typedef typename Tree::node_type node_type;
    if( t.m_data != 0  )
        detail::complete_tree_structure_impl( t.m_data , static_cast< node_type* >( 0 ) , 0 );
}


#endif // COMPLETE_TREE_STRUCTURE_H_INCLUDED
