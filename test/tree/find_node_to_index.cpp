/*
 * find_node_to_index.cpp
 * Date: 2013-02-20
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include <gp/tree/find_node_to_index.hpp>
#include "../common/test_tree.hpp"

#include <gtest/gtest.h>

TEST( tree_tests , find_node_to_index1 )
{
    test_tree tree;
    auto *n = gp::find_node_to_index( tree.data.data() , 0 );
    EXPECT_EQ( n->value , "plus" );
    n = gp::find_node_to_index( tree.data.data() , 1 );
    EXPECT_EQ( n->value , "sin" );
    n = gp::find_node_to_index( tree.data.data() , 2 );
    EXPECT_EQ( n->value , "x" );
    n = gp::find_node_to_index( tree.data.data() , 3 );
    EXPECT_EQ( n->value , "minus" );
    n = gp::find_node_to_index( tree.data.data() , 4 );
    EXPECT_EQ( n->value , "y" );
    n = gp::find_node_to_index( tree.data.data() , 5 );
    EXPECT_EQ( n->value , "2" );

}

