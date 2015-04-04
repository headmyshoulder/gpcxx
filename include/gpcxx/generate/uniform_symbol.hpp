/*
 * uniform_symbol.hpp
 * Date: 2013-02-20
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef GPCXX_GENERATE_UNIFORM_SYMBOL_HPP_INCLUDED
#define GPCXX_GENERATE_UNIFORM_SYMBOL_HPP_INCLUDED

#include <vector>
#include <cassert>
#include <random>

namespace gpcxx {


template< typename T >
struct uniform_symbol
{
    typedef T value_type;


    uniform_symbol( const std::vector< value_type >& symbols )
    : m_symbols( symbols )
    {
        assert( !m_symbols.empty() );
    }
    
    uniform_symbol( std::vector< value_type >&& symbols )
    : m_symbols( std::move( symbols ) )
    {
        assert( !m_symbols.empty() );
    }

    template< typename Rng >
    value_type operator()( Rng &rng ) const
    {
        assert( !m_symbols.empty() );
        std::uniform_int_distribution< size_t > dist( 0 , m_symbols.size() - 1 );
        return m_symbols[ dist( rng ) ];
    }

    size_t num_symbols( void ) const { return m_symbols.size(); }

private:

    std::vector< value_type > m_symbols;
};


template< typename T  >
uniform_symbol< T > make_uniform_symbol( std::vector< T > const& symbols )
{
    return uniform_symbol< T >( symbols );
}



} // namespace gpcxx

#endif // GPCXX_GENERATE_UNIFORM_SYMBOL_HPP_INCLUDED
