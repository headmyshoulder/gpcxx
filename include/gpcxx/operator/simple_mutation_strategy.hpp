/*
  gpcxx/operator/simple_mutation_strategy.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GPCXX_OPERATOR_SIMPLE_MUTATION_STRATEGY_HPP_DEFINED
#define GPCXX_OPERATOR_SIMPLE_MUTATION_STRATEGY_HPP_DEFINED

#include <gpcxx/tree/cursor_traits.hpp>

#include <random>
#include <stdexcept>
#include <functional>

namespace gpcxx {

namespace detail {

    template< class T , class Gen >
    T mutate_value( T t , Gen &gen , size_t max_trials = 1000 )
    {
        size_t count = 0;
        T v = gen();
        while( ( v == t ) && ( count < max_trials ) )
        {
            v = gen();
            ++count;
        }
        return v;
    }

    template< class Cursor , class TerminalGen , class UnaryGen , class BinaryGen >
    bool simple_mutation_impl( Cursor node , TerminalGen &terminal_gen , UnaryGen &unary_gen , BinaryGen &binary_gen )
    {
        typedef typename cursor_value< Cursor >::type value_type;
        if( node.size() == 0 )
        {
            *node = mutate_value( *node , terminal_gen );
            return true;
        }
        else if( node.size() == 1 )
        {
            *node = mutate_value( *node , unary_gen );
            return true;
        }
        else
        {
            *node = mutate_value( *node , binary_gen );
            return true;
        }
    }


} // namespace detail


template< typename Rng , typename TerminalGen , typename UnaryGen , typename BinaryGen >
struct simple_mutation_strategy
{
    Rng &m_rng;
    TerminalGen &m_terminal_gen;
    UnaryGen &m_unary_gen;
    BinaryGen &m_binary_gen;

    simple_mutation_strategy( Rng &rng , TerminalGen &terminal_gen , UnaryGen &unary_gen , BinaryGen &binary_gen )
    : m_rng( rng ) , m_terminal_gen( terminal_gen ) , m_unary_gen( unary_gen ) , m_binary_gen( binary_gen )
    { }

    template< class Tree , class TG , class UG , class BG >
    static bool mutation_impl( Tree &t , size_t i , TG terminal_gen , UG unary_gen , BG binary_gen )
    {
        typedef typename Tree::cursor cursor;
        cursor n = t.rank_is( i );
        return detail::simple_mutation_impl( n , terminal_gen , unary_gen , binary_gen );
    }

    template< class Tree >
    void operator()( Tree &t )
    {
        if( t.empty() ) return;
        if( t.root().height() < 2 ) return;
        std::uniform_int_distribution< size_t > dist( 0 , t.size() - 1 );



        size_t count = 0 ;
        bool ok = true;
        do
        {
            size_t index = dist( m_rng );
            ok = mutation_impl( t , index , std::bind( m_terminal_gen , std::ref( m_rng ) ) ,
                                            std::bind( m_unary_gen , std::ref( m_rng ) ) ,
                                            std::bind( m_binary_gen , std::ref( m_rng ) ) );
        }
        while( !ok && ( count < 128 ) );

        if( count == 128 ) throw std::runtime_error( "Maximal number of mutation trials reached" );
    }
};


template< class Rng , class TerminalGen , class UnaryGen , class BinaryGen >
simple_mutation_strategy< Rng , TerminalGen , UnaryGen , BinaryGen >
make_simple_mutation_strategy( Rng &rng , TerminalGen &gen1 , UnaryGen &gen2 , BinaryGen &gen3 )
{
    return simple_mutation_strategy< Rng , TerminalGen , UnaryGen , BinaryGen >( rng , gen1 , gen2 , gen3 );
}





} // namespace gpcxx


#endif // GPCXX_OPERATOR_SIMPLE_MUTATION_STRATEGY_HPP_DEFINED
