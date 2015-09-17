/*
 * test/canonic/summarize_constants.cpp
 * Date: 2015-09-17
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "canonic_test_trees.hpp"
#include "../common/test_functions.hpp"
#include <gpcxx/canonic/summarize_constants.hpp>

#include <gtest/gtest.h>

#define TESTNAME sumarize_constants_tests


using node_type = canonic_test_trees::node_type;
using tree_type = canonic_test_trees::tree_type;
using rule = std::function< gpcxx::rule_result( tree_type& , tree_type::cursor ) >;
using rule_container = std::vector< rule >;

TEST( TESTNAME , test1 )
{
    auto t = canonic_test_trees::test_tree7();
    
    auto rule = gpcxx::make_summarize_constant( []( double t ) {
        return node_type::make_constant_terminal( gpcxx::double_terminal< double >( t ) , std::to_string( t ) ); } );
    rule_container rules { rule };
    gpcxx::transform_tree( rules , t );

    EXPECT_EQ( t.size() , size_t { 1 } );
    test_cursor( t.root() , "0.841471" , 0 , 1 , 0 );
}

TEST( TESTNAME , test2 )
{
    auto t = canonic_test_trees::test_tree8();
    
    auto rule = gpcxx::make_summarize_constant( []( double t ) {
        return node_type::make_constant_terminal( gpcxx::double_terminal< double >( t ) , std::to_string( t ) ); } );
    rule_container rules { rule };
    gpcxx::transform_tree( rules , t );

    EXPECT_EQ( t.size() , size_t { 3 } );
    test_cursor( t.root() , "+" , 2 , 2 , 0 );
    test_cursor( t.root().children(0) , "x" , 0 , 1 , 1 );
    test_cursor( t.root().children(1) , "0.841471" , 0 , 1 , 1 );
}

TEST( TESTNAME , test3 )
{
    auto t = canonic_test_trees::test_tree9();
    
    auto rule = gpcxx::make_summarize_constant( []( double t ) {
        return node_type::make_constant_terminal( gpcxx::double_terminal< double >( t ) , std::to_string( t ) ); } );
    rule_container rules { rule };
    gpcxx::transform_tree( rules , t );

    EXPECT_EQ( t.size() , size_t { 1 } );
    test_cursor( t.root() , "1.841471" , 0 , 1 , 0 );
}
