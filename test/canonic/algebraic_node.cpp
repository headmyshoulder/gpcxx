/*
 * test/canonic/algebraic_node.cpp
 * Date: 2015-09-02
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/canonic/algebraic_node.hpp>

#include <gtest/gtest.h>

#define TESTNAME algebraic_node_tests

using namespace std;
using namespace gpcxx;

using node_type = algebraic_node<>;

TEST( TESTNAME , test_construction )
{
    auto node = node_type { "x" , false , true , false , 21 };
    EXPECT_EQ( node.name() , "x" );
    EXPECT_FALSE( node.commutative() );
    EXPECT_TRUE( node.associative() );
    EXPECT_FALSE( node.constant() );
    EXPECT_EQ( node.precedence() , 21 );
}

TEST( TESTNAME , test_commutative_binary_operation_construction )
{
    auto node = node_type::make_commutative_binary_operation( "+" );
    EXPECT_EQ( node.name() , "+" );
    EXPECT_TRUE( node.commutative() );
    EXPECT_TRUE( node.associative() );
    EXPECT_FALSE( node.constant() );
    EXPECT_EQ( node.precedence() , 20 );
}

TEST( TESTNAME , test_unary_operation_construction )
{
    auto node = node_type::make_unary_operation( "sin" );
    EXPECT_EQ( node.name() , "sin" );
    EXPECT_FALSE( node.commutative() );
    EXPECT_FALSE( node.associative() );
    EXPECT_FALSE( node.constant() );
    EXPECT_EQ( node.precedence() , 10 );
}

TEST( TESTNAME , test_identity_operation_construction )
{
    auto node = node_type::make_identity_operation( "inv" );
    EXPECT_EQ( node.name() , "inv" );
    EXPECT_FALSE( node.commutative() );
    EXPECT_FALSE( node.associative() );
    EXPECT_FALSE( node.constant() );
    EXPECT_EQ( node.precedence() , 11 );
}

TEST( TESTNAME , test_constant_terminal_construction )
{
    auto node = node_type::make_constant_terminal( "1.5" );
    EXPECT_EQ( node.name() , "1.5" );
    EXPECT_FALSE( node.commutative() );
    EXPECT_FALSE( node.associative() );
    EXPECT_TRUE( node.constant() );
    EXPECT_EQ( node.precedence() , 0 );
}

TEST( TESTNAME , test_variable_terminal_construction )
{
    auto node = node_type::make_variable_terminal( "x" );
    EXPECT_EQ( node.name() , "x" );
    EXPECT_FALSE( node.commutative() );
    EXPECT_FALSE( node.associative() );
    EXPECT_FALSE( node.constant() );
    EXPECT_EQ( node.precedence() , 1 );
}
