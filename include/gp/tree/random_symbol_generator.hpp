/*
 * random_symbol_generator.hpp
 * Date: 2013-02-20
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef RANDOM_SYMBOL_GENERATOR_HPP_INCLUDED
#define RANDOM_SYMBOL_GENERATOR_HPP_INCLUDED

#include <vector>
#include <cassert>
#include <random>

namespace gp {


template< class Node , class Rng >
struct random_symbol_generator
{
    typedef Node node_type;
    typedef node_type* node_pointer;
    typedef typename Node::value_type value_type;


    random_symbol_generator( const std::vector< value_type > &symbols , Rng &rng , size_t arity )
        : m_symbols( symbols ) , m_rng( rng ) , m_arity( arity )
    {
        assert( !m_symbols.empty() );
    }

    node_pointer operator()( void )
    {
        return new node_type( random_symbol() );
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

#endif // RANDOM_SYMBOL_GENERATOR_HPP_INCLUDED
