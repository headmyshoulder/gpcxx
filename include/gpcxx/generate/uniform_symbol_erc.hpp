/*
  gpcxx/generate/uniform_symbol_erc.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GPCXX_GENERATE_UNIFORM_SYMBOL_ERC_HPP_DEFINED
#define GPCXX_GENERATE_UNIFORM_SYMBOL_ERC_HPP_DEFINED

#include <gpcxx/util/assert.hpp>

#include <boost/variant.hpp>

#include <random>


namespace gpcxx {
    
    
template< typename Result , typename ErcDist >
struct uniform_symbol_erc
{
    using erc_dist_type = ErcDist;
    using result_type   = Result;

    template< typename Symbols >
    uniform_symbol_erc( Symbols const& symbols , double prob_fraction_erc , erc_dist_type const& erc_dist )
    : m_symbols( symbols.begin() , symbols.end() ) , m_prob_fraction_erc( prob_fraction_erc ) , m_erc_dist( erc_dist )
    {
        GPCXX_ASSERT( !m_symbols.empty() );
        GPCXX_ASSERT( m_prob_fraction_erc > 0.0 );
    }

    template< typename Rng >
    result_type operator()( Rng &rng )
    {
        std::discrete_distribution< size_t > dist( { 1.0 , m_prob_fraction_erc } );
        if( dist( rng ) == 0 )
        {
            return random_symbol( rng );
        }
        else
        {
            return erc( rng );
        }
    }

    template< typename Rng >
    result_type random_symbol( Rng &rng ) const
    {
        GPCXX_ASSERT( !m_symbols.empty() );
        std::uniform_int_distribution< size_t > dist( 0 , m_symbols.size() - 1 );
        return m_symbols[ dist( rng ) ];
    }
    
    template< typename Rng >
    result_type erc( Rng &rng ) 
    {
        return m_erc_dist( rng );
    }

    size_t num_symbols( void ) const { return m_symbols.size(); }

private:

    std::vector< result_type > m_symbols;
    double m_prob_fraction_erc;
    erc_dist_type m_erc_dist;
};



template< typename Result , typename ErcDist , typename Symbols >
uniform_symbol_erc< Result , ErcDist >
make_uniform_symbol_erc(
    Symbols const& symbols ,
    double prob_fraction_erc ,
    ErcDist const& erc_dist )
{
    return uniform_symbol_erc< Result , ErcDist >( symbols , prob_fraction_erc , erc_dist );
}



} // namespace gpcxx


#endif // GPCXX_GENERATE_UNIFORM_SYMBOL_ERC_HPP_DEFINED
