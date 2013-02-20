/*
 * mutation.h
 * Date: 2013-01-26
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef MUTATION_H_INCLUDED
#define MUTATION_H_INCLUDED

#include <gp/tree/find_node_to_index.hpp>

#include <stdexcept>
#include <random>
#include <cassert>

namespace gp {

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
        if( count == 1000 ) throw std::runtime_error( "Ups - " );
        return T(); // dummy
    }

    template< class Node , class TerminalGen , class UnaryGen , class BinaryGen >
    bool mutation_impl( Node *node , TerminalGen &terminal_gen , UnaryGen &unary_gen , BinaryGen &binary_gen )
    {
        typedef typename Node::value_type value_type;
        if( node->arity == 0 )
        {
            node->value = mutate_value( node->value , terminal_gen );
            return true;
        }
        else if( node->arity == 1 )
        {
            node->value = mutate_value( node->value , unary_gen );
            return false;
        }
        else
        {
            node->value = mutate_value( node->value , binary_gen );
            return true;
        }
    }


} // namespace detail


template< class Tree , class TerminalGen , class UnaryGen , class BinaryGen >
bool mutation( Tree &t , size_t i , TerminalGen &terminal_gen , UnaryGen &unary_gen , BinaryGen &binary_gen )
{
    typedef typename Tree::node_type node_type;
    node_type *n = find_node_to_index( t.data() , i );
    assert( n != 0 );
    return detail::mutation_impl( n , terminal_gen , unary_gen , binary_gen );
}

template< class Tree , class Rng , class TerminalGen , class UnaryGen , class BinaryGen >
void mutation( Tree &t , Rng &rng , TerminalGen &terminal_gen , UnaryGen &unary_gen , BinaryGen &binary_gen )
{
    typedef typename Tree::node_type node_type;

    std::uniform_int_distribution< size_t > dist( 0 , t.m_data->num_elements - 1 );

    if( t.m_data->height < 2 ) return;

    size_t count = 0 ;
    bool ok = true;
    do
    {
        size_t index = dist( rng );
        ok = mutation( t , index , terminal_gen , unary_gen , binary_gen );
    }
    while( !ok && ( count < 128 ) );

    if( count == 128 ) throw std::runtime_error( "Maximal number of mutation trials reached" );
}

} // namespace gp

#endif // MUTATION_H_INCLUDED
