/*
 * basic_generate_strategy.hpp
 * Date: 2013-01-25
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef GPCXX_GENERATE_BASIC_GENERATE_STRATEGY_HPP_INCLUDED
#define GPCXX_GENERATE_BASIC_GENERATE_STRATEGY_HPP_INCLUDED

#include <random>
#include <stack>
#include <cassert>
#include <array>

namespace gpcxx {

    
template< typename Rng , typename TerminalGen , typename UnaryGen , typename BinaryGen >
class basic_generate_strategy
{
public:
    
    basic_generate_strategy( Rng &rng , TerminalGen &gen0 , UnaryGen &gen1 , BinaryGen &gen2 , 
                             size_t min_height , size_t max_height , std::array< double , 3 > const& gen_weights )
    : m_rng( rng ) , m_terminal_gen( gen0 ) , m_unary_gen( gen1 ) , m_binary_gen( gen2 ) ,
      m_min_height( min_height ) , m_max_height( max_height ) ,
      m_gen_weights( gen_weights ) { }
      
    template< typename Tree >
    typename Tree::cursor insert_node( Tree &tree , typename Tree::cursor current , size_t new_arity ) const
    {
        switch( new_arity )
        {
            case 0 : return tree.insert_below( current , m_terminal_gen( m_rng ) ); break;
            case 1 : return tree.insert_below( current , m_unary_gen( m_rng ) ); break;
            case 2 : return tree.insert_below( current , m_binary_gen( m_rng ) ); break;
        }
        return typename Tree::cursor();
    }

      
    template< class Tree >
    void operator()( Tree &tree ) const
    {
        typedef Tree tree_type;
        typedef typename tree_type::cursor cursor;


        std::array< double , 2 > weights_dice = {{ m_gen_weights[1] , m_gen_weights[2] }};
        std::array< double , 3 > weights_thrice = m_gen_weights;

                                             
        std::discrete_distribution<> dice( weights_dice.begin() , weights_dice.end() );
        std::discrete_distribution<> thrice( weights_thrice.begin() , weights_thrice.end() );

        // first - cursor to node, second - how many childs still have to be created
        std::stack< std::pair< cursor , size_t > > gen_stack; 

        // initialize
        cursor root = tree.insert_below( tree.root() , m_binary_gen( m_rng ) );
            
        gen_stack.push( std::make_pair( root , 2 ) );

        size_t height = 1;
        while( !gen_stack.empty() )
        {
            cursor current = gen_stack.top().first;
            if( current.size() == gen_stack.top().second )
            {
                gen_stack.pop();
                height--;
                continue;
            }

            ++height;

            size_t new_arity = ( height < m_min_height ) ? dice( m_rng ) + 1 : ( height < m_max_height ) ? thrice( m_rng ) : 0 ;
            cursor n = insert_node( tree , current , new_arity );

            if( new_arity > 0 ) gen_stack.push( std::make_pair( n , new_arity ) );
            else height--;
        }

        // t.make_consistent();
    }
    
private:
    
    Rng &m_rng;
    TerminalGen &m_terminal_gen;
    UnaryGen &m_unary_gen;
    BinaryGen &m_binary_gen;
    size_t m_min_height , m_max_height;
    std::array< double , 3 > m_gen_weights;
};


template< typename Rng , typename TerminalGen , typename UnaryGen , typename BinaryGen >
basic_generate_strategy< Rng , TerminalGen , UnaryGen , BinaryGen >
make_basic_generate_strategy( Rng &rng , TerminalGen &gen0 , UnaryGen &gen1 , BinaryGen &gen2 , 
                              size_t min_height , size_t max_height , std::array< double , 3 > const& gen_weights = {{ 1.0 , 1.0 , 1.0 }} )
{
    return basic_generate_strategy< Rng , TerminalGen , UnaryGen , BinaryGen >( rng , gen0 , gen1 , gen2 , min_height , max_height , gen_weights );
}


} // namespace gpcxx




#endif // GPCXX_GENERATE_BASIC_GENERATE_STRATEGY_HPP_INCLUDED
