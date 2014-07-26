/*
  gpcxx/operator/tournament_selector.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GPCXX_OPERATOR_TOURNAMENT_SELECTOR_HPP_DEFINED
#define GPCXX_OPERATOR_TOURNAMENT_SELECTOR_HPP_DEFINED

#include <random>
#include <cassert>

namespace gpcxx {


template< typename Rng >
class tournament_selector
{
public:
    
    tournament_selector( Rng &rng , size_t tournament_size )
    : m_rng( rng ) , m_tournament_size( tournament_size ) { }
    
    template< typename Pop , typename Fitness >
    typename Pop::value_type const&
    operator()( Pop const& pop , Fitness const & fitness ) const
    {
        typedef typename Pop::value_type individual_type;
        
        assert( pop.size() == fitness.size() );
        assert( m_tournament_size > 0 );
        assert( pop.size() > 0 );
        
        std::uniform_int_distribution< size_t > dist( 0 , pop.size()-1 );
        
        size_t index = dist( m_rng );
        individual_type const* best = &( pop[ index ] );
        typename Pop::value_type const& best_value = fitness[ index ];
        
        for( size_t i=1 ; i<m_tournament_size ; ++i )
        {
            size_t index = dist( m_rng );
            if( fitness[index] < best_value )
            {
                best = &( pop[index] );
                best_value = fitness[index];
            }
            return *best;
        }
        
    }
    
private:
    
    Rng &m_rng;
    size_t m_tournament_size;
};

template< typename Rng >
tournament_selector< Rng > make_tournament_selector( Rng &rng , size_t tournament_size )
{
    return tournament_selector< Rng >( rng , tournament_size );
}


} // namespace gpcxx


#endif // GPCXX_OPERATOR_TOURNAMENT_SELECTOR_HPP_DEFINED
