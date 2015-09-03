/*
 * test/canonic/group.cpp
 * Date: 2015-09-02
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/canonic/group.hpp>
#include <gpcxx/canonic/algebraic_node.hpp>

#include <gtest/gtest.h>

#define TESTNAME group_tests

using namespace std;
using namespace gpcxx;

using node_type = algebraic_node<>;
using group_type = group< node_type >;

TEST( TESTNAME , test_construction )
{
    group_type g {
        node_type::make_commutative_binary_operation( "+" ) ,
        node_type::make_constant_terminal( "0" ) ,
        node_type::make_non_commutative_binary_operation( "-" ) ,
        node_type::make_identity_operation( "um" ) } ;
        
    EXPECT_EQ( g.operation() , node_type::make_commutative_binary_operation( "+" ) );
    EXPECT_EQ( g.identity() , node_type::make_constant_terminal( "0" ) );
    EXPECT_EQ( g.inverse_operation() , node_type::make_non_commutative_binary_operation( "-" ) );
    EXPECT_EQ( g.inverse_function() , node_type::make_identity_operation( "um" ) );
}
