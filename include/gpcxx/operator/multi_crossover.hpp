/*
 * gpcxx/operator/multi_crossover.hpp
 * Date: 2016-02-02
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_OPERATOR_MULTI_CROSSOVER_HPP_INCLUDED
#define GPCXX_OPERATOR_MULTI_CROSSOVER_HPP_INCLUDED


#include <gpcxx/operator/detail/operator_base.hpp>
#include <gpcxx/util/assert.hpp>

#include <utility>
#include <vector>
#include <random>


namespace gpcxx {

    
template< typename Rng , typename Strategy , typename Selector >
class multi_crossover : public detail::operator_base< Strategy::arity >
{
public:
    
    multi_crossover( Rng& rng , Strategy strategy , Selector selector )
    : m_rng( rng ) , m_strategy( std::move( strategy ) ) , m_selector( std::move( selector ) ) { }
    
    
    template< typename Pop , typename Fitness >
    std::vector< typename Pop::value_type >
    operator()( Pop const& pop , Fitness const& fitness )
    {
        auto sel = selection( pop , fitness );
        return operation( sel );
    }
    
    template< typename Pop , typename Fitness >
    std::vector< typename Pop::const_iterator >
    selection( Pop const& pop , Fitness const& fitness )
    {
        GPCXX_ASSERT( pop.size() > 2 );
        std::vector< typename Pop::const_iterator > s(2);
        s[1] = s[0] = m_selector( pop , fitness );
        while( s[0] == s[1] )
            s[1] = m_selector( pop , fitness );
        return s;
    }
    
    template< typename Selection >
    std::vector< typename std::iterator_traits< typename Selection::value_type >::value_type >
    operation( Selection const& selection )
    {
        GPCXX_ASSERT( selection.size() == 2 );
        std::vector< typename std::iterator_traits< typename Selection::value_type >::value_type > nodes( 2 );
        nodes[ 0 ] = *( selection[0] );
        nodes[ 1 ] = *( selection[1] );
        GPCXX_ASSERT( nodes[ 0 ].size() == nodes[ 1 ].size() );
        std::uniform_int_distribution< size_t > dist( 0 , nodes[0].size() - 1 );
        auto index1 = dist( m_rng );
        auto index2 = dist( m_rng );
        auto& tree1 = nodes[ 0 ][ index1 ];
        auto& tree2 = nodes[ 1 ][ index2 ];
        if( ( ! tree1.empty() ) && ( ! tree2.empty() ) )
            m_strategy( tree1 , tree2 );
        return nodes;
    }
    
private:

    Rng& m_rng;    
    Strategy m_strategy;
    Selector m_selector;
};

template< typename Rng , typename Strategy , typename Selector >
multi_crossover< Rng , Strategy , Selector > make_multi_crossover( Rng& rng , Strategy strategy , Selector selector )
{
    return multi_crossover< Rng , Strategy , Selector >( rng , std::move( strategy ) , std::move( selector ) );
}

} // namespace gpcxx


#endif // GPCXX_OPERATOR_MULTI_CROSSOVER_HPP_INCLUDED
