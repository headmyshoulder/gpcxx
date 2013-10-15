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

#define TESTNAME general_tree_tests

using namespace std;
using namespace gpcxx;

   
template< typename Cursor >
void test_cursor( Cursor n , std::string const& value , size_t arity , size_t height , size_t level )
{
    EXPECT_EQ( *n , value );
    EXPECT_EQ( n.size() , arity );
    EXPECT_EQ( n.height() , height );
    EXPECT_EQ( n.level() , level );
}

template< typename T >
void test_cursor( gpcxx::detail::intrusive_cursor< T > n , std::string const& value , size_t arity , size_t height , size_t level )
{
    EXPECT_EQ( n.node()->name() , value );
    EXPECT_EQ( n.size() , arity );
    EXPECT_EQ( n.height() , height );
    EXPECT_EQ( n.level() , level );
}


template <class T>
class TESTNAME : public testing::Test
{
protected:
    
    typedef typename get_tree_type< T >::type tree_type;
    typedef typename get_node_factory< T >::type node_factory_type;
    typedef typename tree_type::cursor cursor;
    typedef test_tree< T > test_tree_type;
    
    TESTNAME( void )
    : m_tree() , m_test_trees()
    { };

    tree_type m_tree;
    test_tree_type m_test_trees;
    node_factory_type m_factory;
};


using testing::Types;

typedef Types< basic_tree_tag , intrusive_tree_tag > Implementations;

TYPED_TEST_CASE( TESTNAME , Implementations );


TYPED_TEST( TESTNAME , default_construct )
{
    auto root = this->m_tree.root();
    EXPECT_EQ( this->m_tree.size() , 0 );
    EXPECT_TRUE( this->m_tree.empty() );
}


TYPED_TEST( TESTNAME , insert_value )
{
    auto root = this->m_tree.root();
    auto n1 = this->m_tree.insert_below( root , this->m_factory( "+" ) );
    
    EXPECT_EQ( this->m_tree.size() , 1 );
    EXPECT_FALSE( this->m_tree.empty() );
    test_cursor( n1 , "+" , 0 , 1 , 0 );
    test_cursor( this->m_tree.root() , "+" , 0 , 1 , 0 );

    auto n2 = this->m_tree.insert_below( n1 , this->m_factory( "11" ) );
    auto n3 = this->m_tree.insert_below( n1 , this->m_factory( "12" ) );
    
    EXPECT_FALSE( this->m_tree.empty() );
    EXPECT_EQ( this->m_tree.size() , 3 );
    test_cursor( this->m_tree.root() , "+" , 2 , 2 , 0 );
    test_cursor( this->m_tree.root().children(0) , "11" , 0 , 1 , 1 );
    test_cursor( this->m_tree.root().children(1) , "12" , 0 , 1 , 1 );
    test_cursor( n2 , "11" , 0 , 1 , 1 );
    test_cursor( n3 , "12" , 0 , 1 , 1 );
}

TYPED_TEST( TESTNAME , insert_and_erase )
{
    this->m_tree.insert_below( this->m_tree.root() , this->m_factory( "+" ) );
    auto n1 = this->m_tree.insert_below( this->m_tree.root() , this->m_factory( "-" ) );
    auto n2 = this->m_tree.insert_below( this->m_tree.root() , this->m_factory( "*" ) );
    auto n3 = this->m_tree.insert_below( n1 , this->m_factory( "13" ) );
    auto n4 = this->m_tree.insert_below( n1 , this->m_factory( "14" ) );
    auto n5 = this->m_tree.insert_below( n2 , this->m_factory( "15" ) );
    auto n6 = this->m_tree.insert_below( n2 , this->m_factory( "16" ) );
    
    EXPECT_EQ( this->m_tree.size() , 7 );
    EXPECT_FALSE( this->m_tree.empty() );
    test_cursor( this->m_tree.root() , "+" , 2 , 3 , 0 );
    test_cursor( this->m_tree.root().children(0) , "-" , 2 , 2 , 1 );
    test_cursor( this->m_tree.root().children(0).children(0) , "13" , 0 , 1 , 2 );
    test_cursor( this->m_tree.root().children(0).children(1) , "14" , 0 , 1 , 2 );
    test_cursor( this->m_tree.root().children(1) , "*" , 2 , 2 , 1 );
    test_cursor( this->m_tree.root().children(1).children(0) , "15" , 0 , 1 , 2 );
    test_cursor( this->m_tree.root().children(1).children(1) , "16" , 0 , 1 , 2 );
    
    this->m_tree.erase( n2 );
    
    EXPECT_EQ( this->m_tree.size() , 4 );
    EXPECT_FALSE( this->m_tree.empty() );
    test_cursor( this->m_tree.root() , "+" , 1 , 3 , 0 );
    test_cursor( this->m_tree.root().children(0) , "-" , 2 , 2 , 1 );
    test_cursor( this->m_tree.root().children(0).children(0) , "13" , 0 , 1 , 2 );
    test_cursor( this->m_tree.root().children(0).children(1) , "14" , 0 , 1 , 2 );
}



