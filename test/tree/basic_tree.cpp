/*
 test/tree/basic_tree.cpp

 Copyright 2013 Karsten Ahnert

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/tree/basic_tree.hpp>
#include <gpcxx/tree/detail/inspect_node_base.hpp>

#include "../common/test_tree.hpp"

#include <gtest/gtest.h>

#include <sstream>

using namespace std;
using namespace gpcxx;


// detail::inspect_basic_node( clog , root.parent_node() , 0 );

#define TESTNAME basic_tree_tests

#define TEST_NODE( n , VALUE , ARITY , HEIGHT , LEVEL )                   \
    EXPECT_EQ( *(n) , VALUE );                                            \
    EXPECT_EQ( (n).size() , size_t( ARITY ) );                            \
    EXPECT_EQ( (n).height() , size_t( HEIGHT ) );                         \
    EXPECT_EQ( (n).level() , size_t( LEVEL ) )


TEST( TESTNAME , default_construct )
{
    basic_tree< int > tree;
    auto root = tree.root();
    EXPECT_EQ( root.node() , nullptr );
    EXPECT_EQ( tree.size() , size_t( 0 ) );
    EXPECT_TRUE( tree.empty() );
}

TEST( TESTNAME , insert_below_rvalue )
{
    basic_tree< std::string > tree;
    auto root = tree.root();
    basic_tree< std::string >::cursor n1 = tree.insert_below( root , "+" );
    
    EXPECT_EQ( tree.size() , size_t( 1 ) );
    EXPECT_FALSE( tree.empty() );
    TEST_NODE( n1 , "+" , 0 , 1 , 0 );
    TEST_NODE( tree.root() , "+" , 0 , 1 , 0 );

    tree.insert_below( n1 , "11" );
    tree.insert_below( n1 , "12" );

    EXPECT_FALSE( tree.empty() );
    EXPECT_EQ( tree.size() , size_t( 3 ) );
    TEST_NODE( tree.root() , "+" , 2 , 2 , 0 );
    TEST_NODE( tree.root().children(0) , "11" , 0 , 1 , 1 );
    TEST_NODE( tree.root().children(1) , "12" , 0 , 1 , 1 );
}

TEST( TESTNAME , insert_value )
{
    std::string plus { "+" } , child1 { "11" } , child2 { "12" };
    basic_tree< std::string > tree;
    auto root = tree.root();

    auto n1 = tree.insert( root , plus );
    EXPECT_EQ( tree.size() , size_t( 1 ) );
    EXPECT_FALSE( tree.empty() );
    TEST_NODE( n1 , "+" , 0 , 1 , 0 );
    TEST_NODE( tree.root() , "+" , 0 , 1 , 0 );
    
    auto n2 = tree.insert_below( n1 , child2 );
    /* auto n3 = */ tree.insert( n2 , child1 );
    
    EXPECT_FALSE( tree.empty() );
    EXPECT_EQ( tree.size() , size_t( 3 ) );
    TEST_NODE( tree.root() , "+" , 2 , 2 , 0 );
    TEST_NODE( tree.root().children(0) , "11" , 0 , 1 , 1 );
    TEST_NODE( tree.root().children(1) , "12" , 0 , 1 , 1 );
}


TEST( TESTNAME , insert_rvalue )
{
    std::string plus { "+" } , child1 { "11" } , child2 { "12" };
    basic_tree< std::string > tree;
    auto root = tree.root();

    auto n1 = tree.insert( root , std::move( plus ) );
    EXPECT_EQ( tree.size() , size_t( 1 ) );
    EXPECT_FALSE( tree.empty() );
    TEST_NODE( n1 , "+" , 0 , 1 , 0 );
    TEST_NODE( tree.root() , "+" , 0 , 1 , 0 );
    
    auto n2 = tree.insert_below( n1 , std::move( child2 ) );
    /* auto n3 = */ tree.insert( n2 , std::move( child1 ) );
    
    EXPECT_FALSE( tree.empty() );
    EXPECT_EQ( tree.size() , size_t( 3 ) );
    TEST_NODE( tree.root() , "+" , 2 , 2 , 0 );
    TEST_NODE( tree.root().children(0) , "11" , 0 , 1 , 1 );
    TEST_NODE( tree.root().children(1) , "12" , 0 , 1 , 1 );
}

