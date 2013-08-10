/*
 * test_generator.hpp
 * Date: 2013-02-20
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef TEST_GENERATOR_HPP_INCLUDED
#define TEST_GENERATOR_HPP_INCLUDED

#include <gpcxx/generate/uniform_symbol.hpp>

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
    typedef gpcxx::uniform_symbol< std::string > generator_type;

    test_generator( void )
        : term_symbols( { "x" , "y" , "z" } ) ,
          unary_symbols(  { "sin" , "cos" , "exp" } ) ,
          binary_symbols( { "plus" , "minus" , "multiplies" } ) ,
          gen0( term_symbols ) ,
          gen1( unary_symbols ) ,
          gen2( binary_symbols ) 
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
