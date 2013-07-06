/*
 * random_symbol_generator.cpp
 * Date: 2013-02-20
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include <gp/generate/random_symbol_generator.hpp>
// #include <gp/tree/linked_node.hpp>
#include "../common/test_generator.hpp"

#include <gtest/gtest.h>



TEST( tree_tests , random_symbol_generator1 )
{
    test_generator gen;

    for( size_t i=0 ; i<100 ; ++i )
    {
        std::string s = gen.gen0.random_symbol();
        EXPECT_TRUE( contains( gen.term_symbols , s ) );
    }
    for( size_t i=0 ; i<100 ; ++i )
    {
        std::string s = gen.gen1.random_symbol();
        EXPECT_TRUE( contains( gen.unary_symbols , s ) );
    }
    for( size_t i=0 ; i<100 ; ++i )
    {
        std::string s = gen.gen2.random_symbol();
        EXPECT_TRUE( contains( gen.binary_symbols , s ) );
    }
}

TEST( tree_tests , random_symbol_generator2 )
{
    test_generator gen;

    for( size_t i=0 ; i<100 ; ++i )
    {
        auto n0 = gen.gen0();
        EXPECT_TRUE( contains( gen.term_symbols , n0->value() ) );
        delete n0;
    }

    for( size_t i=0 ; i<100 ; ++i )
    {
        auto n1 = gen.gen1();
        EXPECT_TRUE( contains( gen.unary_symbols , n1->value() ) );
        delete n1;
    }

    for( size_t i=0 ; i<100 ; ++i )
    {
        auto n2 = gen.gen2();
        EXPECT_TRUE( contains( gen.binary_symbols , n2->value() ) );
        delete n2;
    }
}



