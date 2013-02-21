/*
 * linked_node.cpp
 * Date: 2013-02-20
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include <gp/tree/linked_node.hpp>

#include <gtest/gtest.h>

using namespace std;

typedef gp::linked_node< char > node_type;

TEST( tree_tests , linked_node_copy_empty )
{
    node_type l1( 'a' , 2 );
    node_type l2( l1 );
}

TEST( tree_tests , linked_node_copy )
{
    node_type l1( 'a' , new node_type( 'b' ) , new node_type( 'c' ) );
    node_type l2;
    l2 = l1;
}

TEST( tree_tests , linked_node_assign_empty )
{
    node_type l1( 'a' , 2 );
    node_type l2( l1 );
}

TEST( tree_tests , linked_node_assign )
{
    node_type l1( 'a' , new node_type( 'b' ) , new node_type( 'c' ) );
    node_type l2;
    l2 = l1;
}

