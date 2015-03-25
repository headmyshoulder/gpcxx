/*
  gpcxx/operator/random_selector.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GPCXX_OPERATOR_RANDOM_SELECTOR_HPP_DEFINED
#define GPCXX_OPERATOR_RANDOM_SELECTOR_HPP_DEFINED

#include <random>
#include <iterator>

namespace gpcxx {

    
template< typename Rng >
class random_selector
{
public:
    
    random_selector( Rng &rng ) : m_rng( rng ) { }
    
    template< typename Pop , typename Fitness >
    typename Pop::const_iterator 
    operator()( Pop const& pop , Fitness const& fitness ) const
    {
        size_t n = pop.size();
        std::uniform_int_distribution< size_t > dist( 0 , n-1 );
        return std::next( std::begin( pop ) , dist( m_rng ) );
    }
    
private:

    Rng &m_rng;
};


template< typename Rng >
random_selector< Rng > make_random_selector( Rng &rng )
{
    return random_selector< Rng >( rng );
}


} // namespace gpcxx


#endif // GPCXX_OPERATOR_RANDOM_SELECTOR_HPP_DEFINED