TEST( TESTNAME , insert_above_value )
{
    std::string plus { "+" } , child1 { "11" } , child2 { "12" };
    basic_tree< std::string > tree;
    auto root = tree.root();

    auto n1 = tree.insert_above( root , child1 );
    EXPECT_EQ( tree.size() , size_t( 1 ) );
    EXPECT_FALSE( tree.empty() );
    TEST_NODE( n1 , "11" , 0 , 1 , 0 );
    TEST_NODE( tree.root() , "11" , 0 , 1 , 0 );
    
    auto n2 = tree.insert_above( n1 , plus );
    /* auto n3 = */ tree.insert_below( n2 , child2 );
    
    EXPECT_FALSE( tree.empty() );
    EXPECT_EQ( tree.size() , size_t( 3 ) );
    TEST_NODE( tree.root() , "+" , 2 , 2 , 0 );
    TEST_NODE( tree.root().children(0) , "11" , 0 , 1 , 1 );
    TEST_NODE( tree.root().children(1) , "12" , 0 , 1 , 1 );
}


TEST( TESTNAME , insert_above_rv )
{
    std::string plus { "+" } , child1 { "11" } , child2 { "12" };
    basic_tree< std::string > tree;
    auto root = tree.root();

    auto n1 = tree.insert_above( root , std::move( child1 ) );
    EXPECT_EQ( tree.size() , size_t( 1 ) );
    EXPECT_FALSE( tree.empty() );
    TEST_NODE( n1 , "11" , 0 , 1 , 0 );
    TEST_NODE( tree.root() , "11" , 0 , 1 , 0 );
    
    auto n2 = tree.insert_above( n1 , std::move( plus ) );
    /* auto n3 = */ tree.insert_below( n2 , std::move( child2 ) );
    
    EXPECT_FALSE( tree.empty() );
    EXPECT_EQ( tree.size() , size_t( 3 ) );
    TEST_NODE( tree.root() , "+" , 2 , 2 , 0 );
    TEST_NODE( tree.root().children(0) , "11" , 0 , 1 , 1 );
    TEST_NODE( tree.root().children(1) , "12" , 0 , 1 , 1 );
}

TEST( TESTNAME , insert_and_erase )
{
    basic_tree< std::string > tree;
    tree.insert_below( tree.root() , "+" );
    auto n1 = tree.insert_below( tree.root() , "-" );
    auto n2 = tree.insert_below( tree.root() , "*" );
    /* auto n3 = */ tree.insert_below( n1 , "13" );
    /* auto n4 = */ tree.insert_below( n1 , "14" );
    /* auto n5 = */ tree.insert_below( n2 , "15" );
    /* auto n6 = */ tree.insert_below( n2 , "16" );
    
    EXPECT_EQ( tree.size() , size_t( 7 ) );
    EXPECT_FALSE( tree.empty() );
    TEST_NODE( tree.root() , "+" , 2 , 3 , 0 );
    TEST_NODE( tree.root().children(0) , "-" , 2 , 2 , 1 );
    TEST_NODE( tree.root().children(0).children(0) , "13" , 0 , 1 , 2 );
    TEST_NODE( tree.root().children(0).children(1) , "14" , 0 , 1 , 2 );
    TEST_NODE( tree.root().children(1) , "*" , 2 , 2 , 1 );
    TEST_NODE( tree.root().children(1).children(0) , "15" , 0 , 1 , 2 );
    TEST_NODE( tree.root().children(1).children(1) , "16" , 0 , 1 , 2 );
    
    tree.erase( n2 );
    
    EXPECT_EQ( tree.size() , size_t( 4 ) );
    EXPECT_FALSE( tree.empty() );
    TEST_NODE( tree.root() , "+" , 1 , 3 , 0 );
    TEST_NODE( tree.root().children(0) , "-" , 2 , 2 , 1 );
    TEST_NODE( tree.root().children(0).children(0) , "13" , 0 , 1 , 2 );
    TEST_NODE( tree.root().children(0).children(1) , "14" , 0 , 1 , 2 );
}

TEST( TESTNAME , cursor_parents )
{
    test_tree< basic_tree_tag > trees;
    EXPECT_EQ( trees.data.root().children(0).children(0).parent() , trees.data.root().children(0) );
}



