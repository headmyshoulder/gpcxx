/*
 test/tree/general_tree.cpp

 Copyright 2013 Karsten Ahnert

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "../common/test_tree.hpp"

#include <gtest/gtest.h>

#include <sstream>

using namespace std;
using namespace gpcxx;

#define TEST_NODE( n , VALUE , ARITY , HEIGHT , LEVEL )          \
    EXPECT_EQ( *(n) , VALUE );                                   \
    EXPECT_EQ( (n).size() , ARITY );                             \
    EXPECT_EQ( (n).height() , HEIGHT );                          \
    EXPECT_EQ( (n).level() , LEVEL )


template <class T>
class general_tree_tests : public testing::Test
{
protected:
    
    typedef typename get_tree_type< T >::type tree_type;
    typedef typename tree_type::cursor cursor;
    typedef test_tree< T > test_tree_type;
    
    general_tree_tests( void )
    : m_tree( new tree_type() )
    , m_test_trees( new test_tree_type() )
    { };

    virtual ~general_tree_tests( void )
    {
        delete m_tree;
        delete m_test_trees;
    }

    tree_type *m_tree;
    test_tree_type *m_test_trees;
};


using testing::Types;

typedef Types< basic_tree_tag , recursive_tree_tag , basic_tree_fast_tag > Implementations;

TYPED_TEST_CASE( general_tree_tests , Implementations );


TYPED_TEST( general_tree_tests , default_construct )
{
    auto root = this->m_tree->root();
    EXPECT_EQ( this->m_tree->size() , 0 );
    EXPECT_TRUE( this->m_tree->empty() );
}


TYPED_TEST( general_tree_tests , insert_value )
{
    auto root = this->m_tree->root();
    auto n1 = this->m_tree->insert_below( root , "+" );
    
    EXPECT_EQ( this->m_tree->size() , 1 );
    EXPECT_FALSE( this->m_tree->empty() );
    TEST_NODE( n1 , "+" , 0 , 1 , 0 );
    TEST_NODE( this->m_tree->root() , "+" , 0 , 1 , 0 );

    auto n2 = this->m_tree->insert_below( n1 , "11" );
    auto n3 = this->m_tree->insert_below( n1 , "12" );
    EXPECT_FALSE( this->m_tree->empty() );
    EXPECT_EQ( this->m_tree->size() , 3 );
    TEST_NODE( this->m_tree->root() , "+" , 2 , 2 , 0 );
    TEST_NODE( this->m_tree->root().children(0) , "11" , 0 , 1 , 1 );
    TEST_NODE( this->m_tree->root().children(1) , "12" , 0 , 1 , 1 );
    TEST_NODE( n2 , "11" , 0 , 1 , 1 );
    TEST_NODE( n3 , "12" , 0 , 1 , 1 );
}

TYPED_TEST( general_tree_tests , insert_and_erase )
{
    this->m_tree->insert_below( this->m_tree->root() , "+" );
    auto n1 = this->m_tree->insert_below( this->m_tree->root() , "-" );
    auto n2 = this->m_tree->insert_below( this->m_tree->root() , "*" );
    auto n3 = this->m_tree->insert_below( n1 , "13" );
    auto n4 = this->m_tree->insert_below( n1 , "14" );
    auto n5 = this->m_tree->insert_below( n2 , "15" );
    auto n6 = this->m_tree->insert_below( n2 , "16" );
    
    EXPECT_EQ( this->m_tree->size() , 7 );
    EXPECT_FALSE( this->m_tree->empty() );
    TEST_NODE( this->m_tree->root() , "+" , 2 , 3 , 0 );
    TEST_NODE( this->m_tree->root().children(0) , "-" , 2 , 2 , 1 );
    TEST_NODE( this->m_tree->root().children(0).children(0) , "13" , 0 , 1 , 2 );
    TEST_NODE( this->m_tree->root().children(0).children(1) , "14" , 0 , 1 , 2 );
    TEST_NODE( this->m_tree->root().children(1) , "*" , 2 , 2 , 1 );
    TEST_NODE( this->m_tree->root().children(1).children(0) , "15" , 0 , 1 , 2 );
    TEST_NODE( this->m_tree->root().children(1).children(1) , "16" , 0 , 1 , 2 );
    
    this->m_tree->erase( n2 );
    
    EXPECT_EQ( this->m_tree->size() , 4 );
    EXPECT_FALSE( this->m_tree->empty() );
    TEST_NODE( this->m_tree->root() , "+" , 1 , 3 , 0 );
    TEST_NODE( this->m_tree->root().children(0) , "-" , 2 , 2 , 1 );
    TEST_NODE( this->m_tree->root().children(0).children(0) , "13" , 0 , 1 , 2 );
    TEST_NODE( this->m_tree->root().children(0).children(1) , "14" , 0 , 1 , 2 );
}



TYPED_TEST( general_tree_tests , cursor_parents )
{
    EXPECT_EQ( this->m_test_trees->data.root().children(0).children(0).parent() , this->m_test_trees->data.root().children(0) );
}

TYPED_TEST( general_tree_tests , insert_cursor )
{
    typename general_tree_tests< TypeParam >::cursor c = this->m_test_trees->data.root().children(1);
    this->m_test_trees->data.erase( c.children(1) );
    typename general_tree_tests< TypeParam >::cursor c2 = this->m_test_trees->data2.root();
    this->m_test_trees->data.insert_below( c , c2 );
    
    EXPECT_EQ( this->m_test_trees->data.size() , 9 );
    EXPECT_FALSE( this->m_test_trees->data.empty() );
    EXPECT_EQ( this->m_test_trees->data2.size() , 4 );
    EXPECT_FALSE( this->m_test_trees->data2.empty() );
    
    TEST_NODE( this->m_test_trees->data.root() , "plus" , 2 , 5 , 0 );
    TEST_NODE( this->m_test_trees->data.root().children(0) , "sin" , 1 , 2 , 1 );
    TEST_NODE( this->m_test_trees->data.root().children(0).children(0) , "x" , 0 , 1 , 2 );
    TEST_NODE( this->m_test_trees->data.root().children(1) , "minus" , 2 , 4 , 1 );
    TEST_NODE( this->m_test_trees->data.root().children(1).children(0) , "y" , 0 , 1 , 2 );
    TEST_NODE( this->m_test_trees->data.root().children(1).children(1) , "minus" , 2 , 3 , 2 );
    TEST_NODE( this->m_test_trees->data.root().children(1).children(1).children(0) , "cos" , 1 , 2 , 3 );
    TEST_NODE( this->m_test_trees->data.root().children(1).children(1).children(0).children(0) , "y" , 0 , 1 , 4 );
    TEST_NODE( this->m_test_trees->data.root().children(1).children(1).children(1) , "x" , 0 , 1 , 3 );
}






// TYPED_TEST( general_tree_tests , assign )
// {
//     this->m_test_trees->data.assign( this->m_test_trees->data2.root() );
//     
//     EXPECT_EQ( this->m_test_trees->data.size() , 4 );
//     EXPECT_FALSE( this->m_test_trees->data.empty() );
//     TEST_NODE( this->m_test_trees->data.root() , "minus" , 2 , 3 , 0 );
//     TEST_NODE( this->m_test_trees->data.root().children(0) , "cos" , 1 , 2 , 1 );
//     TEST_NODE( this->m_test_trees->data.root().children(0).children(0) , "y" , 0 , 1 , 2 );
//     TEST_NODE( this->m_test_trees->data.root().children(1) , "x" , 0 , 1 , 1 );
// }
// 
// TYPED_TEST( general_tree_tests , clear )
// {
//     this->m_test_trees->data.clear();
//     
//     EXPECT_EQ( this->m_test_trees->data.size() , 0 );
//     EXPECT_TRUE( this->m_test_trees->data.empty() );
// }
// 
// TYPED_TEST( general_tree_tests , iterator_construct )
// {
//     typename general_tree_tests< TypeParam >::tree_type t( this->m_test_trees->data2.root() );
//     
//     EXPECT_EQ( t.size() , 4 );
//     EXPECT_FALSE( t.empty() );
//     TEST_NODE( t.root() , "minus" , 2 , 3 , 0 );
//     TEST_NODE( t.root().children(0) , "cos" , 1 , 2 , 1 );
//     TEST_NODE( t.root().children(0).children(0) , "y" , 0 , 1 , 2 );
//     TEST_NODE( t.root().children(1) , "x" , 0 , 1 , 1 );
// }
// 
// TYPED_TEST( general_tree_tests , copy_construct )
// {
//     typename general_tree_tests< TypeParam >::tree_type t( this->m_test_trees->data2 );
//     
//     EXPECT_EQ( this->m_test_trees->data2.size() , 4 );
//     EXPECT_FALSE( this->m_test_trees->data2.empty() );
//     EXPECT_EQ( t.size() , 4 );
//     EXPECT_FALSE( t.empty() );
//     TEST_NODE( t.root() , "minus" , 2 , 3 , 0 );
//     TEST_NODE( t.root().children(0) , "cos" , 1 , 2 , 1 );
//     TEST_NODE( t.root().children(0).children(0) , "y" , 0 , 1 , 2 );
//     TEST_NODE( t.root().children(1) , "x" , 0 , 1 , 1 );
// }
// 
// TYPED_TEST( general_tree_tests , move_construct )
// {
//     typename general_tree_tests< TypeParam >::tree_type t( std::move( this->m_test_trees->data2 ) );
//     
//     EXPECT_EQ( this->m_test_trees->data2.size() , 0 );
//     EXPECT_TRUE( this->m_test_trees->data2.empty() );
//     
//     EXPECT_EQ( t.size() , 4 );
//     EXPECT_FALSE( t.empty() );
//     TEST_NODE( t.root() , "minus" , 2 , 3 , 0 );
//     TEST_NODE( t.root().children(0) , "cos" , 1 , 2 , 1 );
//     TEST_NODE( t.root().children(0).children(0) , "y" , 0 , 1 , 2 );
//     TEST_NODE( t.root().children(1) , "x" , 0 , 1 , 1 );
// }
// 
// TYPED_TEST( general_tree_tests , copy_assign )
// {
//     this->m_test_trees->data = this->m_test_trees->data2;
//     
//     EXPECT_EQ( this->m_test_trees->data2.size() , 4 );
//     EXPECT_FALSE( this->m_test_trees->data2.empty() );
//     
//     EXPECT_EQ( this->m_test_trees->data.size() , 4 );
//     EXPECT_FALSE( this->m_test_trees->data.empty() );
//     TEST_NODE( this->m_test_trees->data.root() , "minus" , 2 , 3 , 0 );
//     TEST_NODE( this->m_test_trees->data.root().children(0) , "cos" , 1 , 2 , 1 );
//     TEST_NODE( this->m_test_trees->data.root().children(0).children(0) , "y" , 0 , 1 , 2 );
//     TEST_NODE( this->m_test_trees->data.root().children(1) , "x" , 0 , 1 , 1 );
// }
// 
// TYPED_TEST( general_tree_tests , move_assign )
// {
//     this->m_test_trees->data = std::move( this->m_test_trees->data2 );
//     
//     EXPECT_EQ( this->m_test_trees->data2.size() , 0 );
//     EXPECT_TRUE( this->m_test_trees->data2.empty() );
//     
//     EXPECT_EQ( this->m_test_trees->data.size() , 4 );
//     EXPECT_FALSE( this->m_test_trees->data.empty() );
//     TEST_NODE( this->m_test_trees->data.root() , "minus" , 2 , 3 , 0 );
//     TEST_NODE( this->m_test_trees->data.root().children(0) , "cos" , 1 , 2 , 1 );
//     TEST_NODE( this->m_test_trees->data.root().children(0).children(0) , "y" , 0 , 1 , 2 );
//     TEST_NODE( this->m_test_trees->data.root().children(1) , "x" , 0 , 1 , 1 );
// }
// 
// 
// TYPED_TEST( general_tree_tests , swap_method )
// {
//     this->m_test_trees->data.swap( this->m_test_trees->data2 );
//     
//     EXPECT_EQ( this->m_test_trees->data.size() , 4 );
//     EXPECT_FALSE( this->m_test_trees->data.empty() );
//     TEST_NODE( this->m_test_trees->data.root() , "minus" , 2 , 3 , 0 );
//     TEST_NODE( this->m_test_trees->data.root().children(0) , "cos" , 1 , 2 , 1 );
//     TEST_NODE( this->m_test_trees->data.root().children(0).children(0) , "y" , 0 , 1 , 2 );
//     TEST_NODE( this->m_test_trees->data.root().children(1) , "x" , 0 , 1 , 1 );
// 
//     EXPECT_EQ( this->m_test_trees->data2.size() , 6 );
//     EXPECT_FALSE( this->m_test_trees->data2.empty() );
//     TEST_NODE( this->m_test_trees->data2.root() , "plus" , 2 , 3 , 0 );
//     TEST_NODE( this->m_test_trees->data2.root().children(0) , "sin" , 1 , 2 , 1 );
//     TEST_NODE( this->m_test_trees->data2.root().children(0).children(0) , "x" , 0 , 1 , 2 );
//     TEST_NODE( this->m_test_trees->data2.root().children(1) , "minus" , 2 , 2 , 1 );
//     TEST_NODE( this->m_test_trees->data2.root().children(1).children(0) , "y" , 0 , 1 , 2 );
//     TEST_NODE( this->m_test_trees->data2.root().children(1).children(1) , "2" , 0 , 1 , 2 );
// }
// 
// TYPED_TEST( general_tree_tests , swap_function )
// {
//     swap( this->m_test_trees->data , this->m_test_trees->data2 );
//     
//     EXPECT_EQ( this->m_test_trees->data.size() , 4 );
//     EXPECT_FALSE( this->m_test_trees->data.empty() );
//     TEST_NODE( this->m_test_trees->data.root() , "minus" , 2 , 3 , 0 );
//     TEST_NODE( this->m_test_trees->data.root().children(0) , "cos" , 1 , 2 , 1 );
//     TEST_NODE( this->m_test_trees->data.root().children(0).children(0) , "y" , 0 , 1 , 2 );
//     TEST_NODE( this->m_test_trees->data.root().children(1) , "x" , 0 , 1 , 1 );
// 
//     EXPECT_EQ( this->m_test_trees->data2.size() , 6 );
//     EXPECT_FALSE( this->m_test_trees->data2.empty() );
//     TEST_NODE( this->m_test_trees->data2.root() , "plus" , 2 , 3 , 0 );
//     TEST_NODE( this->m_test_trees->data2.root().children(0) , "sin" , 1 , 2 , 1 );
//     TEST_NODE( this->m_test_trees->data2.root().children(0).children(0) , "x" , 0 , 1 , 2 );
//     TEST_NODE( this->m_test_trees->data2.root().children(1) , "minus" , 2 , 2 , 1 );
//     TEST_NODE( this->m_test_trees->data2.root().children(1).children(0) , "y" , 0 , 1 , 2 );
//     TEST_NODE( this->m_test_trees->data2.root().children(1).children(1) , "2" , 0 , 1 , 2 );
// }
// 
// TYPED_TEST( general_tree_tests , equal_compare )
// {
//     typename general_tree_tests< TypeParam >::tree_type t = this->m_test_trees->data;
//     
//     EXPECT_FALSE( this->m_test_trees->data == this->m_test_trees->data2 );
//     EXPECT_TRUE( this->m_test_trees->data != this->m_test_trees->data2 );
//     EXPECT_NE( ( this->m_test_trees->data ) , ( this->m_test_trees->data2 ) );
//     
//     EXPECT_TRUE( this->m_test_trees->data == t );
//     EXPECT_FALSE( this->m_test_trees->data != t );
//     EXPECT_EQ( ( this->m_test_trees->data ) , t );
// }
// 
// TYPED_TEST( general_tree_tests , rank_is )
// {
//     EXPECT_EQ( this->m_test_trees->data.rank_is( 0 ) , this->m_test_trees->data.root() );
//     EXPECT_EQ( this->m_test_trees->data.rank_is( 1 ) , this->m_test_trees->data.root().children(0) );
//     EXPECT_EQ( this->m_test_trees->data.rank_is( 2 ) , this->m_test_trees->data.root().children(1) );
//     EXPECT_EQ( this->m_test_trees->data.rank_is( 3 ) , this->m_test_trees->data.root().children(0).children(0) );
//     EXPECT_EQ( this->m_test_trees->data.rank_is( 4 ) , this->m_test_trees->data.root().children(1).children(0) );
//     EXPECT_EQ( this->m_test_trees->data.rank_is( 5 ) , this->m_test_trees->data.root().children(1).children(1) );
//     EXPECT_EQ( this->m_test_trees->data.rank_is( 6 ) , this->m_test_trees->data.shoot() );
//     EXPECT_EQ( this->m_test_trees->data.rank_is( 7 ) , this->m_test_trees->data.shoot() );
// }
// 
// TYPED_TEST( general_tree_tests , swap_subtrees1 )
// {
//     typename general_tree_tests< TypeParam >::tree_type t1 , t2;
//     swap_subtrees( t1 , t1.root() , t2 , t2.root() );
//     EXPECT_TRUE( t1.empty() );
//     EXPECT_TRUE( t2.empty() );
//     EXPECT_EQ( t1.size() , 0 );
//     EXPECT_EQ( t2.size() , 0 );
// }
// 
// TYPED_TEST( general_tree_tests , swap_subtrees2 )
// {
//     typename general_tree_tests< TypeParam >::tree_type t1 , t2;
//     t1.insert_below( t1.root() , "+" );
//     swap_subtrees( t1 , t1.root() , t2 , t2.root() );
//     EXPECT_TRUE( t1.empty() );
//     EXPECT_FALSE( t2.empty() );
//     EXPECT_EQ( t1.size() , 0 );
//     EXPECT_EQ( t2.size() , 1 );
// }
// 
// TYPED_TEST( general_tree_tests , swap_subtrees3 )
// {
//     typename general_tree_tests< TypeParam >::tree_type t1 , t2;
//     auto c1 = t1.insert_below( t1.root() , "+" );
//     t1.insert_below( c1 , "-" );
//     swap_subtrees( t1 , t1.root() , t2 , t2.root() );
//     EXPECT_TRUE( t1.empty() );
//     EXPECT_FALSE( t2.empty() );
//     EXPECT_EQ( t1.size() , 0 );
//     EXPECT_EQ( t2.size() , 2 );
//     TEST_NODE( t2.root() , "+" , 1 , 2 , 0 );
//     TEST_NODE( t2.root().children(0) , "-" , 0 , 1 , 1 );
// }
// 
// TYPED_TEST( general_tree_tests , swap_subtrees4 )
// {
//     typename general_tree_tests< TypeParam >::tree_type t1 , t2;
//     auto c1 = t1.insert_below( t1.root() , "+" );
//     t1.insert_below( c1 , "-" );
//     swap_subtrees( t1 , t1.root().children(0) , t2 , t2.root() );
//     EXPECT_FALSE( t1.empty() );
//     EXPECT_FALSE( t2.empty() );
//     EXPECT_EQ( t1.size() , 1 );
//     EXPECT_EQ( t2.size() , 1 );
//     TEST_NODE( t1.root() , "+" , 0 , 1 , 0 );
//     TEST_NODE( t2.root() , "-" , 0 , 1 , 0 );
// }
// 
// TYPED_TEST( general_tree_tests , swap_subtrees5 )
// {
//     swap_subtrees( this->m_test_trees->data , this->m_test_trees->data.root().children(1)  , this->m_test_trees->data2 , this->m_test_trees->data2.root().children(0) );
//     
//     EXPECT_FALSE( this->m_test_trees->data.empty() );
//     EXPECT_FALSE( this->m_test_trees->data2.empty() );
//     EXPECT_EQ( this->m_test_trees->data.size() , 5 );
//     EXPECT_EQ( this->m_test_trees->data2.size() , 5 );
//     TEST_NODE( this->m_test_trees->data.root() , "plus" , 2 , 3 , 0 );
//     TEST_NODE( this->m_test_trees->data.root().children(0) , "sin" , 1 , 2 , 1 );
//     TEST_NODE( this->m_test_trees->data.root().children(0).children(0) , "x" , 0 , 1 , 2 );
//     TEST_NODE( this->m_test_trees->data.root().children(1) , "cos" , 1 , 2 , 1 );
//     TEST_NODE( this->m_test_trees->data.root().children(1).children(0) , "y" , 0 , 1 , 2 );
//     
//     TEST_NODE( this->m_test_trees->data2.root() , "minus" , 2 , 3 , 0 );
//     TEST_NODE( this->m_test_trees->data2.root().children(0) , "minus" , 2 , 2 , 1 );
//     TEST_NODE( this->m_test_trees->data2.root().children(0).children(0) , "y" , 0 , 1 , 2 );
//     TEST_NODE( this->m_test_trees->data2.root().children(0).children(1) , "2" , 0 , 1 , 2 );
//     TEST_NODE( this->m_test_trees->data2.root().children(1) , "x" , 0 , 1 , 1 );
// }