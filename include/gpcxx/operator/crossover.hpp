/*
  gpcxx/operator/crossover.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GPCXX_OPERATOR_CROSSOVER_HPP_DEFINED
#define GPCXX_OPERATOR_CROSSOVER_HPP_DEFINED

#include <utility>
#include <vector>
#include <cassert>

namespace gpcxx {

    
template< typename Strategy , typename Selector >
class crossover
{
public:
    
    crossover( Strategy strategy , Selector selector )
    : m_strategy( std::move( strategy ) ) , m_selector( std::move( selector ) ) { }
    
    
    template< typename Pop , typename Fitness >
    std::vector< typename Pop::value_type >
    operator()( Pop const& pop , Fitness const& fitness )
    {
        std::vector< typename Pop::value_type > nodes( 2 );
        nodes[ 0 ] = *( m_selector( pop , fitness ) );
        nodes[ 1 ] = *( m_selector( pop , fitness ) );
        if( ( ! nodes[0].empty() ) && ( ! nodes[1].empty() ) )
            m_strategy( nodes[0] , nodes[0] );
        return nodes;
    }
    
    template< typename Pop , typename Fitness >
    std::vector< typename Pop::const_iterator >
    selection( Pop const& pop , Fitness const& fitness )
    {
        std::vector< typename Pop::const_iterator > s(2);
        s[0] = m_selector( pop , fitness );
        s[1] = m_selector( pop , fitness );
        return s;
    }
    
    template< typename Selection >
    std::vector< typename std::iterator_traits< typename Selection::value_type >::value_type >
    operation( Selection const& selection )
    {
        assert( selection.size() == 2 );
        std::vector< typename std::iterator_traits< typename Selection::value_type >::value_type > nodes( 2 );
        nodes[ 0 ] = *( selection[0] );
        nodes[ 1 ] = *( selection[1] );
        if( ( ! nodes[0].empty() ) && ( ! nodes[1].empty() ) )
            m_strategy( nodes[0] , nodes[0] );
        return nodes;
    }
    
private:
    
    Strategy m_strategy;
    Selector m_selector;
};

template< typename Strategy , typename Selector >
crossover< Strategy , Selector > make_crossover( Strategy strategy , Selector selector )
{
    return crossover< Strategy , Selector >( std::move( strategy ) , std::move( selector ) );
}

} // namespace gpcxx


#endif // GPCXX_OPERATOR_CROSSOVER_HPP_DEFINED