TEST( TESTNAME , insert_cursor )
{
    test_tree< basic_tree_tag > trees;
    test_tree< basic_tree_tag >::tree_type::cursor c = trees.data.root().children(1);
    trees.data.erase( c.children(1) );
    test_tree< basic_tree_tag >::tree_type::cursor c2 = trees.data2.root();
    trees.data.insert_below( c , c2 );
    
    EXPECT_EQ( trees.data.size() , size_t( 9 ) );
    EXPECT_FALSE( trees.data.empty() );
    EXPECT_EQ( trees.data2.size() , size_t( 4 ) );
    EXPECT_FALSE( trees.data2.empty() );
    
    TEST_NODE( trees.data.root() , "plus" , 2 , 5 , 0 );
    TEST_NODE( trees.data.root().children(0) , "sin" , 1 , 2 , 1 );
    TEST_NODE( trees.data.root().children(0).children(0) , "x" , 0 , 1 , 2 );
    TEST_NODE( trees.data.root().children(1) , "minus" , 2 , 4 , 1 );
    TEST_NODE( trees.data.root().children(1).children(0) , "y" , 0 , 1 , 2 );
    TEST_NODE( trees.data.root().children(1).children(1) , "minus" , 2 , 3 , 2 );
    TEST_NODE( trees.data.root().children(1).children(1).children(0) , "cos" , 1 , 2 , 3 );
    TEST_NODE( trees.data.root().children(1).children(1).children(0).children(0) , "y" , 0 , 1 , 4 );
    TEST_NODE( trees.data.root().children(1).children(1).children(1) , "x" , 0 , 1 , 3 );
}


TEST( TESTNAME , assign )
{
    test_tree< basic_tree_tag > trees;
    trees.data.assign( trees.data2.root() );
    
    EXPECT_EQ( trees.data.size() , size_t( 4 ) );
    EXPECT_FALSE( trees.data.empty() );
    TEST_NODE( trees.data.root() , "minus" , 2 , 3 , 0 );
    TEST_NODE( trees.data.root().children(0) , "cos" , 1 , 2 , 1 );
    TEST_NODE( trees.data.root().children(0).children(0) , "y" , 0 , 1 , 2 );
    TEST_NODE( trees.data.root().children(1) , "x" , 0 , 1 , 1 );
}

TEST( TESTNAME , clear )
{
    test_tree< basic_tree_tag > trees;
    trees.data.clear();
    
    EXPECT_EQ( trees.data.size() , size_t( 0 ) );
    EXPECT_TRUE( trees.data.empty() );
}

TEST( TESTNAME , iterator_construct )
{
    test_tree< basic_tree_tag > trees;
    test_tree< basic_tree_tag >::tree_type t( trees.data2.root() );
    
    EXPECT_EQ( t.size() , size_t( 4 ) );
    EXPECT_FALSE( t.empty() );
    TEST_NODE( t.root() , "minus" , 2 , 3 , 0 );
    TEST_NODE( t.root().children(0) , "cos" , 1 , 2 , 1 );
    TEST_NODE( t.root().children(0).children(0) , "y" , 0 , 1 , 2 );
    TEST_NODE( t.root().children(1) , "x" , 0 , 1 , 1 );
}

TEST( TESTNAME , copy_construct )
{
    test_tree< basic_tree_tag > trees;
    test_tree< basic_tree_tag >::tree_type t( trees.data2 );
    
    EXPECT_EQ( trees.data2.size() , size_t( 4 ) );
    EXPECT_FALSE( trees.data2.empty() );
    EXPECT_EQ( t.size() , size_t( 4 ) );
    EXPECT_FALSE( t.empty() );
    TEST_NODE( t.root() , "minus" , 2 , 3 , 0 );
    TEST_NODE( t.root().children(0) , "cos" , 1 , 2 , 1 );
    TEST_NODE( t.root().children(0).children(0) , "y" , 0 , 1 , 2 );
    TEST_NODE( t.root().children(1) , "x" , 0 , 1 , 1 );
}

TEST( TESTNAME , move_construct )
{
    test_tree< basic_tree_tag > trees;
    test_tree< basic_tree_tag >::tree_type t( std::move( trees.data2 ) );
    
    EXPECT_EQ( trees.data2.size() , size_t( 0 ) );
    EXPECT_TRUE( trees.data2.empty() );
    
    EXPECT_EQ( t.size() , size_t( 4 ) );
    EXPECT_FALSE( t.empty() );
    TEST_NODE( t.root() , "minus" , 2 , 3 , 0 );
    TEST_NODE( t.root().children(0) , "cos" , 1 , 2 , 1 );
    TEST_NODE( t.root().children(0).children(0) , "y" , 0 , 1 , 2 );
    TEST_NODE( t.root().children(1) , "x" , 0 , 1 , 1 );
}

TEST( TESTNAME , copy_assign )
{
    test_tree< basic_tree_tag > trees;
    trees.data = trees.data2;
    
    EXPECT_EQ( trees.data2.size() , size_t( 4 ) );
    EXPECT_FALSE( trees.data2.empty() );
    
    EXPECT_EQ( trees.data.size() , size_t( 4 ) );
    EXPECT_FALSE( trees.data.empty() );
    TEST_NODE( trees.data.root() , "minus" , 2 , 3 , 0 );
    TEST_NODE( trees.data.root().children(0) , "cos" , 1 , 2 , 1 );
    TEST_NODE( trees.data.root().children(0).children(0) , "y" , 0 , 1 , 2 );
    TEST_NODE( trees.data.root().children(1) , "x" , 0 , 1 , 1 );
}

