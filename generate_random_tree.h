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

    tree_node< T >* operator()( void )
    {
        tree_node< T >* n = new tree_node< T >( random_symbol() , m_arity );
        return n;
    }

    T random_symbol( void )
    {
        std::uniform_int_distribution< size_t > dist( 0 , m_symbols.size() - 1 );
        return m_symbols[ dist( m_rng ) ];
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

    std::uniform_int_distribution<> dice( 0 , 1 );
    std::uniform_int_distribution<> thrice( 0 , 2 );

    // first - index of node
    // first - how many childs still have to be created
    stack< pair< node_type* , size_t > > gen_stack; 

    // initialize
    node_type *n = binary_gen();
    t.m_data = n;
    gen_stack.push( make_pair( n , 0 ) );

    size_t height = 0;

    while( !gen_stack.empty() )
    {
        if( gen_stack.top().first->arity == gen_stack.top().second )
        {
            gen_stack.pop();
            height--;
            continue;
        }

        ++height;

        node_type *n;
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

        gen_stack.top().first->children[ gen_stack.top().second ] = n;
        gen_stack.top().second++;

        if( n->arity > 0 ) gen_stack.push( make_pair( n , 0 ) );
        else height--;
    }


    complete_tree_structure( t );
}






#endif // GENERATE_RANDOM_TREE_H_INCLUDED
