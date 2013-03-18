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
        T v = T();
        while( count < 1000  )
        {
            v = gen.random_symbol();
            if( ! ( v == t ) ) { return v; }
            ++count;
        }
        return v;
    }

    template< class Node , class TerminalGen , class UnaryGen , class BinaryGen >
    bool mutation_impl( Node *node , TerminalGen &terminal_gen , UnaryGen &unary_gen , BinaryGen &binary_gen )
    {
        typedef typename Node::value_type value_type;
        if( node->arity() == 0 )
        {
            node->value() = mutate_value( node->value() , terminal_gen );
            return true;
        }
        else if( node->arity() == 1 )
        {
            node->value() = mutate_value( node->value() , unary_gen );
            return false;
        }
        else
        {
            node->value() = mutate_value( node->value() , binary_gen );
            return true;
        }
    }


} // namespace detail


struct mutation
{

    template< class Tree , class TerminalGen , class UnaryGen , class BinaryGen >
    static bool mutation_impl( Tree &t , size_t i , TerminalGen &terminal_gen , UnaryGen &unary_gen , BinaryGen &binary_gen )
    {
        typedef typename Tree::node_type node_type;
        node_type &n = t[i];
        return detail::mutation_impl( &n , terminal_gen , unary_gen , binary_gen );
    }

    template< class Tree , class Rng , class TerminalGen , class UnaryGen , class BinaryGen >
    void operator()( Tree &t , Rng &rng , TerminalGen &terminal_gen , UnaryGen &unary_gen , BinaryGen &binary_gen )
    {
        typedef typename Tree::node_type node_type;

        std::uniform_int_distribution< size_t > dist( 0 , t.num_elements() - 1 );

        if( t.height() < 2 ) return;

        size_t count = 0 ;
        bool ok = true;
        do
        {
            size_t index = dist( rng );
            ok = mutation_impl( t , index , terminal_gen , unary_gen , binary_gen );
        }
        while( !ok && ( count < 128 ) );

        if( count == 128 ) throw std::runtime_error( "Maximal number of mutation trials reached" );
    }
};

template< class Rng , class TerminalGen , class UnaryGen , class BinaryGen >
struct mutation_binder
{
    Rng &m_rng;
    TerminalGen &m_gen1;
    UnaryGen &m_gen2;
    BinaryGen &m_gen3;
    mutation_binder( Rng &rng , TerminalGen &gen1 , UnaryGen &gen2 , BinaryGen &gen3 )
        : m_rng( rng ) , m_gen1( gen1 ) , m_gen2( gen2 ) , m_gen3( gen3 )
    { }

    template< class Tree >
    void operator()( Tree &t ) const
    {
        mutation()( t , m_rng , m_gen1 , m_gen2 , m_gen3 );
    }
};

template< class Rng , class TerminalGen , class UnaryGen , class BinaryGen >
mutation_binder< Rng , TerminalGen , UnaryGen , BinaryGen >
make_mutation_binder( Rng &rng , TerminalGen &gen1 , UnaryGen &gen2 , BinaryGen &gen3 )
{
    return mutation_binder< Rng , TerminalGen , UnaryGen , BinaryGen >( rng , gen1 , gen2 , gen3 );
}

} // namespace gp

#endif // MUTATION_H_INCLUDED
