/*
 * test/canonic/order_commutative.cpp
 * Date: 2015-09-08
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "canonic_test_trees.hpp"
#include <gpcxx/canonic/order_commutative.hpp>

#include <gtest/gtest.h>

#include <sstream>
#include <vector>
#include <functional>

#define TESTNAME order_commutative_tests

using node_type = canonic_test_trees::node_type;
using tree_type = canonic_test_trees::tree_type;
using rule = std::function< gpcxx::rule_result( tree_type& , tree_type::cursor ) >;
using rule_container = std::vector< rule >;
using algebras_type = gpcxx::algebras< node_type >;


#include <gpcxx/io.hpp>
#include <iostream>
using namespace std;
using namespace gpcxx;

TEST( TESTNAME , test_tree1 )
{
    auto t = canonic_test_trees::test_tree1();
    
    algebras_type algebras { };
    algebras.add_abelian_magma( node_type::make_binary_operation( gpcxx::plus_func {} , "+" ) );
    
    rule_container rules { gpcxx::order_commutative< algebras_type > { algebras } };
    gpcxx::transform_tree( rules , t );
    
    EXPECT_EQ( t.size() , size_t( 3 ) );
    EXPECT_EQ( t.root()->name() , "+" );
    EXPECT_EQ( t.root().children(0)->name() , "x" );
    EXPECT_EQ( t.root().children(1)->name() , "y" );
}

TEST( TESTNAME , test_tree2 )
{
    auto t = canonic_test_trees::test_tree2();
    
    algebras_type algebras { };
    algebras.add_abelian_magma( node_type::make_binary_operation( gpcxx::plus_func {} , "+" ) );
    
    rule_container rules { gpcxx::order_commutative< algebras_type > { algebras } };
    gpcxx::transform_tree( rules , t );
    
    EXPECT_EQ( t.size() , size_t( 5 ) );
    EXPECT_EQ( t.root()->name() , "+" );
    EXPECT_EQ( t.root().children(0)->name() , "x" );
    EXPECT_EQ( t.root().children(1)->name() , "y" );
    EXPECT_EQ( t.root().children(2)->name() , "z" );
    EXPECT_EQ( t.root().children(3)->name() , "1.5" );
}