TEST( TESTNAME , move_assign )
{
    test_tree< basic_tree_tag > trees;
    trees.data = std::move( trees.data2 );
    
    EXPECT_EQ( trees.data2.size() , size_t( 0 ) );
    EXPECT_TRUE( trees.data2.empty() );
    
    EXPECT_EQ( trees.data.size() , size_t( 4 ) );
    EXPECT_FALSE( trees.data.empty() );
    TEST_NODE( trees.data.root() , "minus" , 2 , 3 , 0 );
    TEST_NODE( trees.data.root().children(0) , "cos" , 1 , 2 , 1 );
    TEST_NODE( trees.data.root().children(0).children(0) , "y" , 0 , 1 , 2 );
    TEST_NODE( trees.data.root().children(1) , "x" , 0 , 1 , 1 );
}


TEST( TESTNAME , swap_method )
{
    test_tree< basic_tree_tag > trees;
    trees.data.swap( trees.data2 );
    
    EXPECT_EQ( trees.data.size() , size_t( 4 ) );
    EXPECT_FALSE( trees.data.empty() );
    TEST_NODE( trees.data.root() , "minus" , 2 , 3 , 0 );
    TEST_NODE( trees.data.root().children(0) , "cos" , 1 , 2 , 1 );
    TEST_NODE( trees.data.root().children(0).children(0) , "y" , 0 , 1 , 2 );
    TEST_NODE( trees.data.root().children(1) , "x" , 0 , 1 , 1 );

    EXPECT_EQ( trees.data2.size() , size_t( 6 ) );
    EXPECT_FALSE( trees.data2.empty() );
    TEST_NODE( trees.data2.root() , "plus" , 2 , 3 , 0 );
    TEST_NODE( trees.data2.root().children(0) , "sin" , 1 , 2 , 1 );
    TEST_NODE( trees.data2.root().children(0).children(0) , "x" , 0 , 1 , 2 );
    TEST_NODE( trees.data2.root().children(1) , "minus" , 2 , 2 , 1 );
    TEST_NODE( trees.data2.root().children(1).children(0) , "y" , 0 , 1 , 2 );
    TEST_NODE( trees.data2.root().children(1).children(1) , "2" , 0 , 1 , 2 );
}

TEST( TESTNAME , swap_function )
{
    test_tree< basic_tree_tag > trees;
    swap( trees.data , trees.data2 );
    
    EXPECT_EQ( trees.data.size() , size_t( 4 ) );
    EXPECT_FALSE( trees.data.empty() );
    TEST_NODE( trees.data.root() , "minus" , 2 , 3 , 0 );
    TEST_NODE( trees.data.root().children(0) , "cos" , 1 , 2 , 1 );
    TEST_NODE( trees.data.root().children(0).children(0) , "y" , 0 , 1 , 2 );
    TEST_NODE( trees.data.root().children(1) , "x" , 0 , 1 , 1 );

    EXPECT_EQ( trees.data2.size() , size_t( 6 ) );
    EXPECT_FALSE( trees.data2.empty() );
    TEST_NODE( trees.data2.root() , "plus" , 2 , 3 , 0 );
    TEST_NODE( trees.data2.root().children(0) , "sin" , 1 , 2 , 1 );
    TEST_NODE( trees.data2.root().children(0).children(0) , "x" , 0 , 1 , 2 );
    TEST_NODE( trees.data2.root().children(1) , "minus" , 2 , 2 , 1 );
    TEST_NODE( trees.data2.root().children(1).children(0) , "y" , 0 , 1 , 2 );
    TEST_NODE( trees.data2.root().children(1).children(1) , "2" , 0 , 1 , 2 );
}

TEST( TESTNAME , equal_compare )
{
    test_tree< basic_tree_tag > trees;
    test_tree< basic_tree_tag >::tree_type t = trees.data;
    
    EXPECT_FALSE( trees.data == trees.data2 );
    EXPECT_TRUE( trees.data != trees.data2 );
    EXPECT_NE( ( trees.data ) , ( trees.data2 ) );
    
    EXPECT_TRUE( trees.data == t );
    EXPECT_FALSE( trees.data != t );
    EXPECT_EQ( ( trees.data ) , t );
}

