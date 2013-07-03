/*
 test/tree/basic_tree.cpp

 Copyright 2013 Karsten Ahnert

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gp/tree/basic_tree.hpp>
#include <gp/io/simple.hpp>
#include <gp/tree/detail/basic_node_inspect.hpp>

#include <gtest/gtest.h>

#include <sstream>

using namespace std;
using namespace gp;

TEST( tree_tests , basic_tree_default_construct )
{
    basic_tree< int > tree;
    auto root = tree.root();
    EXPECT_EQ( tree.size() , 0 );
    EXPECT_TRUE( tree.empty() );
    
    // detail::inspect_basic_node( clog , root.parent_node() , 0 );
    
}

TEST( tree_tests , basic_tree_insert_one )
{
    basic_tree< std::string > tree;
    auto root = tree.root();
    basic_tree< std::string >::cursor n1 = tree.insert_below( root , std::string( "+" ) );
    
    // detail::inspect_basic_node( clog , root.parent_node() , 0 );
    
    EXPECT_EQ( tree.size() , 1 );
    EXPECT_FALSE( tree.empty() );
    EXPECT_EQ( *n1 , "+" );
    EXPECT_EQ( n1.size() , 0 );

    tree.insert_below( n1 , std::string( "11" ) );
    tree.insert_below( n1 , std::string( "12" ) );
    
    // detail::inspect_basic_node( clog , root.parent_node() , 0 );
    
    ostringstream str;
    str << simple( tree );
    EXPECT_EQ( str.str() , "11 + 12" );
    
    // detail::inspect_basic_node( clog , root.parent_node() , 0 );
    
    
}

TEST( tree_tests , basic_tree_insert_two )
{
    basic_tree< std::string > tree;
    tree.insert_below( tree.root() , std::string( "+" ) );
    tree.insert_below( tree.root() , std::string( "11" ) );
    tree.insert_below( tree.root() , std::string( "12" ) );
    ostringstream str;
    str << simple( tree );
    EXPECT_EQ( str.str() , "11 + 12" );
}


