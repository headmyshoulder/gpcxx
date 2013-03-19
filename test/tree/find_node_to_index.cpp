/*
 * find_node_to_index.cpp
 * Date: 2013-02-20
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include <gp/tree/linked_node.hpp>
#include "../common/test_tree.hpp"

#include <gtest/gtest.h>

TEST( tree_tests , find_node_to_index1 )
{
    test_tree tree;

    auto &n1 = tree.data.at( 0 );
    EXPECT_EQ( n1.value() , "plus" );
    EXPECT_EQ( tree.data[0].value() , "plus" );
    auto &n2 = tree.data.at( 1 );
    EXPECT_EQ( n2.value() , "sin" );
    EXPECT_EQ( tree.data[1].value() , "sin" );
    auto &n3 = tree.data.at( 2 );
    EXPECT_EQ( n3.value() , "x" );
    EXPECT_EQ( tree.data[2].value() , "x" );
    auto &n4 = tree.data.at( 3 );
    EXPECT_EQ( n4.value() , "minus" );
    EXPECT_EQ( tree.data[3].value() , "minus" );
    auto &n5 = tree.data.at( 4 );
    EXPECT_EQ( n5.value() , "y" );
    EXPECT_EQ( tree.data[4].value() , "y" );
    auto &n6 = tree.data.at( 5 );
    EXPECT_EQ( n6.value() , "2" );
    EXPECT_EQ( tree.data[5].value() , "2" );
}

TEST( tree_tests , find_node_to_index2 )
{
    const test_tree tree;

    auto &n1 = tree.data.at( 0 );
    EXPECT_EQ( n1.value() , "plus" );
    EXPECT_EQ( tree.data[0].value() , "plus" );
    auto &n2 = tree.data.at( 1 );
    EXPECT_EQ( n2.value() , "sin" );
    EXPECT_EQ( tree.data[1].value() , "sin" );
    auto &n3 = tree.data.at( 2 );
    EXPECT_EQ( n3.value() , "x" );
    EXPECT_EQ( tree.data[2].value() , "x" );
    auto &n4 = tree.data.at( 3 );
    EXPECT_EQ( n4.value() , "minus" );
    EXPECT_EQ( tree.data[3].value() , "minus" );
    auto &n5 = tree.data.at( 4 );
    EXPECT_EQ( n5.value() , "y" );
    EXPECT_EQ( tree.data[4].value() , "y" );
    auto &n6 = tree.data.at( 5 );
    EXPECT_EQ( n6.value() , "2" );
    EXPECT_EQ( tree.data[5].value() , "2" );
}
