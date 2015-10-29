/*
 * test/canonic/sort_commutative.cpp
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
#include <gpcxx/canonic/sort_commutative.hpp>

#include <gtest/gtest.h>

#include <sstream>
#include <vector>
#include <functional>

#define TESTNAME sort_commutative_tests

using node_type = canonic_test_trees::node_type;
using tree_type = canonic_test_trees::tree_type;
using rule = std::function< gpcxx::rule_result( tree_type& , tree_type::cursor ) >;
using rule_container = std::vector< rule >;
using algebras_type = gpcxx::algebras< node_type >;



TEST( TESTNAME , test1 )
{
    auto t = canonic_test_trees::test_tree1();
    
    algebras_type algebras { };
    algebras.add_abelian_magma( node_type::make_binary_operation( gpcxx::plus_func {} , "+" ) );
    
    rule_container rules { gpcxx::make_sort_commutative( algebras ) };
    gpcxx::transform_tree( rules , t );
    
    EXPECT_EQ( t.size() , size_t( 3 ) );
    test_cursor( t.root() , "+" , 2 , 2 , 0 );
    test_cursor( t.root().children(0) , "x" , 0 , 1 , 1 );
    test_cursor( t.root().children(1) , "y" , 0 , 1 , 1 );
}

TEST( TESTNAME , test2 )
{
    auto t = canonic_test_trees::test_tree2();
    
    algebras_type algebras { };
    algebras.add_abelian_magma( node_type::make_binary_operation( gpcxx::plus_func {} , "+" ) );
    
    rule_container rules { gpcxx::make_sort_commutative( algebras ) };
    gpcxx::transform_tree( rules , t );
    
    EXPECT_EQ( t.size() , size_t( 5 ) );
    test_cursor( t.root() , "+" , 4 , 2 , 0 );
    test_cursor( t.root().children(0) , "x" , 0 , 1 , 1 );
    test_cursor( t.root().children(1) , "y" , 0 , 1 , 1 );
    test_cursor( t.root().children(2) , "z" , 0 , 1 , 1 );
    test_cursor( t.root().children(3) , "1.5" , 0 , 1 , 1 );
}

