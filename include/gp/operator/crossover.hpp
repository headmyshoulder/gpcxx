/*
  gp/operator/crossover.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GP_OPERATOR_CROSSOVER_HPP_DEFINED
#define GP_OPERATOR_CROSSOVER_HPP_DEFINED

#include <random>

namespace gp {

    
template< typename Strategy , typename Selector >
class crossover
{
public:
    
    crossover( Strategy strategy , Selector selector )
    : m_strategy( strategy ) , m_selector( selector ) { }
    
    template< typename Pop , typename Fitness >
    std::pair< typename Pop::value_type , typename Pop::value_type >
    operator()( Pop &pop , Fitness &fitness )
    {
        typedef typename Pop::value_type individual_type;
        individual_type node1 = m_selector( pop , fitness );
        individual_type node2 = m_selector( pop , fitness );
        if( ( !node1.empty() ) && ( ! node2.empty() ) )
            m_strategy( node1 , node2 );
        return std::make_pair( node1 , node2 );
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

} // namespace gp


#endif // GP_OPERATOR_CROSSOVER_HPP_DEFINED
