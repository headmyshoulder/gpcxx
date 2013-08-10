/*
 * random_symbol_generator.cpp
 * Date: 2013-02-20
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include <gpcxx/generate/uniform_symbol.hpp>

#include "../common/test_generator.hpp"

#include <gtest/gtest.h>

#define TESTNAME random_symbol_generator_tests



TEST( TESTNAME , random_symbol_generator1 )
{
    test_generator gen;

    for( size_t i=0 ; i<100 ; ++i )
    {
        std::string s = gen.gen0( gen.rng );
        EXPECT_TRUE( contains( gen.term_symbols , s ) );
    }
    for( size_t i=0 ; i<100 ; ++i )
    {
        std::string s = gen.gen1( gen.rng );
        EXPECT_TRUE( contains( gen.unary_symbols , s ) );
    }
    for( size_t i=0 ; i<100 ; ++i )
    {
        std::string s = gen.gen2( gen.rng );
        EXPECT_TRUE( contains( gen.binary_symbols , s ) );
    }
}
