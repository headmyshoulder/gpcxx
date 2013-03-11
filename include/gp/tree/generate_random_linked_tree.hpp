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
#include <array>

namespace gp {


struct generate_random_linked_tree
{
    template< class Tree , class TerminalGen , class UnaryGen , class BinaryGen , class Rng >
    void operator()( 
        Tree &t ,
        TerminalGen &terminal_gen , UnaryGen &unary_gen , BinaryGen &binary_gen , 
        Rng &rng , 
        size_t min_height , size_t max_height , const std::array< int , 3 > &gen_weights )
    {
        using namespace std;
        typedef typename Tree::node_type node_type;

        std::array< int , 2 > weights_dice = {{ gen_weights[1] , gen_weights[2] }};
        std::array< int , 3 > weights_thrice = gen_weights;

                                             
        std::discrete_distribution<> dice( weights_dice.begin() , weights_dice.end() );
        std::discrete_distribution<> thrice( weights_thrice.begin() , weights_thrice.end() );

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


    template< class Tree , class TerminalGen , class UnaryGen , class BinaryGen , class Rng >
    void operator()( 
        Tree &t ,
        TerminalGen &terminal_gen , UnaryGen &unary_gen , BinaryGen &binary_gen , 
        Rng &rng , 
        size_t min_height , size_t max_height )
    {
        std::array< int , 3 > weights = {{ 1 , 1 , 1 }};
        return (*this)( t , terminal_gen , unary_gen , binary_gen , rng , min_height , max_height , weights );
    }

};

template< class Rng , class TerminalGen , class UnaryGen , class BinaryGen >
struct tree_generator_binder
{
    Rng &m_rng;
    TerminalGen &m_gen0;
    UnaryGen &m_gen1;
    BinaryGen &m_gen2;
    size_t m_min_height , m_max_height;
    std::array< int , 3 > m_gen_weights;
    tree_generator_binder( Rng &rng , TerminalGen &gen0 , UnaryGen &gen1 , BinaryGen &gen2 , 
                           size_t min_height , size_t max_height )
        : m_rng( rng ) , m_gen0( gen0 ) , m_gen1( gen1 ) , m_gen2( gen2 ) ,
          m_min_height( min_height ) , m_max_height( max_height ) ,
          m_gen_weights( {{ 1 , 1 , 1 }} )
    {
    }

    tree_generator_binder( Rng &rng , TerminalGen &gen0 , UnaryGen &gen1 , BinaryGen &gen2 , 
                           size_t min_height , size_t max_height , std::array< int , 3 > &gen_weights )
        : m_rng( rng ) , m_gen0( gen0 ) , m_gen1( gen1 ) , m_gen2( gen2 ) ,
          m_min_height( min_height ) , m_max_height( max_height ) ,
          m_gen_weights( gen_weights )
    {
    }


    template< class Tree >
    void operator()( Tree &t ) const
    {
        generate_random_linked_tree()( t , m_gen0 , m_gen1 , m_gen2 , m_rng , m_min_height , m_max_height , m_gen_weights );
    }
};

template< class Rng , class TerminalGen , class UnaryGen , class BinaryGen >
tree_generator_binder< Rng , TerminalGen , UnaryGen , BinaryGen >
make_tree_generator_binder( Rng &rng , TerminalGen &gen0 , UnaryGen &gen1 , BinaryGen &gen2 , 
                           size_t min_height , size_t max_height )
{
    return tree_generator_binder< Rng , TerminalGen , UnaryGen , BinaryGen >( rng , gen0 , gen1 , gen2 , min_height , max_height );
}

template< class Rng , class TerminalGen , class UnaryGen , class BinaryGen >
tree_generator_binder< Rng , TerminalGen , UnaryGen , BinaryGen >
make_tree_generator_binder( Rng &rng , TerminalGen &gen0 , UnaryGen &gen1 , BinaryGen &gen2 , 
                            size_t min_height , size_t max_height , std::array< int , 3 > gen_weights )
{
    return tree_generator_binder< Rng , TerminalGen , UnaryGen , BinaryGen >( rng , gen0 , gen1 , gen2 , min_height , max_height , gen_weights );
}



} // namespace gp




#endif // GENERATE_RANDOM_TREE_H_INCLUDED
