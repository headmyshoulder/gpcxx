/*
 test/tree/basic_tree.cpp

 Copyright 2013 Karsten Ahnert

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gp/tree/basic_tree.hpp>
#include <gp/tree/detail/basic_node_inspect.hpp>

#include "../common/test_tree.hpp"

#include <gtest/gtest.h>

#include <sstream>

using namespace std;
using namespace gp;


// detail::inspect_basic_node( clog , root.parent_node() , 0 );

#define TEST_NODE( n , VALUE , ARITY ) \
    EXPECT_EQ( *(n) , VALUE );                                   \
    EXPECT_EQ( (n).size() , ARITY );



TEST( basic_tree_tests , default_construct )
{
    basic_tree< int > tree;
    auto root = tree.root();
    EXPECT_EQ( tree.size() , 0 );
    EXPECT_TRUE( tree.empty() );
    
    
}

TEST( basic_tree_tests , insert_value )
{
    basic_tree< std::string > tree;
    auto root = tree.root();
    basic_tree< std::string >::cursor n1 = tree.insert_below( root , "+" );
    
    EXPECT_EQ( tree.size() , 1 );
    EXPECT_FALSE( tree.empty() );
    TEST_NODE( n1 , "+" , 0 );
    TEST_NODE( tree.root() , "+" , 0 );

    tree.insert_below( n1 , "11" );
    tree.insert_below( n1 , "12" );

    EXPECT_FALSE( tree.empty() );
    EXPECT_EQ( tree.size() , 3 );
    TEST_NODE( tree.root() , "+" , 2 );
    TEST_NODE( tree.root().children(0) , "11" , 0 );
    TEST_NODE( tree.root().children(1) , "12" , 0 );
}

TEST( basic_tree_tests , insert_and_erase )
{
    basic_tree< std::string > tree;
    tree.insert_below( tree.root() , "+" );
    auto n1 = tree.insert_below( tree.root() , "-" );
    auto n2 = tree.insert_below( tree.root() , "*" );
    auto n3 = tree.insert_below( n1 , "13" );
    auto n4 = tree.insert_below( n1 , "14" );
    auto n5 = tree.insert_below( n2 , "15" );
    auto n6 = tree.insert_below( n2 , "16" );    
    
    EXPECT_EQ( tree.size() , 7 );
    EXPECT_FALSE( tree.empty() );
    TEST_NODE( tree.root() , "+" , 2 );
    TEST_NODE( tree.root().children(0) , "-" , 2 );
    TEST_NODE( tree.root().children(0).children(0) , "13" , 0 );
    TEST_NODE( tree.root().children(0).children(1) , "14" , 0 );
    TEST_NODE( tree.root().children(1) , "*" , 2 );
    TEST_NODE( tree.root().children(1).children(0) , "15" , 0 );
    TEST_NODE( tree.root().children(1).children(1) , "16" , 0 );
    
    tree.erase( n2 );
    
    EXPECT_EQ( tree.size() , 4 );
    EXPECT_FALSE( tree.empty() );
    TEST_NODE( tree.root() , "+" , 1 );
    TEST_NODE( tree.root().children(0) , "-" , 2 );
    TEST_NODE( tree.root().children(0).children(0) , "13" , 0 );
    TEST_NODE( tree.root().children(0).children(1) , "14" , 0 );
}


TEST( basic_tree_tests , insert_cursor )
{
    test_tree< basic_tree_tag > trees;
    test_tree< basic_tree_tag >::tree_type::cursor c = trees.data.root().children(1);
    trees.data.erase( c.children(1) );
    test_tree< basic_tree_tag >::tree_type::cursor c2 = trees.data2.root();
    trees.data.insert_below( c , c2 );
    
    EXPECT_EQ( trees.data.size() , 9 );
    EXPECT_FALSE( trees.data.empty() );
    EXPECT_EQ( trees.data2.size() , 4 );
    EXPECT_FALSE( trees.data2.empty() );
    
    TEST_NODE( trees.data.root() , "plus" , 2 );
    TEST_NODE( trees.data.root().children(0) , "sin" , 1 );
    TEST_NODE( trees.data.root().children(0).children(0) , "x" , 0 );
    TEST_NODE( trees.data.root().children(1) , "minus" , 2 );
    TEST_NODE( trees.data.root().children(1).children(0) , "y" , 0 );
    TEST_NODE( trees.data.root().children(1).children(1) , "minus" , 2 );
    TEST_NODE( trees.data.root().children(1).children(1).children(0) , "cos" , 1 );
    TEST_NODE( trees.data.root().children(1).children(1).children(0).children(0) , "y" , 0 );
    TEST_NODE( trees.data.root().children(1).children(1).children(1) , "x" , 0 );
}







// TEST( tree_tests , linked_node_copy_empty )
// {
//     node_type l1( 'a' );
//     node_type l2( l1 );
// 
//     TEST_NODE( l1 , 'a' , 0 , 1 , 1 , 0 , nullptr );
//     TEST_NODE( l2 , 'a' , 0 , 1 , 1 , 0 , nullptr );
//     
// }
// 
// TEST( tree_tests , linked_node_copy )
// {
//     node_type l1( 'a' );
//     auto i1 = l1.emplace( 'b' );
//     auto i2 = l1.emplace( 'c' );
//     node_type l2( l1 );
// 
//     TEST_NODE( l1 , 'a' , 2 , 3 , 2 , 0 , nullptr );
//     TEST_NODE( *i1 , 'b' , 0 , 1 , 1 , 1 , &l1 );
//     TEST_NODE( *i2 , 'c' , 0 , 1 , 1 , 1 , &l1 );
// 
//     TEST_NODE( l2 , 'a' , 2 , 3 , 2 , 0 , nullptr );
//     TEST_NODE( l2.children( 0 ) , 'b' , 0 , 1 , 1 , 1 , &l2 );
//     TEST_NODE( l2.children( 1 ) , 'c' , 0 , 1 , 1 , 1 , &l2 );
// }
// 
// TEST( tree_tests , linked_node_copy_2 )
// {
//     node_type l1( 'a' );
//     auto i1 = l1.emplace( 'b' );
//     auto i2 = l1.emplace( 'c' );
//     auto i3 = i1->emplace( 'd' );
//     auto i4 = i1->emplace( 'e' );
//     auto i5 = i2->emplace( 'f' );
// 
//     node_type l2( l1 );
// 
//     TEST_NODE( l1 , 'a' , 2 , 6 , 3 , 0 , nullptr );
//     TEST_NODE( *i1 , 'b' , 2 , 3 , 2 , 1 , &l1 );
//     TEST_NODE( *i2 , 'c' , 1 , 2 , 2 , 1 , &l1 );
//     TEST_NODE( *i3 , 'd' , 0 , 1 , 1 , 2 , &l1.children(0) );
//     TEST_NODE( *i4 , 'e' , 0 , 1 , 1 , 2 , &l1.children(0) );
//     TEST_NODE( *i5 , 'f' , 0 , 1 , 1 , 2 , &l1.children(1) );
// 
//     TEST_NODE( l2 , 'a' , 2 , 6 , 3 , 0 , nullptr );
//     TEST_NODE( l2.children(0) , 'b' , 2 , 3 , 2 , 1 , &l2 );
//     TEST_NODE( l2.children(1) , 'c' , 1 , 2 , 2 , 1 , &l2 );
//     TEST_NODE( l2.children(0).children(0) , 'd' , 0 , 1 , 1 , 2 , &l2.children(0) );
//     TEST_NODE( l2.children(0).children(1) , 'e' , 0 , 1 , 1 , 2 , &l2.children(0) );
//     TEST_NODE( l2.children(1).children(0) , 'f' , 0 , 1 , 1 , 2 , &l2.children(1) );
// }
// 
// TEST( tree_tests , linked_node_copy_3 )
// {
//     node_type l1( 'a' );
//     auto i1 = l1.emplace( 'b' );
//     auto i2 = l1.emplace( 'c' );
//     auto i3 = i1->emplace( 'd' );
//     auto i4 = i1->emplace( 'e' );
//     auto i5 = i2->emplace( 'f' );
// 
//     node_type l2( l1.children(0) );
// 
//     TEST_NODE( l1 , 'a' , 2 , 6 , 3 , 0 , nullptr );
//     TEST_NODE( *i1 , 'b' , 2 , 3 , 2 , 1 , &l1 );
//     TEST_NODE( *i2 , 'c' , 1 , 2 , 2 , 1 , &l1 );
//     TEST_NODE( *i3 , 'd' , 0 , 1 , 1 , 2 , &l1.children(0) );
//     TEST_NODE( *i4 , 'e' , 0 , 1 , 1 , 2 , &l1.children(0) );
//     TEST_NODE( *i5 , 'f' , 0 , 1 , 1 , 2 , &l1.children(1) );
// 
//     TEST_NODE( l2 , 'b' , 2 , 3 , 2 , 0 , nullptr );
//     TEST_NODE( l2.children(0) , 'd' , 0 , 1 , 1 , 1 , &l2 );
//     TEST_NODE( l2.children(1) , 'e' , 0 , 1 , 1 , 1 , &l2 );
// }
// 
// TEST( tree_tests , linked_node_assign_empty )
// {
//     node_type l1( 'a' );
//     node_type l2;
//     l2 = l1;
// 
//     TEST_NODE( l1 , 'a' , 0 , 1 , 1 , 0 , nullptr );
//     TEST_NODE( l2 , 'a' , 0 , 1 , 1 , 0 , nullptr );
// }
// 
// TEST( tree_tests , linked_node_assign )
// {
//     node_type l1( 'a' );
//     auto i1 = l1.emplace( 'b' );
//     auto i2 = l1.emplace( 'c' );
//     node_type l2;
//     l2 = l1;
// 
//     TEST_NODE( l1 , 'a' , 2 , 3 , 2 , 0 , nullptr );
//     TEST_NODE( *i1 , 'b' , 0 , 1 , 1 , 1 , &l1 );
//     TEST_NODE( *i2 , 'c' , 0 , 1 , 1 , 1 , &l1 );
// 
//     TEST_NODE( l2 , 'a' , 2 , 3 , 2 , 0 , nullptr );
//     TEST_NODE( l2.children( 0 ) , 'b' , 0 , 1 , 1 , 1 , &l2 );
//     TEST_NODE( l2.children( 1 ) , 'c' , 0 , 1 , 1 , 1 , &l2 );
// }
// 
// TEST( tree_tests , linked_node_assign_2 )
// {
//     node_type l1( 'a' );
//     auto i1 = l1.emplace( 'b' );
//     auto i2 = l1.emplace( 'c' );
//     auto i3 = i1->emplace( 'd' );
//     auto i4 = i1->emplace( 'e' );
//     auto i5 = i2->emplace( 'f' );
// 
//     node_type l2;
// 
//     l2 = ( l1 );
// 
//     TEST_NODE( l1 , 'a' , 2 , 6 , 3 , 0 , nullptr );
//     TEST_NODE( *i1 , 'b' , 2 , 3 , 2 , 1 , &l1 );
//     TEST_NODE( *i2 , 'c' , 1 , 2 , 2 , 1 , &l1 );
//     TEST_NODE( *i3 , 'd' , 0 , 1 , 1 , 2 , &l1.children(0) );
//     TEST_NODE( *i4 , 'e' , 0 , 1 , 1 , 2 , &l1.children(0) );
//     TEST_NODE( *i5 , 'f' , 0 , 1 , 1 , 2 , &l1.children(1) );
// 
//     TEST_NODE( l2 , 'a' , 2 , 6 , 3 , 0 , nullptr );
//     TEST_NODE( l2.children(0) , 'b' , 2 , 3 , 2 , 1 , &l2 );
//     TEST_NODE( l2.children(1) , 'c' , 1 , 2 , 2 , 1 , &l2 );
//     TEST_NODE( l2.children(0).children(0) , 'd' , 0 , 1 , 1 , 2 , &l2.children(0) );
//     TEST_NODE( l2.children(0).children(1) , 'e' , 0 , 1 , 1 , 2 , &l2.children(0) );
//     TEST_NODE( l2.children(1).children(0) , 'f' , 0 , 1 , 1 , 2 , &l2.children(1) );
// }
// 
// TEST( tree_tests , linked_node_assign_3 )
// {
//     node_type l1( 'a' );
//     auto i1 = l1.emplace( 'b' );
//     auto i2 = l1.emplace( 'c' );
//     auto i3 = i1->emplace( 'd' );
//     auto i4 = i1->emplace( 'e' );
//     auto i5 = i2->emplace( 'f' );
// 
//     node_type l2;
//     l2.emplace( 'b' );
//     l2.emplace( 'c' );
//     l2 = l1.children(0);
// 
//     TEST_NODE( l1 , 'a' , 2 , 6 , 3 , 0 , nullptr );
//     TEST_NODE( *i1 , 'b' , 2 , 3 , 2 , 1 , &l1 );
//     TEST_NODE( *i2 , 'c' , 1 , 2 , 2 , 1 , &l1 );
//     TEST_NODE( *i3 , 'd' , 0 , 1 , 1 , 2 , &l1.children(0) );
//     TEST_NODE( *i4 , 'e' , 0 , 1 , 1 , 2 , &l1.children(0) );
//     TEST_NODE( *i5 , 'f' , 0 , 1 , 1 , 2 , &l1.children(1) );
// 
//     TEST_NODE( l2 , 'b' , 2 , 3 , 2 , 0 , nullptr );
//     TEST_NODE( l2.children(0) , 'd' , 0 , 1 , 1 , 1 , &l2 );
//     TEST_NODE( l2.children(1) , 'e' , 0 , 1 , 1 , 1 , &l2 );
// }
// 
// 
// TEST( tree_tests , linked_node_insert )
// {
//     node_type l1( 'a' );
//     auto i1 = l1.emplace( 'b' );
//     auto i2 = l1.emplace( 'c' );
//     auto i3 = i1->emplace( 'd' );
//     auto i4 = i1->emplace( 'e' );
//     auto i5 = i2->emplace( 'f' );
// 
//     node_type l2;
//     l2.emplace( 'b' );
//     l2.emplace( 'c' );
//     l2 = l1.children(0);
//     l2.insert( l1.children( 1 ) );
// 
//     TEST_NODE( l1 , 'a' , 2 , 6 , 3 , 0 , nullptr );
//     TEST_NODE( *i1 , 'b' , 2 , 3 , 2 , 1 , &l1 );
//     TEST_NODE( *i2 , 'c' , 1 , 2 , 2 , 1 , &l1 );
//     TEST_NODE( *i3 , 'd' , 0 , 1 , 1 , 2 , &l1.children(0) );
//     TEST_NODE( *i4 , 'e' , 0 , 1 , 1 , 2 , &l1.children(0) );
//     TEST_NODE( *i5 , 'f' , 0 , 1 , 1 , 2 , &l1.children(1) );
// 
//     TEST_NODE( l2 , 'b' , 3 , 5 , 3 , 0 , nullptr );
//     TEST_NODE( l2.children(0) , 'd' , 0 , 1 , 1 , 1 , &l2 );
//     TEST_NODE( l2.children(1) , 'e' , 0 , 1 , 1 , 1 , &l2 );
//     TEST_NODE( l2.children(2) , 'c' , 1 , 2 , 2 , 1 , &l2 );
//     TEST_NODE( l2.children(2).children(0) , 'f' , 0 , 1 , 1 , 2 , &l2.children(2) );
// }
// 
// 
// TEST( tree_tests , linked_emplace_inconsistent )
// {
//     node_type l1( 'a' );
//     auto i1 = l1.emplace_inconsistent( 'b' );
//     auto i2 = l1.emplace_inconsistent( 'c' );
//     auto i3 = i1->emplace_inconsistent( 'd' );
//     auto i4 = i1->emplace_inconsistent( 'e' );
//     auto i5 = i2->emplace_inconsistent( 'f' );
// 
//     TEST_NODE( l1 , 'a' , 2 , 1 , 1 , 0 , nullptr );
//     TEST_NODE( *i1 , 'b' , 2 , 1 , 1 , 1 , &l1 );
//     TEST_NODE( *i2 , 'c' , 1 , 1 , 1 , 1 , &l1 );
//     TEST_NODE( *i3 , 'd' , 0 , 1 , 1 , 2 , &l1.children(0) );
//     TEST_NODE( *i4 , 'e' , 0 , 1 , 1 , 2 , &l1.children(0) );
//     TEST_NODE( *i5 , 'f' , 0 , 1 , 1 , 2 , &l1.children(1) );
// 
//     l1.make_consistent();
// 
//     TEST_NODE( l1 , 'a' , 2 , 6 , 3 , 0 , nullptr );
//     TEST_NODE( *i1 , 'b' , 2 , 3 , 2 , 1 , &l1 );
//     TEST_NODE( *i2 , 'c' , 1 , 2 , 2 , 1 , &l1 );
//     TEST_NODE( *i3 , 'd' , 0 , 1 , 1 , 2 , &l1.children(0) );
//     TEST_NODE( *i4 , 'e' , 0 , 1 , 1 , 2 , &l1.children(0) );
//     TEST_NODE( *i5 , 'f' , 0 , 1 , 1 , 2 , &l1.children(1) );
// }
// 
// 
// 
// TEST( tree_tests , swap_real_1 )
// {
//     test_tree tree;
//     tree.data.swap( tree.data2 );
// 
//     TEST_NODE( tree.data                         , "minus" , 2 , 4 , 3 , 0 , nullptr );
//     TEST_NODE( tree.data.children(0)             , "cos"   , 1 , 2 , 2 , 1 , &tree.data );
//     TEST_NODE( tree.data.children(0).children(0) , "y"     , 0 , 1 , 1 , 2 , tree.data.children_ptr(0) );
//     TEST_NODE( tree.data.children(1)             , "x"     , 0 , 1 , 1 , 1 , &tree.data );
// 
//     TEST_NODE( tree.data2                         , "plus"  , 2 , 6 , 3 , 0 , nullptr );
//     TEST_NODE( tree.data2.children(0)             , "sin"   , 1 , 2 , 2 , 1 , &tree.data2 );
//     TEST_NODE( tree.data2.children(0).children(0) , "x"     , 0 , 1 , 1 , 2 , tree.data2.children_ptr(0) );
//     TEST_NODE( tree.data2.children(1)             , "minus" , 2 , 3 , 2 , 1 , &tree.data2 );
//     TEST_NODE( tree.data2.children(1).children(0) , "y"     , 0 , 1 , 1 , 2 , tree.data2.children_ptr(1) );
//     TEST_NODE( tree.data2.children(1).children(1) , "2"     , 0 , 1 , 1 , 2 , tree.data2.children_ptr(1) );
// }
// 
// TEST( tree_tests , swap_real_2 )
// {
//     test_tree tree;
//     tree.data.swap( tree.data );
// 
//     TEST_NODE( tree.data2                         , "minus" , 2 , 4 , 3 , 0 , nullptr );
//     TEST_NODE( tree.data2.children(0)             , "cos"   , 1 , 2 , 2 , 1 , &tree.data2 );
//     TEST_NODE( tree.data2.children(0).children(0) , "y"     , 0 , 1 , 1 , 2 , tree.data2.children_ptr(0) );
//     TEST_NODE( tree.data2.children(1)             , "x"     , 0 , 1 , 1 , 1 , &tree.data2 );
// 
//     TEST_NODE( tree.data                         , "plus"  , 2 , 6 , 3 , 0 , nullptr );
//     TEST_NODE( tree.data.children(0)             , "sin"   , 1 , 2 , 2 , 1 , &tree.data );
//     TEST_NODE( tree.data.children(0).children(0) , "x"     , 0 , 1 , 1 , 2 , tree.data.children_ptr(0) );
//     TEST_NODE( tree.data.children(1)             , "minus" , 2 , 3 , 2 , 1 , &tree.data );
//     TEST_NODE( tree.data.children(1).children(0) , "y"     , 0 , 1 , 1 , 2 , tree.data.children_ptr(1) );
//     TEST_NODE( tree.data.children(1).children(1) , "2"     , 0 , 1 , 1 , 2 , tree.data.children_ptr(1) );
// }
// 
// 
// 
// TEST( tree_tests , swap_one_1 )
// {
//     test_tree tree;
//     tree.data.swap( tree.data2.at(1) );
// 
//     TEST_NODE( tree.data                           , "cos" , 1 , 2 , 2 , 0 , nullptr );
//     TEST_NODE( tree.data.children(0)               , "y"   , 0 , 1 , 1 , 1 , &tree.data );
// 
//     TEST_NODE( tree.data2                          , "minus" , 2 , 8 , 4 , 0 , nullptr );
//     TEST_NODE( tree.data2.children(0)              , "plus"  , 2 , 6 , 3 , 1 , &tree.data2 );
//     TEST_NODE( tree.data2.children(0).children(0)  , "sin"   , 1 , 2 , 2 , 2 , tree.data2.children_ptr(0) );
//     TEST_NODE( tree.data2.children(0).children(1)  , "minus" , 2 , 3 , 2 , 2 , tree.data2.children_ptr(0) );
//     TEST_NODE( tree.data2.children(1)              , "x"     , 0 , 1 , 1 , 1 , &tree.data2 );
// 
//     TEST_NODE( tree.data2.children(0).children(0).children(0)  , "x" , 0 , 1 , 1 , 3 , tree.data2.children(0).children_ptr(0) );
//     TEST_NODE( tree.data2.children(0).children(1).children(0)  , "y" , 0 , 1 , 1 , 3 , tree.data2.children(0).children_ptr(1) );
//     TEST_NODE( tree.data2.children(0).children(1).children(1)  , "2" , 0 , 1 , 1 , 3 , tree.data2.children(0).children_ptr(1) );
// }
// 
// TEST( tree_tests , swap_one_2 )
// {
//     test_tree tree;
//     tree.data2.swap( tree.data.at(3) );
// 
//     TEST_NODE( tree.data                         , "plus"  , 2 , 7 , 4 , 0 , nullptr );
//     TEST_NODE( tree.data.children(0)             , "sin"   , 1 , 2 , 2 , 1 , &tree.data );
//     TEST_NODE( tree.data.children(0).children(0) , "x"     , 0 , 1 , 1 , 2 , tree.data.children_ptr(0) );
//     TEST_NODE( tree.data.children(1)             , "minus" , 2 , 4 , 3 , 1 , &tree.data );
//     TEST_NODE( tree.data.children(1).children(0) , "cos"   , 1 , 2 , 2 , 2 , tree.data.children_ptr(1) );
//     TEST_NODE( tree.data.children(1).children(1) , "x"     , 0 , 1 , 1 , 2 , tree.data.children_ptr(1) );
// 
//     TEST_NODE( tree.data.children(1).children(0).children(0) , "y" , 0 , 1 , 1 , 3 , tree.data.children(1).children_ptr(0) );
// 
//     TEST_NODE( tree.data2                         , "minus"  , 2 , 3 , 2 , 0 , nullptr );
//     TEST_NODE( tree.data2.children(0)             , "y"      , 0 , 1 , 1 , 1 , &tree.data2 );
//     TEST_NODE( tree.data2.children(1)             , "2"      , 0 , 1 , 1 , 1 , &tree.data2 );
// }
// 
// 
// TEST( tree_tests , swap_two_1 )
// {
//     test_tree tree;
//     tree.data.at(1).swap( tree.data2.at(3) );
// 
//     TEST_NODE( tree.data                         , "plus"  , 2 , 5 , 3 , 0 , nullptr );
//     TEST_NODE( tree.data.children(0)             , "x"     , 0 , 1 , 1 , 1 , &tree.data );
//     TEST_NODE( tree.data.children(1)             , "minus" , 2 , 3 , 2 , 1 , &tree.data );
//     TEST_NODE( tree.data.children(1).children(0) , "y"     , 0 , 1 , 1 , 2 , tree.data.children_ptr(1) );
//     TEST_NODE( tree.data.children(1).children(1) , "2"     , 0 , 1 , 1 , 2 , tree.data.children_ptr(1) );
// 
//     TEST_NODE( tree.data2                         , "minus" , 2 , 5 , 3 , 0 , nullptr );
//     TEST_NODE( tree.data2.children(0)             , "cos"   , 1 , 2 , 2 , 1 , &tree.data2 );
//     TEST_NODE( tree.data2.children(1)             , "sin"   , 1 , 2 , 2 , 1 , &tree.data2 );
//     TEST_NODE( tree.data2.children(0).children(0) , "y"     , 0 , 1 , 1 , 2 , tree.data2.children_ptr(0) );
//     TEST_NODE( tree.data2.children(1).children(0) , "x"     , 0 , 1 , 1 , 2 , tree.data2.children_ptr(1) );
// }
// 
// TEST( tree_tests , swap_two_2 )
// {
//     test_tree tree;
//     tree.data.at(3).swap( tree.data2.at(3) );
// 
//     TEST_NODE( tree.data                         , "plus"  , 2 , 4 , 3 , 0 , nullptr );
//     TEST_NODE( tree.data.children(0)             , "sin"   , 1 , 2 , 2 , 1 , &tree.data );
//     TEST_NODE( tree.data.children(1)             , "x"     , 0 , 1 , 1 , 1 , &tree.data );
//     TEST_NODE( tree.data.children(0).children(0) , "x"     , 0 , 1 , 1 , 2 , tree.data.children_ptr(0) );
// 
//     TEST_NODE( tree.data2                         , "minus" , 2 , 6 , 3 , 0 , nullptr );
//     TEST_NODE( tree.data2.children(0)             , "cos"   , 1 , 2 , 2 , 1 , &tree.data2 );
//     TEST_NODE( tree.data2.children(1)             , "minus" , 2 , 3 , 2 , 1 , &tree.data2 );
//     TEST_NODE( tree.data2.children(0).children(0) , "y"     , 0 , 1 , 1 , 2 , tree.data2.children_ptr(0) );
//     TEST_NODE( tree.data2.children(1).children(0) , "y"     , 0 , 1 , 1 , 2 , tree.data2.children_ptr(1) );
//     TEST_NODE( tree.data2.children(1).children(1) , "2"     , 0 , 1 , 1 , 2 , tree.data2.children_ptr(1) );
// }
// 
// 
// 
// TEST( tree_tests , swap_two_3 )
// {
//     test_tree tree;
//     tree.data.at(1).swap( tree.data.at(3) );
// 
//     TEST_NODE( tree.data                         , "plus"  , 2 , 6 , 3 , 0 , nullptr );
//     TEST_NODE( tree.data.children(0)             , "minus" , 2 , 3 , 2 , 1 , &tree.data );
//     TEST_NODE( tree.data.children(1)             , "sin"   , 1 , 2 , 2 , 1 , &tree.data );
//     TEST_NODE( tree.data.children(0).children(0) , "y"     , 0 , 1 , 1 , 2 , tree.data.children_ptr(0) );
//     TEST_NODE( tree.data.children(0).children(1) , "2"     , 0 , 1 , 1 , 2 , tree.data.children_ptr(0) );
//     TEST_NODE( tree.data.children(1).children(0) , "x"     , 0 , 1 , 1 , 2 , tree.data.children_ptr(1) );
// }
