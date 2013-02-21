/*
 * complete_linked_tree_structure.hpp
 * Date: 2013-01-26
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef COMPLETE_LINKED_TREE_STRUCTURE_HPP_INCLUDED
#define COMPLETE_LINKED_TREE_STRUCTURE_HPP_INCLUDED

#include <gp/tree/linked_node_tree.hpp>


namespace gp {


template< class Tree >
void complete_linked_tree_structure( Tree &t )
{
    typedef typename Tree::node_type node_type;
    if( t.data() != 0  )
        detail::complete_linked_tree_structure_impl( t.data() , static_cast< node_type* >( 0 ) , 0 );
}

} // namespace gp


#endif // COMPLETE_LINKED_TREE_STRUCTURE_H_INCLUDED
