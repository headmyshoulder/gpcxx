/*
 * tree.h
 * Date: 2013-01-25
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

#include <vector>
#include <cstddef>

template< typename T >
struct node
{
    const static size_t max_arity = 2;

    T value;
    size_t arity;
    size_t child_index[ max_arity - 1 ];
    size_t level;
    size_t length;
    size_t subtree_height;
    int parent_index;

    node( void )
        : value() , arity( 0 ) , child_index() , level( 0 ) , length( 1 ) , subtree_height( 0 ) , parent_index( -1 )
    { }
};



template< class T >
class tree
{
public:

    typedef T value_type;
    typedef node< value_type > node_type;
    typedef std::vector< node_type > vector_type;

    tree( void ) : m_data() { }

    vector_type& data( void ) { return m_data; }
    const vector_type& data( void ) const { return m_data; }

private:

    vector_type m_data;
};


#endif // TREE_H_INCLUDED
