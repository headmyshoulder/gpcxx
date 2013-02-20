/*
 * mutation.cpp
 * Date: 2013-02-20
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include <gp/operator/mutation.hpp>
#include "../common/test_tree.hpp"
#include "../common/test_generator.hpp"

#include <gtest/gtest.h>

#include <iostream>

using namespace std;

TEST( operator_tests , mutation1 )
{
    test_tree tree;
    test_generator gen;

    bool not_equal_plus = false;
    auto root = tree.data.data();

    for( size_t i=0 ; i<100 ; ++i )
    {
        mutation( tree.data , 0 , gen.gen0 , gen.gen1 , gen.gen2 );
        if( root->value != "plus" ) not_equal_plus = true;
        EXPECT_TRUE( contains( gen.binary_symbols , root->value ) );
    }

    bool not_equal_sin = false;
    auto c1 = root->children[0];
    for( size_t i=0 ; i<100 ; ++i )
    {
        mutation( tree.data , 1 , gen.gen0 , gen.gen1 , gen.gen2 );
        if( c1->value != "sin" ) not_equal_sin = true;
        EXPECT_TRUE( contains( gen.unary_symbols , c1->value ) );
    }

    bool not_equal_x = false;
    auto c2 = c1->children[0];
    for( size_t i=0 ; i<100 ; ++i )
    {
        mutation( tree.data , 2 , gen.gen0 , gen.gen1 , gen.gen2 );
        if( c2->value != "x" ) not_equal_x = true;
        EXPECT_TRUE( contains( gen.term_symbols , c2->value ) );
    }
    EXPECT_TRUE( not_equal_x );
}

TEST( operator_tests , mutation2 )
{
    test_tree tree;
    test_generator gen;

    mutation( tree.data , gen.rng , gen.gen0 , gen.gen1 , gen.gen2 );
}
