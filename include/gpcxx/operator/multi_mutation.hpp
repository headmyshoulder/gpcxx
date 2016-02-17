/*
 * gpcxx/operator/multi_mutation.hpp
 * Date: 2016-02-02
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_OPERATOR_MULTI_MUTATION_HPP_INCLUDED
#define GPCXX_OPERATOR_MULTI_MUTATION_HPP_INCLUDED


#include <gpcxx/operator/detail/operator_base.hpp>
#include <gpcxx/util/assert.hpp>

#include <utility>
#include <vector>
#include <random>


namespace gpcxx {

template< typename Rng , typename Strategy , typename Selector >
class multi_mutation : public detail::operator_base< Strategy::arity >
{
public:
    
    multi_mutation( Rng& rng , Strategy strategy , Selector selector )
    : m_rng( rng ) , m_strategy( std::move( strategy ) ) , m_selector( std::move( selector ) ) { }

    template< typename Pop , typename Fitness >
    std::vector< typename Pop::value_type >
    operator()( Pop const& pop , Fitness const& fitness )
    {
        std::vector< typename Pop::value_type > nodes( 1 );
        nodes[0] = *( m_selector( pop , fitness ) );
        std::uniform_int_distribution< size_t > dist( 0 , nodes[0].size() - 1 );
        auto component_index = dist( m_rng );
        auto &tree = nodes[0][component_index];
        if( ! tree.empty() )
            m_strategy( tree );
        return nodes;
    }
    
    template< typename Pop , typename Fitness >
    std::vector< typename Pop::const_iterator >
    selection( Pop const& pop , Fitness const& fitness )
    {
        std::vector< typename Pop::const_iterator > s( 1 );
        s[0] = m_selector( pop , fitness );
        return s;
    }
    
    template< typename Selection >
    std::vector< typename std::iterator_traits< typename Selection::value_type >::value_type >
    operation( Selection const& selection )
    {
        GPCXX_ASSERT( selection.size() == 1 );
        std::vector< typename std::iterator_traits< typename Selection::value_type >::value_type > nodes( 1 );
        nodes[ 0 ] = *( selection[0] );
        std::uniform_int_distribution< size_t > dist( 0 , nodes[0].size() - 1 );
        auto component_index = dist( m_rng );
        auto& tree = nodes[0][component_index];
        if( ! tree.empty() )
            m_strategy( tree );
        return nodes;
    }


private:
    
    Rng& m_rng;
    Strategy m_strategy;
    Selector m_selector;
};

template< typename Rng , typename Strategy , typename Selector >
multi_mutation< Rng , Strategy , Selector > make_multi_mutation( Rng& rng , Strategy strategy , Selector selector )
{
    return multi_mutation< Rng , Strategy , Selector >( rng , std::move( strategy ) , std::move( selector ) );
}

} // namespace gpcxx

#endif // GPCXX_OPERATOR_MULTI_MUTATION_HPP_INCLUDED