TEST( TESTNAME , rank_is )
{
    test_tree< basic_tree_tag > trees;
    EXPECT_EQ( trees.data.rank_is( 0 ) , trees.data.root() );
    EXPECT_EQ( trees.data.rank_is( 1 ) , trees.data.root().children(0) );
    EXPECT_EQ( trees.data.rank_is( 2 ) , trees.data.root().children(1) );
    EXPECT_EQ( trees.data.rank_is( 3 ) , trees.data.root().children(0).children(0) );
    EXPECT_EQ( trees.data.rank_is( 4 ) , trees.data.root().children(1).children(0) );
    EXPECT_EQ( trees.data.rank_is( 5 ) , trees.data.root().children(1).children(1) );
    EXPECT_EQ( trees.data.rank_is( 6 ) , trees.data.shoot() );
    EXPECT_EQ( trees.data.rank_is( 7 ) , trees.data.shoot() );
}

TEST( TESTNAME , swap_subtrees1 )
{
    basic_tree< std::string > t1 , t2;
    swap_subtrees( t1 , t1.root() , t2 , t2.root() );
    EXPECT_TRUE( t1.empty() );
    EXPECT_TRUE( t2.empty() );
    EXPECT_EQ( t1.size() , size_t( 0 ) );
    EXPECT_EQ( t2.size() , size_t( 0 ) );
}

TEST( TESTNAME , swap_subtrees2 )
{
    basic_tree< std::string > t1 , t2;
    t1.insert_below( t1.root() , "+" );
    swap_subtrees( t1 , t1.root() , t2 , t2.root() );
    EXPECT_TRUE( t1.empty() );
    EXPECT_FALSE( t2.empty() );
    EXPECT_EQ( t1.size() , size_t( 0 ) );
    EXPECT_EQ( t2.size() , size_t( 1 ) );
}

TEST( TESTNAME , swap_subtrees3 )
{
    basic_tree< std::string > t1 , t2;
    auto c1 = t1.insert_below( t1.root() , "+" );
    t1.insert_below( c1 , "-" );
    swap_subtrees( t1 , t1.root() , t2 , t2.root() );
    EXPECT_TRUE( t1.empty() );
    EXPECT_FALSE( t2.empty() );
    EXPECT_EQ( t1.size() , size_t( 0 ) );
    EXPECT_EQ( t2.size() , size_t( 2 ) );
    TEST_NODE( t2.root() , "+" , 1 , 2 , 0 );
    TEST_NODE( t2.root().children(0) , "-" , 0 , 1 , 1 );
}

TEST( TESTNAME , swap_subtrees4 )
{
    basic_tree< std::string > t1 , t2;
    auto c1 = t1.insert_below( t1.root() , "+" );
    t1.insert_below( c1 , "-" );
    swap_subtrees( t1 , t1.root().children(0) , t2 , t2.root() );
    EXPECT_FALSE( t1.empty() );
    EXPECT_FALSE( t2.empty() );
    EXPECT_EQ( t1.size() , size_t( 1 ) );
    EXPECT_EQ( t2.size() , size_t( 1 ) );
    TEST_NODE( t1.root() , "+" , 0 , 1 , 0 );
    TEST_NODE( t2.root() , "-" , 0 , 1 , 0 );
}

TEST( TESTNAME , swap_subtrees5 )
{
    test_tree< basic_tree_tag > trees;
    swap_subtrees( trees.data , trees.data.root().children(1)  , trees.data2 , trees.data2.root().children(0) );
    
    EXPECT_FALSE( trees.data.empty() );
    EXPECT_FALSE( trees.data2.empty() );
    EXPECT_EQ( trees.data.size() , size_t( 5 ) );
    EXPECT_EQ( trees.data2.size() , size_t( 5 ) );
    TEST_NODE( trees.data.root() , "plus" , 2 , 3 , 0 );
    TEST_NODE( trees.data.root().children(0) , "sin" , 1 , 2 , 1 );
    TEST_NODE( trees.data.root().children(0).children(0) , "x" , 0 , 1 , 2 );
    TEST_NODE( trees.data.root().children(1) , "cos" , 1 , 2 , 1 );
    TEST_NODE( trees.data.root().children(1).children(0) , "y" , 0 , 1 , 2 );
    
    TEST_NODE( trees.data2.root() , "minus" , 2 , 3 , 0 );
    TEST_NODE( trees.data2.root().children(0) , "minus" , 2 , 2 , 1 );
    TEST_NODE( trees.data2.root().children(0).children(0) , "y" , 0 , 1 , 2 );
    TEST_NODE( trees.data2.root().children(0).children(1) , "2" , 0 , 1 , 2 );
    TEST_NODE( trees.data2.root().children(1) , "x" , 0 , 1 , 1 );
}