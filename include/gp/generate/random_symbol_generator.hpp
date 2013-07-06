/*
 * random_symbol_generator.hpp
 * Date: 2013-02-20
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef GP_GENERATE_RANDOM_SYMBOL_GENERATOR_HPP_INCLUDED
#define GP_GENERATE_RANDOM_SYMBOL_GENERATOR_HPP_INCLUDED

#include <vector>
#include <cassert>
#include <random>

namespace gp {


template< typename T , typename Rng >
struct random_symbol_generator
{
    typedef T value_type;


    random_symbol_generator( const std::vector< value_type > &symbols , Rng &rng , size_t arity )
        : m_symbols( symbols ) , m_rng( rng ) , m_arity( arity )
    {
        assert( !m_symbols.empty() );
    }

    vale_type operator()( void )
    {
        return random_symbol();
    }

    value_type random_symbol( void )
    {
        assert( !m_symbols.empty() );
        std::uniform_int_distribution< size_t > dist( 0 , m_symbols.size() - 1 );
        return m_symbols[ dist( m_rng ) ];
    }

    explicit operator bool () const { return !m_symbols.empty(); }

    size_t num_symbols( void ) const { return m_symbols.size(); }

private:

    std::vector< value_type > m_symbols;
    Rng &m_rng;
    size_t m_arity;
};





} // namespace gp

#endif // GP_GENERATE_RANDOM_SYMBOL_GENERATOR_HPP_INCLUDED
