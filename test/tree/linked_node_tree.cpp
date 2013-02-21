/*
 * linked_node_tree.cpp
 * Date: 2013-02-20
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include <gp/tree/linked_node_tree.hpp>

#include <gtest/gtest.h>

typedef gp::linked_node_tree< char > tree_type;
typedef tree_type::node_type node_type;

TEST( tree_tests , linked_node_tree1 )
{
    tree_type tree;
    EXPECT_EQ( true , true );
}


TEST( tree_tests , linked_node_tree_copy_empty )
{
    tree_type tree1;
    tree_type tree2( tree1 );
}

TEST( tree_tests , linked_node_tree_copy )
{
    tree_type tree1;
    tree1.set_data( new node_type( 'a' , new node_type( 'b' ) , new node_type( 'c' ) ) );
    tree_type tree2( tree1 );
}

TEST( tree_tests , linked_node_tree_assign_empty )
{
    tree_type tree1;
    tree_type tree2;
    tree2 = tree1;
}

TEST( tree_tests , linked_node_tree_assign )
{
    tree_type tree1;
    tree1.set_data( new node_type( 'a' , new node_type( 'b' ) , new node_type( 'c' ) ) );
    tree_type tree2;
    tree2 = tree1;
}

