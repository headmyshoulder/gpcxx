/*
 * generate_random_tree.h
 * Date: 2013-01-25
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef GENERATE_RANDOM_TREE_H_INCLUDED
#define GENERATE_RANDOM_TREE_H_INCLUDED

#include "tree.h"
#include "complete_tree_structure.h"

#include <random>
#include <stack>
#include <cassert>

template< class T , class Rng >
struct random_symbol_generator
{
    std::vector< T > m_symbols;
    Rng &m_rng;
    size_t m_arity;


    random_symbol_generator( const std::vector< T > &symbols , Rng &rng , size_t arity )
        : m_symbols( symbols ) , m_rng( rng ) , m_arity( arity ) { assert( !m_symbols.empty() ); }

    node< T > operator()( void )
    {
        std::uniform_int_distribution< size_t > dist( 0 , m_symbols.size() - 1 );
        node< T > n;
        n.arity = m_arity;
        n.value = m_symbols[ dist( m_rng ) ];
        return n;
    }
};


template< class Tree , class UnaryGen , class BinaryGen , class TerminalGen , class Rng >
void generate_random_tree(
    Tree &t ,
    UnaryGen &unary_gen , BinaryGen &binary_gen , TerminalGen &terminal_gen ,
    Rng &rng , 
    size_t min_height , size_t max_height )
{
    using namespace std;
    typedef typename Tree::node_type node_type;
    typedef typename Tree::vector_type vector_type;

    vector_type &data = t.data();

    std::uniform_int_distribution<> dice( 0 , 1 );
    std::uniform_int_distribution<> thrice( 0 , 2 );

    // first - index of node
    // first - how many childs still have to be created
    stack< pair< size_t , size_t > > gen_stack; 

    // initialize
    data.push_back( binary_gen() );
    gen_stack.push( make_pair( 0 , 0 ) );
    size_t count = 0;
    size_t height = 0;

    while( !gen_stack.empty() )
    {
        if( data[ gen_stack.top().first ].arity == gen_stack.top().second )
        {
            gen_stack.pop();
            height--;
            continue;
        }

        ++count;
        ++height;

        node_type n;
        if( height < min_height )
        {
            if( dice( rng ) == 0 ) n = unary_gen();
            else n = binary_gen();
        }
        else if( height < max_height )
        {
            int r = thrice( rng );
            if( r == 0 ) n = terminal_gen();
            else if( r == 1 ) n = unary_gen();
            else n = binary_gen();
        }
        else 
        {
            n = terminal_gen();
        }

        if( gen_stack.top().second > 0 )
            data[ gen_stack.top().first ].child_index[ gen_stack.top().second - 1 ] = count;

        gen_stack.top().second++;

        if( n.arity > 0 ) gen_stack.push( make_pair( count , 0 ) );
        else height--;
        data.push_back( n );
    }


    complete_tree_structure( t );
}






#endif // GENERATE_RANDOM_TREE_H_INCLUDED
