/*
 * linked_node_tree.cpp
 * Date: 2013-02-20
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include <gp/tree/linked_node_tree.hpp>

#include <gtest/gtest.h>

using namespace std;

TEST( tree_tests , linked_node_tree1 )
{
    gp::linked_node_tree< char > tree1;
    
    EXPECT_EQ( true , true );
}

