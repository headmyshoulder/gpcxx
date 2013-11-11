/*
 * gpcxx/test/common/generic_generator.hpp
 * Date: 2013-10-30
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_TEST_COMMON_GENERIC_GENERATOR_HPP_INCLUDED
#define GPCXX_TEST_COMMON_GENERIC_GENERATOR_HPP_INCLUDED

#include "test_tree.hpp"
#include <gpcxx/generate/uniform_symbol.hpp>

#include <random>
#include <vector>
#include <string>
#include <algorithm>


template< typename TreeTag >
struct generic_generator
{
    typedef typename get_tree_type< TreeTag >::type tree_type;
    typedef typename get_node_factory< TreeTag >::type factory_type;
    typedef std::vector< typename tree_type::value_type > symbol_container;
    typedef gpcxx::uniform_symbol< typename tree_type::value_type > generator_type;




    generic_generator( void )
        : term_symbols( { m_factory( "x" ) , m_factory( "y" ) , m_factory( "z" ) } ) ,
          unary_symbols(  { m_factory( "sin" ) , m_factory( "cos" ) , m_factory( "exp" ) } ) ,
          binary_symbols( { m_factory( "plus" ) , m_factory( "minus" ) , m_factory( "multiplies" ) } ) ,
          gen0( term_symbols ) ,
          gen1( unary_symbols ) ,
          gen2( binary_symbols ) 
    { }

    factory_type m_factory;

    std::mt19937 rng;
    symbol_container term_symbols;
    symbol_container unary_symbols;
    symbol_container binary_symbols;
    generator_type gen0;
    generator_type gen1;
    generator_type gen2;
};


#endif // GPCXX_TEST_COMMON_GENERIC_GENERATOR_HPP_INCLUDED
