/*
 * generate_random_linked_tree.h
 * Date: 2013-01-25
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef GENERATE_RANDOM_LINKED_TREE_H_INCLUDED
#define GENERATE_RANDOM_LINKED_TREEE_H_INCLUDED

#include <gp/tree/complete_linked_tree_structure.hpp>

#include <random>
#include <stack>
#include <cassert>

namespace gp {

template< class Tree , class TerminalGen , class UnaryGen , class BinaryGen , class Rng >
void generate_random_linked_tree(
    Tree &t ,
    TerminalGen &terminal_gen , UnaryGen &unary_gen , BinaryGen &binary_gen , 
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
    t.set_data( n );
    gen_stack.push( make_pair( n , 0 ) );

    size_t height = 1;

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
            // n = binary_gen();
        }
        else if( height < max_height )
        {
            int r = thrice( rng );
            if( r == 0 ) n = terminal_gen();
            else if( r == 1 ) n = unary_gen();
            else n = binary_gen();
            // if( dice( rng ) == 0 ) n = terminal_gen();
            // else n = binary_gen();
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


    complete_linked_tree_structure( t );
}



} // namespace gp




#endif // GENERATE_RANDOM_TREE_H_INCLUDED
