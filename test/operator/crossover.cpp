/*
 * crossover.cpp
 * Date: 2013-02-20
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include <gp/operator/crossover.hpp>
#include "../common/test_tree.hpp"
#include "../common/test_generator.hpp"

#include <gtest/gtest.h>

using namespace std;

TEST( operator_tests , crossover1 )
{
    test_tree tree;
    test_generator gen;

    gp::crossover::crossover_impl( tree.data , tree.data2 , 3 , 1 );
    auto root1 = tree.data.data();
    EXPECT_EQ( root1->value , "plus" );
    EXPECT_EQ( root1->children[0]->value , "sin" );
    EXPECT_EQ( root1->children[1]->value , "cos" );
    auto root2 = tree.data2.data();
    EXPECT_EQ( root2->value , "minus" );
    EXPECT_EQ( root2->children[0]->value , "minus" );
    EXPECT_EQ( root2->children[1]->value , "x" );
}

TEST( operator_tests , crossover2 )
{
    test_tree tree;
    test_generator gen;

    gp::crossover::crossover_impl( tree.data , tree.data2 , 3 , 0 );
    auto root1 = tree.data.data();
    EXPECT_EQ( root1->value , "plus" );
    EXPECT_EQ( root1->children[0]->value , "sin" );
    EXPECT_EQ( root1->children[1]->value , "minus" );
    auto root2 = tree.data2.data();
    EXPECT_EQ( root2->value , "minus" );
    EXPECT_EQ( root2->children[0]->value , "y" );
    EXPECT_EQ( root2->children[1]->value , "2" );
}

TEST( operator_tests , crossover3 )
{
    test_tree tree;
    test_generator gen;

    gp::crossover::crossover_impl( tree.data , tree.data2 , 0 , 1 );
    auto root1 = tree.data.data();
    EXPECT_EQ( root1->value , "cos" );
    EXPECT_EQ( root1->children[0]->value , "y" );
    auto root2 = tree.data2.data();
    EXPECT_EQ( root2->value , "minus" );
    EXPECT_EQ( root2->children[0]->value , "plus" );
    EXPECT_EQ( root2->children[1]->value , "x" );
}

TEST( operator_tests , crossover4 )
{
    test_tree tree;
    test_generator gen;

    gp::crossover::crossover_impl( tree.data , tree.data2 , 0 , 0 );
    auto root1 = tree.data.data();
    EXPECT_EQ( root1->value , "minus" );
    EXPECT_EQ( root1->children[0]->value , "cos" );
    EXPECT_EQ( root1->children[1]->value , "x" );
    auto root2 = tree.data2.data();
    EXPECT_EQ( root2->value , "plus" );
    EXPECT_EQ( root2->children[0]->value , "sin" );
    EXPECT_EQ( root2->children[1]->value , "minus" );
}

TEST( operator_tests , crossover5 )
{
    test_tree tree;
    test_generator gen;
    gp::crossover()( tree.data , tree.data2 , gen.rng , 10 );

}
