/*
 * test/tree/basic_growing_tree.cpp
 * Date: 2015-05-17
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/tree/basic_tree.hpp>
#include <gpcxx/util/macros.hpp>

#include "../common/test_functions.hpp"

#include <gtest/gtest.h>

#include <sstream>
#include <string>

#define TESTNAME basic_growing_tree_tests

using namespace gpcxx;

TEST( TESTNAME , default_construct )
{
    basic_growing_tree< std::string > tree;
    auto root = tree.root();
    GPCXX_UNUSED_VAR( root );
    EXPECT_EQ( tree.size() , size_t( 0 ) );
    EXPECT_TRUE( tree.empty() );
}

TEST( TESTNAME , insert )
{
    basic_growing_tree< std::string > tree;
    auto root = tree.root();
    auto n1 =         tree.insert_below( root , "+" );
    /* auto n2 = */   tree.insert_below( n1 , "x" );
    /* auto n3 = */   tree.insert_below( n1 , "y" );
    /* auto n4 = */   tree.insert_below( n1 , "z" );
    /* auto n5 = */   tree.insert_below( n1 , "1" );
    /* auto n6 = */   tree.insert_below( n1 , "2" );
    
    EXPECT_FALSE( tree.empty() );
    EXPECT_EQ( tree.size() , 6 );
    test_cursor( tree.root() , "+" , 5 , 2 , 0 );
    test_cursor( tree.root().children(0) , "x" , 0 , 1 , 1 );
    test_cursor( tree.root().children(1) , "y" , 0 , 1 , 1 );
    test_cursor( tree.root().children(2) , "z" , 0 , 1 , 1 );
    test_cursor( tree.root().children(3) , "1" , 0 , 1 , 1 );
    test_cursor( tree.root().children(4) , "2" , 0 , 1 , 1 );
}
