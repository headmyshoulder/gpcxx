/*
 * basic_generate_strategy.cpp
 * Date: 2013-02-21
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include <gpcxx/generate/basic_generate_strategy.hpp>
#include <gpcxx/tree/basic_tree.hpp>

#include "../common/test_generator.hpp"

#include <random>

#include <gtest/gtest.h>

#define TESTNAME basic_generate_strategy_tests


TEST( TESTNAME , generate_radnom_linked_tree_test1 )
{
    test_generator gen;

    std::mt19937 rng;
    std::array< double , 3 > weights{{ 1.0 , 1.0 , 1.0 }};

    for( size_t i=0 ; i<1000 ; ++i )
    {
        gpcxx::basic_tree< std::string > tree;
        
        auto generator = gpcxx::make_basic_generate_strategy( rng , gen.gen0 , gen.gen1 , gen.gen2 , 2 , 4 , weights );
        generator( tree );
        EXPECT_TRUE( tree.root().height() >= 2 );
        EXPECT_TRUE( tree.root().height() <= 4 );
    }
}

TEST( TESTNAME , generate_radnom_linked_tree_test2 )
{
    test_generator gen;

    std::mt19937 rng;

    for( size_t i=0 ; i<1000 ; ++i )
    {
        gpcxx::basic_tree< std::string > tree;
        
        auto generator = gpcxx::make_basic_generate_strategy( rng , gen.gen0 , gen.gen1 , gen.gen2 , 2 , 4 );
        generator( tree );
        EXPECT_TRUE( tree.root().height() >= 2 );
        EXPECT_TRUE( tree.root().height() <= 4 );
    }
}
