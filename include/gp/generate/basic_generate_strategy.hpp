/*
 * basic_generate_strategy.hpp
 * Date: 2013-01-25
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef GP_GENERATE_BASIC_GENERATE_STRATEGY_HPP_INCLUDED
#define GP_GENERATE_BASIC_GENERATE_STRATEGY_HPP_INCLUDED

#include <random>
#include <stack>
#include <cassert>
#include <array>

namespace gp {

    
template< typename Rng , typename TerminalGen , typename UnaryGen , typename BinaryGen >
class basic_generate_strategy
{
public:
    
     basic_generate_strategy( Rng &rng , TerminalGen &gen0 , UnaryGen &gen1 , BinaryGen &gen2 , 
                              size_t min_height , size_t max_height , std::array< int , 3 > const& gen_weights )
     : m_rng( rng ) , m_terminal_gen( gen0 ) , m_unary_gen( gen1 ) , m_binary_gen( gen2 ) ,
       m_min_height( min_height ) , m_max_height( max_height ) ,
      m_gen_weights( gen_weights ) { }
      
    template< class Tree >
    void operator()( Tree &t ) const
    {
        typedef Tree node_type;

        std::array< int , 2 > weights_dice = {{ m_gen_weights[1] , m_gen_weights[2] }};
        std::array< int , 3 > weights_thrice = m_gen_weights;

                                             
        std::discrete_distribution<> dice( weights_dice.begin() , weights_dice.end() );
        std::discrete_distribution<> thrice( weights_thrice.begin() , weights_thrice.end() );

        // first - index of node
        // first - how many childs still have to be created
        std::stack< std::pair< node_type* , size_t > > gen_stack; 

        // initialize
        t.value() = m_binary_gen.random_symbol();
            
        gen_stack.push( std::make_pair( &t , 2 ) );

        size_t height = 1;

        while( !gen_stack.empty() )
        {
            node_type *current = gen_stack.top().first;
            if( current->arity() == gen_stack.top().second )
            {
                gen_stack.pop();
                height--;
                continue;
            }

            ++height;


            node_type *n = nullptr;
            size_t new_arity = 0;
            if( height < m_min_height )
            {
                if( dice( m_rng ) == 0 ) { n = &( *current->emplace_inconsistent( m_unary_gen.random_symbol() ) ) ; new_arity = 1; }
                else { n = &( *current->emplace_inconsistent( m_binary_gen.random_symbol() ) ) ; new_arity = 2; }
            }
            else if( height < m_max_height )
            {
                int r = thrice( m_rng );
                if( r == 0 ) { n = &( *current->emplace_inconsistent( m_terminal_gen.random_symbol() ) ); new_arity = 0; }
                else if( r == 1 ) { n = &( *current->emplace_inconsistent( m_unary_gen.random_symbol() ) ); new_arity = 1; }
                else { n = &( *current->emplace_inconsistent( m_binary_gen.random_symbol() ) ); new_arity = 2; }
            }
            else 
            {
                n = &( *current->emplace_inconsistent( m_terminal_gen.random_symbol() ) ); new_arity = 0;
            }

            if( new_arity > 0 ) gen_stack.push( std::make_pair( n , new_arity ) );
            else height--;
        }

        t.make_consistent();
    }
    
private:
    
    Rng &m_rng;
    TerminalGen &m_terminal_gen;
    UnaryGen &m_unary_gen;
    BinaryGen &m_binary_gen;
    size_t m_min_height , m_max_height;
    std::array< int , 3 > m_gen_weights;
};


template< typename Rng , typename TerminalGen , typename UnaryGen , typename BinaryGen >
basic_generate_strategy< Rng , TerminalGen , UnaryGen , BinaryGen >
make_basic_generate_strategy( Rng &rng , TerminalGen &gen0 , UnaryGen &gen1 , BinaryGen &gen2 , 
                              size_t min_height , size_t max_height , std::array< int , 3 > const& gen_weights )
{
    return basic_generate_strategy< Rng , TerminalGen , UnaryGen , BinaryGen >( rng , gen0 , gen1 , gen2 , min_height , max_height , gen_weights );
}


} // namespace gp




#endif // GP_GENERATE_BASIC_GENERATE_STRATEGY_HPP_INCLUDED
