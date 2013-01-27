/*
 * mutation.h
 * Date: 2013-01-26
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef MUTATION_H_INCLUDED
#define MUTATION_H_INCLUDED

#include "tree.h"


namespace detail {

    template< class T , class Gen >
    T mutate_value( T t , Gen &gen )
    {
        size_t count = 0;
        while( count < 1000  )
        {
            T v = gen.random_symbol();
            if( v != t ) { return v; }
            ++count;
        }
        if( count == 1000 ) throw std::runtime_error( "Ups - terminal" );
        return T(); // dummy
    }

    template< class Node , class TerminalGen , class UnaryGen , class BinaryGen >
    void mutation_impl( Node *node , TerminalGen &terminal_gen , UnaryGen &unary_gen , BinaryGen &binary_gen )
    {
        typedef typename Node::value_type value_type;
        if( node->arity == 0 )
        {
            node->value = mutate_value( node->value , terminal_gen );
        }
        else if( node->arity == 1 )
        {
            node->value = mutate_value( node->value , unary_gen );
        }
        else
        {
            node->value = mutate_value( node->value , binary_gen );
        }
    }


} // namespace detail


template< class Tree , class TerminalGen , class UnaryGen , class BinaryGen >
void mutation( Tree &t , size_t i , TerminalGen &terminal_gen , UnaryGen &unary_gen , BinaryGen &binary_gen )
{
    typedef typename Tree::node_type node_type;
    node_type *n = find_node_to_index( t.m_data , i );
    assert( n != 0 );
    detail::mutation_impl( n , terminal_gen , unary_gen , binary_gen );
}

template< class Tree , class Rng , class TerminalGen , class UnaryGen , class BinaryGen >
void mutation( Tree &t , Rng &rng , TerminalGen &terminal_gen , UnaryGen &unary_gen , BinaryGen &binary_gen )
{
    typedef typename Tree::node_type node_type;

    std::uniform_int_distribution< size_t > dist( 0 , t.m_data->num_elements - 1 );
    size_t index = dist( rng );
    mutation( t , index , terminal_gen , unary_gen , binary_gen );
}


#endif // MUTATION_H_INCLUDED