TYPED_TEST( TESTNAME , cursor_parents )
{
    // EXPECT_EQ( this->m_test_trees.data.root().children(0).children(0).parent() , this->m_test_trees.data.root().children(0) );
}

TYPED_TEST( TESTNAME , insert_cursor )
{
    typename TESTNAME< TypeParam >::cursor c = this->m_test_trees.data.root().children(1);
    this->m_test_trees.data.erase( c.children(1) );
    typename TESTNAME< TypeParam >::cursor c2 = this->m_test_trees.data2.root();
    this->m_test_trees.data.insert_below( c , c2 );
    
    EXPECT_EQ( this->m_test_trees.data.size() , 9 );
    EXPECT_FALSE( this->m_test_trees.data.empty() );
    EXPECT_EQ( this->m_test_trees.data2.size() , 4 );
    EXPECT_FALSE( this->m_test_trees.data2.empty() );
    
    test_cursor( this->m_test_trees.data.root() , "plus" , 2 , 5 , 0 );
    test_cursor( this->m_test_trees.data.root().children(0) , "sin" , 1 , 2 , 1 );
    test_cursor( this->m_test_trees.data.root().children(0).children(0) , "x" , 0 , 1 , 2 );
    test_cursor( this->m_test_trees.data.root().children(1) , "minus" , 2 , 4 , 1 );
    test_cursor( this->m_test_trees.data.root().children(1).children(0) , "y" , 0 , 1 , 2 );
    test_cursor( this->m_test_trees.data.root().children(1).children(1) , "minus" , 2 , 3 , 2 );
    test_cursor( this->m_test_trees.data.root().children(1).children(1).children(0) , "cos" , 1 , 2 , 3 );
    test_cursor( this->m_test_trees.data.root().children(1).children(1).children(0).children(0) , "y" , 0 , 1 , 4 );
    test_cursor( this->m_test_trees.data.root().children(1).children(1).children(1) , "x" , 0 , 1 , 3 );
}






















