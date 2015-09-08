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
#include <gpcxx/tree/intrusive_functions.hpp>

#include <gtest/gtest.h>

#include <array>

#define TESTNAME group_tests

using namespace std;
using namespace gpcxx;

using node_type = algebraic_node< double , std::array< double , 2 > const >;
using group_type = group< node_type >;

TEST( TESTNAME , test_construction )
{
    group_type g {
        node_type::make_binary_operation( gpcxx::plus_func {} , "+" ) ,
        node_type::make_constant_terminal( gpcxx::double_terminal<> { 0.0 } , "0" ) ,
        node_type::make_binary_operation( gpcxx::minus_func {} , "-" ) ,
        node_type::make_identity_operation( gpcxx::unary_minus_func {} , "um" ) } ;
        
    EXPECT_EQ( g.operation() , node_type::make_binary_operation( gpcxx::plus_func {} , "+" ) );
    EXPECT_EQ( g.identity() , node_type::make_constant_terminal( gpcxx::double_terminal<>{ 0.0 } , "0" ) );
    EXPECT_EQ( g.inverse_operation() , node_type::make_binary_operation( gpcxx::minus_func {} , "-" ) );
    EXPECT_EQ( g.inverse_function() , node_type::make_identity_operation( gpcxx::unary_minus_func {} , "um" ) );
}
