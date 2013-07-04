/*
 test/tree/basic_tree.cpp

 Copyright 2013 Karsten Ahnert

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gp/tree/basic_tree.hpp>
#include <gp/tree/detail/basic_node_inspect.hpp>

#include "../common/test_tree.hpp"

#include <gtest/gtest.h>

#include <sstream>

using namespace std;
using namespace gp;


// detail::inspect_basic_node( clog , root.parent_node() , 0 );

#define TEST_NODE( n , VALUE , ARITY ) \
    EXPECT_EQ( *(n) , VALUE );                                   \
    EXPECT_EQ( (n).size() , ARITY );



TEST( basic_tree_tests , default_construct )
{
    basic_tree< int > tree;
    auto root = tree.root();
    EXPECT_EQ( tree.size() , 0 );
    EXPECT_TRUE( tree.empty() );
    
    
}

TEST( basic_tree_tests , insert_value )
{
    basic_tree< std::string > tree;
    auto root = tree.root();
    basic_tree< std::string >::cursor n1 = tree.insert_below( root , "+" );
    
    EXPECT_EQ( tree.size() , 1 );
    EXPECT_FALSE( tree.empty() );
    TEST_NODE( n1 , "+" , 0 );
    TEST_NODE( tree.root() , "+" , 0 );

    tree.insert_below( n1 , "11" );
    tree.insert_below( n1 , "12" );

    EXPECT_FALSE( tree.empty() );
    EXPECT_EQ( tree.size() , 3 );
    TEST_NODE( tree.root() , "+" , 2 );
    TEST_NODE( tree.root().children(0) , "11" , 0 );
    TEST_NODE( tree.root().children(1) , "12" , 0 );
}

TEST( basic_tree_tests , insert_and_erase )
{
    basic_tree< std::string > tree;
    tree.insert_below( tree.root() , "+" );
    auto n1 = tree.insert_below( tree.root() , "-" );
    auto n2 = tree.insert_below( tree.root() , "*" );
    auto n3 = tree.insert_below( n1 , "13" );
    auto n4 = tree.insert_below( n1 , "14" );
    auto n5 = tree.insert_below( n2 , "15" );
    auto n6 = tree.insert_below( n2 , "16" );    
    
    EXPECT_EQ( tree.size() , 7 );
    EXPECT_FALSE( tree.empty() );
    TEST_NODE( tree.root() , "+" , 2 );
    TEST_NODE( tree.root().children(0) , "-" , 2 );
    TEST_NODE( tree.root().children(0).children(0) , "13" , 0 );
    TEST_NODE( tree.root().children(0).children(1) , "14" , 0 );
    TEST_NODE( tree.root().children(1) , "*" , 2 );
    TEST_NODE( tree.root().children(1).children(0) , "15" , 0 );
    TEST_NODE( tree.root().children(1).children(1) , "16" , 0 );
    
    tree.erase( n2 );
    
    EXPECT_EQ( tree.size() , 4 );
    EXPECT_FALSE( tree.empty() );
    TEST_NODE( tree.root() , "+" , 1 );
    TEST_NODE( tree.root().children(0) , "-" , 2 );
    TEST_NODE( tree.root().children(0).children(0) , "13" , 0 );
    TEST_NODE( tree.root().children(0).children(1) , "14" , 0 );
}


TEST( basic_tree_tests , insert_cursor )
{
    test_tree< basic_tree_tag > trees;
    test_tree< basic_tree_tag >::tree_type::cursor c = trees.data.root().children(1);
    trees.data.erase( c.children(1) );
    test_tree< basic_tree_tag >::tree_type::cursor c2 = trees.data2.root();
    trees.data.insert_below( c , c2 );
    
    EXPECT_EQ( trees.data.size() , 9 );
    EXPECT_FALSE( trees.data.empty() );
    EXPECT_EQ( trees.data2.size() , 4 );
    EXPECT_FALSE( trees.data2.empty() );
    
    TEST_NODE( trees.data.root() , "plus" , 2 );
    TEST_NODE( trees.data.root().children(0) , "sin" , 1 );
    TEST_NODE( trees.data.root().children(0).children(0) , "x" , 0 );
    TEST_NODE( trees.data.root().children(1) , "minus" , 2 );
    TEST_NODE( trees.data.root().children(1).children(0) , "y" , 0 );
    TEST_NODE( trees.data.root().children(1).children(1) , "minus" , 2 );
    TEST_NODE( trees.data.root().children(1).children(1).children(0) , "cos" , 1 );
    TEST_NODE( trees.data.root().children(1).children(1).children(0).children(0) , "y" , 0 );
    TEST_NODE( trees.data.root().children(1).children(1).children(1) , "x" , 0 );
}


TEST( basic_tree_tests , assign )
{
    test_tree< basic_tree_tag > trees;
    trees.data.assign( trees.data2.root() );
    
    EXPECT_EQ( trees.data.size() , 4 );
    EXPECT_FALSE( trees.data.empty() );
    TEST_NODE( trees.data.root() , "minus" , 2 );
    TEST_NODE( trees.data.root().children(0) , "cos" , 1 );
    TEST_NODE( trees.data.root().children(0).children(0) , "y" , 0 );
    TEST_NODE( trees.data.root().children(1) , "x" , 0 );
}

TEST( basic_tree_tests , clear )
{
    test_tree< basic_tree_tag > trees;
    trees.data.clear();
    
    EXPECT_EQ( trees.data.size() , 0 );
    EXPECT_TRUE( trees.data.empty() );
}

TEST( basic_tree_tests , iterator_construct )
{
    test_tree< basic_tree_tag > trees;
    basic_tree< std::string > t( trees.data2.root() );
    
    EXPECT_EQ( t.size() , 4 );
    EXPECT_FALSE( t.empty() );
    TEST_NODE( t.root() , "minus" , 2 );
    TEST_NODE( t.root().children(0) , "cos" , 1 );
    TEST_NODE( t.root().children(0).children(0) , "y" , 0 );
    TEST_NODE( t.root().children(1) , "x" , 0 );
}

TEST( basic_tree_tests , copy_construct )
{
    test_tree< basic_tree_tag > trees;
    basic_tree< std::string > t( trees.data2 );
    
    EXPECT_EQ( trees.data2.size() , 4 );
    EXPECT_FALSE( trees.data2.empty() );
    EXPECT_EQ( t.size() , 4 );
    EXPECT_FALSE( t.empty() );
    TEST_NODE( t.root() , "minus" , 2 );
    TEST_NODE( t.root().children(0) , "cos" , 1 );
    TEST_NODE( t.root().children(0).children(0) , "y" , 0 );
    TEST_NODE( t.root().children(1) , "x" , 0 );
}

TEST( basic_tree_tests , move_construct )
{
    test_tree< basic_tree_tag > trees;
    basic_tree< std::string > t( std::move( trees.data2 ) );
    
    EXPECT_EQ( trees.data2.size() , 0 );
    EXPECT_TRUE( trees.data2.empty() );
    
    EXPECT_EQ( t.size() , 4 );
    EXPECT_FALSE( t.empty() );
    TEST_NODE( t.root() , "minus" , 2 );
    TEST_NODE( t.root().children(0) , "cos" , 1 );
    TEST_NODE( t.root().children(0).children(0) , "y" , 0 );
    TEST_NODE( t.root().children(1) , "x" , 0 );
}

TEST( basic_tree_tests , copy_assign )
{
    test_tree< basic_tree_tag > trees;
    trees.data = trees.data2;
    
    EXPECT_EQ( trees.data2.size() , 4 );
    EXPECT_FALSE( trees.data2.empty() );
    
    EXPECT_EQ( trees.data.size() , 4 );
    EXPECT_FALSE( trees.data.empty() );
    TEST_NODE( trees.data.root() , "minus" , 2 );
    TEST_NODE( trees.data.root().children(0) , "cos" , 1 );
    TEST_NODE( trees.data.root().children(0).children(0) , "y" , 0 );
    TEST_NODE( trees.data.root().children(1) , "x" , 0 );
}

TEST( basic_tree_tests , move_assign )
{
    test_tree< basic_tree_tag > trees;
    trees.data = std::move( trees.data2 );
    
    EXPECT_EQ( trees.data2.size() , 0 );
    EXPECT_TRUE( trees.data2.empty() );
    
    EXPECT_EQ( trees.data.size() , 4 );
    EXPECT_FALSE( trees.data.empty() );
    TEST_NODE( trees.data.root() , "minus" , 2 );
    TEST_NODE( trees.data.root().children(0) , "cos" , 1 );
    TEST_NODE( trees.data.root().children(0).children(0) , "y" , 0 );
    TEST_NODE( trees.data.root().children(1) , "x" , 0 );
}


TEST( basic_tree_tests , swap_method )
{
    test_tree< basic_tree_tag > trees;
    trees.data.swap( trees.data2 );
    
    EXPECT_EQ( trees.data.size() , 4 );
    EXPECT_FALSE( trees.data.empty() );
    TEST_NODE( trees.data.root() , "minus" , 2 );
    TEST_NODE( trees.data.root().children(0) , "cos" , 1 );
    TEST_NODE( trees.data.root().children(0).children(0) , "y" , 0 );
    TEST_NODE( trees.data.root().children(1) , "x" , 0 );

    EXPECT_EQ( trees.data2.size() , 6 );
    EXPECT_FALSE( trees.data2.empty() );
    TEST_NODE( trees.data2.root() , "plus" , 2 );
    TEST_NODE( trees.data2.root().children(0) , "sin" , 1 );
    TEST_NODE( trees.data2.root().children(0).children(0) , "x" , 0 );
    TEST_NODE( trees.data2.root().children(1) , "minus" , 2 );
    TEST_NODE( trees.data2.root().children(1).children(0) , "y" , 0 );
    TEST_NODE( trees.data2.root().children(1).children(1) , "2" , 0 );
}

TEST( basic_tree_tests , swap_function )
{
    test_tree< basic_tree_tag > trees;
    swap( trees.data , trees.data2 );
    
    EXPECT_EQ( trees.data.size() , 4 );
    EXPECT_FALSE( trees.data.empty() );
    TEST_NODE( trees.data.root() , "minus" , 2 );
    TEST_NODE( trees.data.root().children(0) , "cos" , 1 );
    TEST_NODE( trees.data.root().children(0).children(0) , "y" , 0 );
    TEST_NODE( trees.data.root().children(1) , "x" , 0 );

    EXPECT_EQ( trees.data2.size() , 6 );
    EXPECT_FALSE( trees.data2.empty() );
    TEST_NODE( trees.data2.root() , "plus" , 2 );
    TEST_NODE( trees.data2.root().children(0) , "sin" , 1 );
    TEST_NODE( trees.data2.root().children(0).children(0) , "x" , 0 );
    TEST_NODE( trees.data2.root().children(1) , "minus" , 2 );
    TEST_NODE( trees.data2.root().children(1).children(0) , "y" , 0 );
    TEST_NODE( trees.data2.root().children(1).children(1) , "2" , 0 );
}

TEST( basic_tree_tests , equal_compare )
{
    test_tree< basic_tree_tag > trees;
    basic_tree< std::string > t = trees.data;
    
    EXPECT_FALSE( trees.data == trees.data2 );
    EXPECT_TRUE( trees.data != trees.data2 );
    EXPECT_NE( ( trees.data ) , ( trees.data2 ) );
    
    EXPECT_TRUE( trees.data == t );
    EXPECT_FALSE( trees.data != t );
    EXPECT_EQ( ( trees.data ) , t );
}
