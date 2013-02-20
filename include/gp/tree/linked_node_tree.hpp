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
class linked_node_tree
{
public:

    typedef T value_type;
    typedef linked_node< T > node_type;
    typedef node_type* node_pointer;
    typedef node_type const* const_node_pointer;
    typedef node_type& node_reference;
    typedef node_type const& const_node_reference;


    linked_node_tree( void ) : m_data( nullptr ) { }
    ~linked_node_tree( void ) { delete m_data; }

    node_pointer data( void ) { return m_data; }
    void set_data( node_pointer data ) { delete m_data; m_data = data; }

    node_reference root( void ) { return *m_data; }
    const_node_reference root( void ) const { return *m_data; }

private:

    node_pointer m_data;
};







} // namespace gp

#endif // LINKED_NODE_TREE_HPP_INCLUDED
