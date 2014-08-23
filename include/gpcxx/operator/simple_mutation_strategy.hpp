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

    template< class Cursor , class Generator >
    void simple_mutation_impl( Cursor node , Generator &generator )
    {
        typedef typename cursor_value< Cursor >::type value_type;
        *node = generator( node.size() );
    }


} // namespace detail


template< typename Rng , typename Generator >
struct simple_mutation_strategy
{
    Rng &m_rng;
    Generator &m_gen;

    simple_mutation_strategy( Rng &rng , Generator &gen )
    : m_rng( rng ) , m_gen( gen )
    { }

    template< class Tree , class Gen>
    static void mutation_impl( Tree &t , size_t i , Gen gen )
    {
        typedef typename Tree::cursor cursor;
        cursor n = t.rank_is( i );
        detail::simple_mutation_impl( n , gen );
    }

    template< class Tree >
    void operator()( Tree &t )
    {
        if( t.empty() ) return;
        if( t.root().height() < 2 ) return;
        std::uniform_int_distribution< size_t > dist( 0 , t.size() - 1 );

        size_t index = dist( m_rng );
        mutation_impl( t , index , [this]( size_t i ) {
            return m_gen.get_node2( m_rng , i ); } );
    }
};


template< class Rng , class Generator >
simple_mutation_strategy< Rng , Generator >
make_simple_mutation_strategy( Rng& rng , Generator& gen )
{
    return simple_mutation_strategy< Rng , Generator >( rng , gen );
}





} // namespace gpcxx


#endif // GPCXX_OPERATOR_SIMPLE_MUTATION_STRATEGY_HPP_DEFINED