// TYPED_TEST( TESTNAME , assign )
// {
//     this->m_test_trees.data.assign( this->m_test_trees.data2.root() );
//     
//     EXPECT_EQ( this->m_test_trees.data.size() , 4 );
//     EXPECT_FALSE( this->m_test_trees.data.empty() );
//     test_cursor( this->m_test_trees.data.root() , "minus" , 2 , 3 , 0 );
//     test_cursor( this->m_test_trees.data.root().children(0) , "cos" , 1 , 2 , 1 );
//     test_cursor( this->m_test_trees.data.root().children(0).children(0) , "y" , 0 , 1 , 2 );
//     test_cursor( this->m_test_trees.data.root().children(1) , "x" , 0 , 1 , 1 );
// }
// 
// TYPED_TEST( TESTNAME , clear )
// {
//     this->m_test_trees.data.clear();
//     
//     EXPECT_EQ( this->m_test_trees.data.size() , 0 );
//     EXPECT_TRUE( this->m_test_trees.data.empty() );
// }
// 
// TYPED_TEST( TESTNAME , iterator_construct )
// {
//     typename TESTNAME< TypeParam >::tree_type t( this->m_test_trees.data2.root() );
//     
//     EXPECT_EQ( t.size() , 4 );
//     EXPECT_FALSE( t.empty() );
//     test_cursor( t.root() , "minus" , 2 , 3 , 0 );
//     test_cursor( t.root().children(0) , "cos" , 1 , 2 , 1 );
//     test_cursor( t.root().children(0).children(0) , "y" , 0 , 1 , 2 );
//     test_cursor( t.root().children(1) , "x" , 0 , 1 , 1 );
// }
// 
// TYPED_TEST( TESTNAME , copy_construct )
// {
//     typename TESTNAME< TypeParam >::tree_type t( this->m_test_trees.data2 );
//     
//     EXPECT_EQ( this->m_test_trees.data2.size() , 4 );
//     EXPECT_FALSE( this->m_test_trees.data2.empty() );
//     EXPECT_EQ( t.size() , 4 );
//     EXPECT_FALSE( t.empty() );
//     test_cursor( t.root() , "minus" , 2 , 3 , 0 );
//     test_cursor( t.root().children(0) , "cos" , 1 , 2 , 1 );
//     test_cursor( t.root().children(0).children(0) , "y" , 0 , 1 , 2 );
//     test_cursor( t.root().children(1) , "x" , 0 , 1 , 1 );
// }
// 
// TYPED_TEST( TESTNAME , move_construct )
// {
//     typename TESTNAME< TypeParam >::tree_type t( std::move( this->m_test_trees.data2 ) );
//     
//     EXPECT_EQ( this->m_test_trees.data2.size() , 0 );
//     EXPECT_TRUE( this->m_test_trees.data2.empty() );
//     
//     EXPECT_EQ( t.size() , 4 );
//     EXPECT_FALSE( t.empty() );
//     test_cursor( t.root() , "minus" , 2 , 3 , 0 );
//     test_cursor( t.root().children(0) , "cos" , 1 , 2 , 1 );
//     test_cursor( t.root().children(0).children(0) , "y" , 0 , 1 , 2 );
//     test_cursor( t.root().children(1) , "x" , 0 , 1 , 1 );
// }
// 
// TYPED_TEST( TESTNAME , copy_assign )
// {
//     this->m_test_trees.data = this->m_test_trees.data2;
//     
//     EXPECT_EQ( this->m_test_trees.data2.size() , 4 );
//     EXPECT_FALSE( this->m_test_trees.data2.empty() );
//     
//     EXPECT_EQ( this->m_test_trees.data.size() , 4 );
//     EXPECT_FALSE( this->m_test_trees.data.empty() );
//     test_cursor( this->m_test_trees.data.root() , "minus" , 2 , 3 , 0 );
//     test_cursor( this->m_test_trees.data.root().children(0) , "cos" , 1 , 2 , 1 );
//     test_cursor( this->m_test_trees.data.root().children(0).children(0) , "y" , 0 , 1 , 2 );
//     test_cursor( this->m_test_trees.data.root().children(1) , "x" , 0 , 1 , 1 );
// }
// 
// TYPED_TEST( TESTNAME , move_assign )
// {
//     this->m_test_trees.data = std::move( this->m_test_trees.data2 );
//     
//     EXPECT_EQ( this->m_test_trees.data2.size() , 0 );
//     EXPECT_TRUE( this->m_test_trees.data2.empty() );
//     
//     EXPECT_EQ( this->m_test_trees.data.size() , 4 );
//     EXPECT_FALSE( this->m_test_trees.data.empty() );
//     test_cursor( this->m_test_trees.data.root() , "minus" , 2 , 3 , 0 );
//     test_cursor( this->m_test_trees.data.root().children(0) , "cos" , 1 , 2 , 1 );
//     test_cursor( this->m_test_trees.data.root().children(0).children(0) , "y" , 0 , 1 , 2 );
//     test_cursor( this->m_test_trees.data.root().children(1) , "x" , 0 , 1 , 1 );
// }
// 
// 
// TYPED_TEST( TESTNAME , swap_method )
// {
//     this->m_test_trees.data.swap( this->m_test_trees.data2 );
//     
//     EXPECT_EQ( this->m_test_trees.data.size() , 4 );
//     EXPECT_FALSE( this->m_test_trees.data.empty() );
//     test_cursor( this->m_test_trees.data.root() , "minus" , 2 , 3 , 0 );
//     test_cursor( this->m_test_trees.data.root().children(0) , "cos" , 1 , 2 , 1 );
//     test_cursor( this->m_test_trees.data.root().children(0).children(0) , "y" , 0 , 1 , 2 );
//     test_cursor( this->m_test_trees.data.root().children(1) , "x" , 0 , 1 , 1 );
// 
//     EXPECT_EQ( this->m_test_trees.data2.size() , 6 );
//     EXPECT_FALSE( this->m_test_trees.data2.empty() );
//     test_cursor( this->m_test_trees.data2.root() , "plus" , 2 , 3 , 0 );
//     test_cursor( this->m_test_trees.data2.root().children(0) , "sin" , 1 , 2 , 1 );
//     test_cursor( this->m_test_trees.data2.root().children(0).children(0) , "x" , 0 , 1 , 2 );
//     test_cursor( this->m_test_trees.data2.root().children(1) , "minus" , 2 , 2 , 1 );
//     test_cursor( this->m_test_trees.data2.root().children(1).children(0) , "y" , 0 , 1 , 2 );
//     test_cursor( this->m_test_trees.data2.root().children(1).children(1) , "2" , 0 , 1 , 2 );
// }
// 
// TYPED_TEST( TESTNAME , swap_function )
// {
//     swap( this->m_test_trees.data , this->m_test_trees.data2 );
//     
//     EXPECT_EQ( this->m_test_trees.data.size() , 4 );
//     EXPECT_FALSE( this->m_test_trees.data.empty() );
//     test_cursor( this->m_test_trees.data.root() , "minus" , 2 , 3 , 0 );
//     test_cursor( this->m_test_trees.data.root().children(0) , "cos" , 1 , 2 , 1 );
//     test_cursor( this->m_test_trees.data.root().children(0).children(0) , "y" , 0 , 1 , 2 );
//     test_cursor( this->m_test_trees.data.root().children(1) , "x" , 0 , 1 , 1 );
// 
//     EXPECT_EQ( this->m_test_trees.data2.size() , 6 );
//     EXPECT_FALSE( this->m_test_trees.data2.empty() );
//     test_cursor( this->m_test_trees.data2.root() , "plus" , 2 , 3 , 0 );
//     test_cursor( this->m_test_trees.data2.root().children(0) , "sin" , 1 , 2 , 1 );
//     test_cursor( this->m_test_trees.data2.root().children(0).children(0) , "x" , 0 , 1 , 2 );
//     test_cursor( this->m_test_trees.data2.root().children(1) , "minus" , 2 , 2 , 1 );
//     test_cursor( this->m_test_trees.data2.root().children(1).children(0) , "y" , 0 , 1 , 2 );
//     test_cursor( this->m_test_trees.data2.root().children(1).children(1) , "2" , 0 , 1 , 2 );
// }
// 
// TYPED_TEST( TESTNAME , equal_compare )
// {
//     typename TESTNAME< TypeParam >::tree_type t = this->m_test_trees.data;
//     
//     EXPECT_FALSE( this->m_test_trees.data == this->m_test_trees.data2 );
//     EXPECT_TRUE( this->m_test_trees.data != this->m_test_trees.data2 );
//     EXPECT_NE( ( this->m_test_trees.data ) , ( this->m_test_trees.data2 ) );
//     
//     EXPECT_TRUE( this->m_test_trees.data == t );
//     EXPECT_FALSE( this->m_test_trees.data != t );
//     EXPECT_EQ( ( this->m_test_trees.data ) , t );
// }
// 
// TYPED_TEST( TESTNAME , rank_is )
// {
//     EXPECT_EQ( this->m_test_trees.data.rank_is( 0 ) , this->m_test_trees.data.root() );
//     EXPECT_EQ( this->m_test_trees.data.rank_is( 1 ) , this->m_test_trees.data.root().children(0) );
//     EXPECT_EQ( this->m_test_trees.data.rank_is( 2 ) , this->m_test_trees.data.root().children(1) );
//     EXPECT_EQ( this->m_test_trees.data.rank_is( 3 ) , this->m_test_trees.data.root().children(0).children(0) );
//     EXPECT_EQ( this->m_test_trees.data.rank_is( 4 ) , this->m_test_trees.data.root().children(1).children(0) );
//     EXPECT_EQ( this->m_test_trees.data.rank_is( 5 ) , this->m_test_trees.data.root().children(1).children(1) );
//     EXPECT_EQ( this->m_test_trees.data.rank_is( 6 ) , this->m_test_trees.data.shoot() );
//     EXPECT_EQ( this->m_test_trees.data.rank_is( 7 ) , this->m_test_trees.data.shoot() );
// }
// 
// TYPED_TEST( TESTNAME , swap_subtrees1 )
// {
//     typename TESTNAME< TypeParam >::tree_type t1 , t2;
//     swap_subtrees( t1 , t1.root() , t2 , t2.root() );
//     EXPECT_TRUE( t1.empty() );
//     EXPECT_TRUE( t2.empty() );
//     EXPECT_EQ( t1.size() , 0 );
//     EXPECT_EQ( t2.size() , 0 );
// }
// 
// TYPED_TEST( TESTNAME , swap_subtrees2 )
// {
//     typename TESTNAME< TypeParam >::tree_type t1 , t2;
//     t1.insert_below( t1.root() , "+" );
//     swap_subtrees( t1 , t1.root() , t2 , t2.root() );
//     EXPECT_TRUE( t1.empty() );
//     EXPECT_FALSE( t2.empty() );
//     EXPECT_EQ( t1.size() , 0 );
//     EXPECT_EQ( t2.size() , 1 );
// }
// 
// TYPED_TEST( TESTNAME , swap_subtrees3 )
// {
//     typename TESTNAME< TypeParam >::tree_type t1 , t2;
//     auto c1 = t1.insert_below( t1.root() , "+" );
//     t1.insert_below( c1 , "-" );
//     swap_subtrees( t1 , t1.root() , t2 , t2.root() );
//     EXPECT_TRUE( t1.empty() );
//     EXPECT_FALSE( t2.empty() );
//     EXPECT_EQ( t1.size() , 0 );
//     EXPECT_EQ( t2.size() , 2 );
//     test_cursor( t2.root() , "+" , 1 , 2 , 0 );
//     test_cursor( t2.root().children(0) , "-" , 0 , 1 , 1 );
// }
// 
// TYPED_TEST( TESTNAME , swap_subtrees4 )
// {
//     typename TESTNAME< TypeParam >::tree_type t1 , t2;
//     auto c1 = t1.insert_below( t1.root() , "+" );
//     t1.insert_below( c1 , "-" );
//     swap_subtrees( t1 , t1.root().children(0) , t2 , t2.root() );
//     EXPECT_FALSE( t1.empty() );
//     EXPECT_FALSE( t2.empty() );
//     EXPECT_EQ( t1.size() , 1 );
//     EXPECT_EQ( t2.size() , 1 );
//     test_cursor( t1.root() , "+" , 0 , 1 , 0 );
//     test_cursor( t2.root() , "-" , 0 , 1 , 0 );
// }
// 
// TYPED_TEST( TESTNAME , swap_subtrees5 )
// {
//     swap_subtrees( this->m_test_trees.data , this->m_test_trees.data.root().children(1)  , this->m_test_trees.data2 , this->m_test_trees.data2.root().children(0) );
//     
//     EXPECT_FALSE( this->m_test_trees.data.empty() );
//     EXPECT_FALSE( this->m_test_trees.data2.empty() );
//     EXPECT_EQ( this->m_test_trees.data.size() , 5 );
//     EXPECT_EQ( this->m_test_trees.data2.size() , 5 );
//     test_cursor( this->m_test_trees.data.root() , "plus" , 2 , 3 , 0 );
//     test_cursor( this->m_test_trees.data.root().children(0) , "sin" , 1 , 2 , 1 );
//     test_cursor( this->m_test_trees.data.root().children(0).children(0) , "x" , 0 , 1 , 2 );
//     test_cursor( this->m_test_trees.data.root().children(1) , "cos" , 1 , 2 , 1 );
//     test_cursor( this->m_test_trees.data.root().children(1).children(0) , "y" , 0 , 1 , 2 );
//     
//     test_cursor( this->m_test_trees.data2.root() , "minus" , 2 , 3 , 0 );
//     test_cursor( this->m_test_trees.data2.root().children(0) , "minus" , 2 , 2 , 1 );
//     test_cursor( this->m_test_trees.data2.root().children(0).children(0) , "y" , 0 , 1 , 2 );
//     test_cursor( this->m_test_trees.data2.root().children(0).children(1) , "2" , 0 , 1 , 2 );
//     test_cursor( this->m_test_trees.data2.root().children(1) , "x" , 0 , 1 , 1 );
// }