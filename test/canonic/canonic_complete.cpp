/*
 * test/canonic/canonic_complete.cpp
 * Date: 2015-09-08
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "canonic_test_trees.hpp"
#include "../common/test_functions.hpp"

#include <gpcxx/canonic/order_commutative.hpp>
#include <gpcxx/canonic/left_lift.hpp>
#include <gpcxx/canonic/remove_double_inverse_functions.hpp>
#include <gpcxx/canonic/remove_inverse_operations.hpp>
#include <gpcxx/canonic/summarize_constants.hpp>

#include <gpcxx/generate/uniform_symbol.hpp>
#include <gpcxx/generate/uniform_symbol_erc.hpp>

#include <gpcxx/io/simple.hpp>

#include <gtest/gtest.h>

#include <vector>
#include <functional>

#define TESTNAME canonic_complete_tests


using node_type = canonic_test_trees::node_type;
using tree_type = canonic_test_trees::tree_type;
using rule = std::function< gpcxx::rule_result( tree_type& , tree_type::cursor ) >;
using rule_container = std::vector< rule >;
using algebras_type = gpcxx::algebras< node_type >;
using group_type = algebras_type::group_type;
using rng_type = std::mt19937;

class TESTNAME : public ::testing::Test
{
protected:
    
    algebras_type m_algebras;
    rule_container m_rules;
    
    void SetUp( void ) override
    {
        m_algebras = algebras_type {};
        
        m_algebras.add_abelian_group( group_type {
            node_type::make_binary_operation( gpcxx::plus_func {} , "+" ) ,
            node_type::make_constant_terminal( gpcxx::double_terminal<> { 0.0 } , "0" ) ,
            node_type::make_binary_operation( gpcxx::minus_func {} , "-" ) ,
            node_type::make_identity_operation( gpcxx::unary_minus_func {} , "um" )
            } );
        m_algebras.add_abelian_group( group_type {
            node_type::make_binary_operation( gpcxx::multiplies_func {} , "*" ) ,
            node_type::make_constant_terminal( gpcxx::double_terminal<> { 1.0 } , "1" ) ,
            node_type::make_binary_operation( gpcxx::divides_func {} , "/" ) ,
            node_type::make_identity_operation( gpcxx::unary_inverse_func {} , "inv" ) 
            } );

        // create simplification rules
        auto constant_factory = []( double t ) {
            std::ostringstream str;
            str << t;
            return node_type::make_constant_terminal( gpcxx::double_terminal< double >( t ) , str.str() ); };

        m_rules = rule_container {
              gpcxx::make_order_commutative( m_algebras )
            , gpcxx::make_left_lift( m_algebras )
            , gpcxx::make_remove_double_inverse_functions( m_algebras )
            , gpcxx::make_remove_inverse_operations( m_algebras )
            , gpcxx::make_summarize_constant( constant_factory )
            };
    }
};

TEST_F( TESTNAME , test1 )
{
    auto t = canonic_test_trees::test_tree1();
    gpcxx::transform_tree( m_rules , t );
    EXPECT_EQ( "x + y" , gpcxx::simple_string( t ) );
}

TEST_F( TESTNAME , test2 )
{
    auto t = canonic_test_trees::test_tree10();
    gpcxx::transform_tree( m_rules , t );
    EXPECT_EQ( "6" , gpcxx::simple_string( t ) );
}

TEST_F( TESTNAME , test3 )
{
    auto t = canonic_test_trees::test_tree11();
    gpcxx::transform_tree( m_rules , t );
    // "+3|y|x|*2|2|*2|4|8"
    EXPECT_EQ( "+( x , y , 64 )" , gpcxx::simple_string( t ) );
}

TEST_F( TESTNAME , test4 )
{
    auto t = canonic_test_trees::test_tree12();
    gpcxx::transform_tree( m_rules , t );
    // "+3|y|x|*2|2|*2|4|8"
    EXPECT_EQ( "+( x * 16 , x , y )" , gpcxx::simple_string( t ) );
}

TEST_F( TESTNAME , test5 )
{
    auto t = canonic_test_trees::test_tree13();
    gpcxx::transform_tree( m_rules , t );
    EXPECT_EQ( "+( x * 32 , y , 2 )" , gpcxx::simple_string( t ) );
}
