/*
 * find_node_to_index.hpp
 * Date: 2013-02-20
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef FIND_NODE_TO_INDEX_HPP_INCLUDED
#define FIND_NODE_TO_INDEX_HPP_INCLUDED

#include <cstddef>


namespace gp {


namespace detail {

template< class Node >
Node* find_node_to_index_impl( Node* node , size_t index , size_t &count )
{
    if( index == count ) return node;
    for( size_t i=0 ; i<node->arity ; ++i )
    {
        ++count;
        Node *n = find_node_to_index_impl( node->children[i] , index , count );
        if( n != 0 ) return n;
    }
    return 0;
}

}


template< class Node >
Node* find_node_to_index( Node *start , size_t index )
{
    if( start == 0 ) return 0;
    size_t count = 0;
    return detail::find_node_to_index_impl( start , index , count );
}

} // namespace gp


#endif // FIND_NODE_TO_INDEX_HPP_INCLUDED
