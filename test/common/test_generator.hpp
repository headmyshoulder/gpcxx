/*
 * test_generator.hpp
 * Date: 2013-02-20
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef TEST_GENERATOR_HPP_INCLUDED
#define TEST_GENERATOR_HPP_INCLUDED

#include <gp/generate/random_symbol_generator.hpp>

#include <random>
#include <vector>
#include <string>
#include <algorithm>


template< class C , class S >
bool contains( const C &c , const S &s )
{
    return ( std::find( c.begin() , c.end() , s ) != c.end() );
}


struct test_generator
{
    typedef std::vector< std::string > symbol_container;
    typedef gp::random_symbol_generator< gp::linked_node< std::string > , std::mt19937 > generator_type;

    test_generator( void )
        : term_symbols( { "x" , "y" , "z" } ) ,
          unary_symbols(  { "sin" , "cos" , "exp" } ) ,
          binary_symbols( { "plus" , "minus" , "multiplies" } ) ,
          gen0( term_symbols , rng , 0 ) ,
          gen1( unary_symbols , rng , 1 ) ,
          gen2( binary_symbols , rng , 2 ) 
    { }

    std::mt19937 rng;
    symbol_container term_symbols;
    symbol_container unary_symbols;
    symbol_container binary_symbols;
    generator_type gen0;
    generator_type gen1;
    generator_type gen2;
};


#endif // TEST_GENERATOR_HPP_INCLUDED
