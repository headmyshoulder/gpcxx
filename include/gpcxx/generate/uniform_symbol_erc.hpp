/*
  gpcxx/generate/uniform_symbol_erc.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GPCXX_GENERATE_UNIFORM_SYMBOL_ERC_HPP_DEFINED
#define GPCXX_GENERATE_UNIFORM_SYMBOL_ERC_HPP_DEFINED

#include <boost/variant.hpp>

#include <random>


namespace gpcxx {

template< typename T1 , typename T2 , typename ErcDist , typename Result = boost::variant< T1 , T2 > >
struct uniform_symbol_erc
{
    using symbol_type   = T1;
    using erc_type      = T2;
    using erc_dist_type = ErcDist;
    using result_type   = Result;


    uniform_symbol_erc( std::vector< symbol_type > const& symbols , double prob_fraction_erc , erc_dist_type const& erc_dist )
    : m_symbols( symbols ) , m_prob_fraction_erc( prob_fraction_erc ) , m_erc_dist( erc_dist )
    {
        assert( !m_symbols.empty() );
        assert( m_prob_fraction_erc > 0.0 );
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
    symbol_type random_symbol( Rng &rng ) const
    {
        assert( !m_symbols.empty() );
        std::uniform_int_distribution< size_t > dist( 0 , m_symbols.size() - 1 );
        return m_symbols[ dist( rng ) ];
    }
    
    template< typename Rng >
    erc_type erc( Rng &rng ) 
    {
        return m_erc_dist( rng );
    }

    size_t num_symbols( void ) const { return m_symbols.size(); }

private:

    std::vector< symbol_type > m_symbols;
    double m_prob_fraction_erc;
    erc_dist_type m_erc_dist;
};



template< typename T1 , typename ErcDist , typename Result = boost::variant< T1 , typename ErcDist::result_type > >
uniform_symbol_erc< T1 , typename ErcDist::result_type , ErcDist , Result >
make_uniform_symbol_erc(
    std::vector< T1 > const& symbols ,
    double prob_fraction_erc ,
    ErcDist const& erc_dist )
{
    return uniform_symbol_erc< T1 , typename ErcDist::result_type , ErcDist , Result >(
        symbols , prob_fraction_erc , erc_dist );
}



} // namespace gpcxx


#endif // GPCXX_GENERATE_UNIFORM_SYMBOL_ERC_HPP_DEFINED
