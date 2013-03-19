/*
 * linked_node.cpp
 * Date: 2013-02-20
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include "../common/test_tree.hpp"

#include <gp/tree/linked_node.hpp>

#include <gtest/gtest.h>



#define TEST_NODE( n , VALUE , ARITY , NUM_ELEMENTS , HEIGHT , LEVEL , PARENT ) \
    EXPECT_EQ( (n).value() , VALUE );                                   \
    EXPECT_EQ( (n).arity() , ARITY );                                   \
    EXPECT_EQ( (n).num_elements() , NUM_ELEMENTS ); \
    EXPECT_EQ( (n).height() , HEIGHT ); \
    EXPECT_EQ( (n).level() , LEVEL ); \
    EXPECT_EQ( (n).empty() , ( ARITY == 0 ) );     \
    EXPECT_EQ( (n).size() , ARITY );   \
    EXPECT_EQ( (n).parent_ptr() , PARENT )


using namespace std;

typedef gp::linked_node< char , 3 > node_type;



TEST( tree_tests , linked_node_copy_empty )
{
    node_type l1( 'a' );
    node_type l2( l1 );

    TEST_NODE( l1 , 'a' , 0 , 1 , 1 , 0 , nullptr );
    TEST_NODE( l2 , 'a' , 0 , 1 , 1 , 0 , nullptr );
    
}

TEST( tree_tests , linked_node_copy )
{
    node_type l1( 'a' );
    auto i1 = l1.emplace( 'b' );
    auto i2 = l1.emplace( 'c' );
    node_type l2( l1 );

    TEST_NODE( l1 , 'a' , 2 , 3 , 2 , 0 , nullptr );
    TEST_NODE( *i1 , 'b' , 0 , 1 , 1 , 1 , &l1 );
    TEST_NODE( *i2 , 'c' , 0 , 1 , 1 , 1 , &l1 );

    TEST_NODE( l2 , 'a' , 2 , 3 , 2 , 0 , nullptr );
    TEST_NODE( l2.children( 0 ) , 'b' , 0 , 1 , 1 , 1 , &l2 );
    TEST_NODE( l2.children( 1 ) , 'c' , 0 , 1 , 1 , 1 , &l2 );
}

TEST( tree_tests , linked_node_copy_2 )
{
    node_type l1( 'a' );
    auto i1 = l1.emplace( 'b' );
    auto i2 = l1.emplace( 'c' );
    auto i3 = i1->emplace( 'd' );
    auto i4 = i1->emplace( 'e' );
    auto i5 = i2->emplace( 'f' );

    node_type l2( l1 );

    TEST_NODE( l1 , 'a' , 2 , 6 , 3 , 0 , nullptr );
    TEST_NODE( *i1 , 'b' , 2 , 3 , 2 , 1 , &l1 );
    TEST_NODE( *i2 , 'c' , 1 , 2 , 2 , 1 , &l1 );
    TEST_NODE( *i3 , 'd' , 0 , 1 , 1 , 2 , &l1.children(0) );
    TEST_NODE( *i4 , 'e' , 0 , 1 , 1 , 2 , &l1.children(0) );
    TEST_NODE( *i5 , 'f' , 0 , 1 , 1 , 2 , &l1.children(1) );

    TEST_NODE( l2 , 'a' , 2 , 6 , 3 , 0 , nullptr );
    TEST_NODE( l2.children(0) , 'b' , 2 , 3 , 2 , 1 , &l2 );
    TEST_NODE( l2.children(1) , 'c' , 1 , 2 , 2 , 1 , &l2 );
    TEST_NODE( l2.children(0).children(0) , 'd' , 0 , 1 , 1 , 2 , &l2.children(0) );
    TEST_NODE( l2.children(0).children(1) , 'e' , 0 , 1 , 1 , 2 , &l2.children(0) );
    TEST_NODE( l2.children(1).children(0) , 'f' , 0 , 1 , 1 , 2 , &l2.children(1) );
}

TEST( tree_tests , linked_node_copy_3 )
{
    node_type l1( 'a' );
    auto i1 = l1.emplace( 'b' );
    auto i2 = l1.emplace( 'c' );
    auto i3 = i1->emplace( 'd' );
    auto i4 = i1->emplace( 'e' );
    auto i5 = i2->emplace( 'f' );

    node_type l2( l1.children(0) );

    TEST_NODE( l1 , 'a' , 2 , 6 , 3 , 0 , nullptr );
    TEST_NODE( *i1 , 'b' , 2 , 3 , 2 , 1 , &l1 );
    TEST_NODE( *i2 , 'c' , 1 , 2 , 2 , 1 , &l1 );
    TEST_NODE( *i3 , 'd' , 0 , 1 , 1 , 2 , &l1.children(0) );
    TEST_NODE( *i4 , 'e' , 0 , 1 , 1 , 2 , &l1.children(0) );
    TEST_NODE( *i5 , 'f' , 0 , 1 , 1 , 2 , &l1.children(1) );

    TEST_NODE( l2 , 'b' , 2 , 3 , 2 , 0 , nullptr );
    TEST_NODE( l2.children(0) , 'd' , 0 , 1 , 1 , 1 , &l2 );
    TEST_NODE( l2.children(1) , 'e' , 0 , 1 , 1 , 1 , &l2 );
}

TEST( tree_tests , linked_node_assign_empty )
{
    node_type l1( 'a' );
    node_type l2;
    l2 = l1;

    TEST_NODE( l1 , 'a' , 0 , 1 , 1 , 0 , nullptr );
    TEST_NODE( l2 , 'a' , 0 , 1 , 1 , 0 , nullptr );
}

TEST( tree_tests , linked_node_assign )
{
    node_type l1( 'a' );
    auto i1 = l1.emplace( 'b' );
    auto i2 = l1.emplace( 'c' );
    node_type l2;
    l2 = l1;

    TEST_NODE( l1 , 'a' , 2 , 3 , 2 , 0 , nullptr );
    TEST_NODE( *i1 , 'b' , 0 , 1 , 1 , 1 , &l1 );
    TEST_NODE( *i2 , 'c' , 0 , 1 , 1 , 1 , &l1 );

    TEST_NODE( l2 , 'a' , 2 , 3 , 2 , 0 , nullptr );
    TEST_NODE( l2.children( 0 ) , 'b' , 0 , 1 , 1 , 1 , &l2 );
    TEST_NODE( l2.children( 1 ) , 'c' , 0 , 1 , 1 , 1 , &l2 );
}

TEST( tree_tests , linked_node_assign_2 )
{
    node_type l1( 'a' );
    auto i1 = l1.emplace( 'b' );
    auto i2 = l1.emplace( 'c' );
    auto i3 = i1->emplace( 'd' );
    auto i4 = i1->emplace( 'e' );
    auto i5 = i2->emplace( 'f' );

    node_type l2;

    l2 = ( l1 );

    TEST_NODE( l1 , 'a' , 2 , 6 , 3 , 0 , nullptr );
    TEST_NODE( *i1 , 'b' , 2 , 3 , 2 , 1 , &l1 );
    TEST_NODE( *i2 , 'c' , 1 , 2 , 2 , 1 , &l1 );
    TEST_NODE( *i3 , 'd' , 0 , 1 , 1 , 2 , &l1.children(0) );
    TEST_NODE( *i4 , 'e' , 0 , 1 , 1 , 2 , &l1.children(0) );
    TEST_NODE( *i5 , 'f' , 0 , 1 , 1 , 2 , &l1.children(1) );

    TEST_NODE( l2 , 'a' , 2 , 6 , 3 , 0 , nullptr );
    TEST_NODE( l2.children(0) , 'b' , 2 , 3 , 2 , 1 , &l2 );
    TEST_NODE( l2.children(1) , 'c' , 1 , 2 , 2 , 1 , &l2 );
    TEST_NODE( l2.children(0).children(0) , 'd' , 0 , 1 , 1 , 2 , &l2.children(0) );
    TEST_NODE( l2.children(0).children(1) , 'e' , 0 , 1 , 1 , 2 , &l2.children(0) );
    TEST_NODE( l2.children(1).children(0) , 'f' , 0 , 1 , 1 , 2 , &l2.children(1) );
}

TEST( tree_tests , linked_node_assign_3 )
{
    node_type l1( 'a' );
    auto i1 = l1.emplace( 'b' );
    auto i2 = l1.emplace( 'c' );
    auto i3 = i1->emplace( 'd' );
    auto i4 = i1->emplace( 'e' );
    auto i5 = i2->emplace( 'f' );

    node_type l2;
    l2.emplace( 'b' );
    l2.emplace( 'c' );
    l2 = l1.children(0);

    TEST_NODE( l1 , 'a' , 2 , 6 , 3 , 0 , nullptr );
    TEST_NODE( *i1 , 'b' , 2 , 3 , 2 , 1 , &l1 );
    TEST_NODE( *i2 , 'c' , 1 , 2 , 2 , 1 , &l1 );
    TEST_NODE( *i3 , 'd' , 0 , 1 , 1 , 2 , &l1.children(0) );
    TEST_NODE( *i4 , 'e' , 0 , 1 , 1 , 2 , &l1.children(0) );
    TEST_NODE( *i5 , 'f' , 0 , 1 , 1 , 2 , &l1.children(1) );

    TEST_NODE( l2 , 'b' , 2 , 3 , 2 , 0 , nullptr );
    TEST_NODE( l2.children(0) , 'd' , 0 , 1 , 1 , 1 , &l2 );
    TEST_NODE( l2.children(1) , 'e' , 0 , 1 , 1 , 1 , &l2 );
}


TEST( tree_tests , linked_node_insert )
{
    node_type l1( 'a' );
    auto i1 = l1.emplace( 'b' );
    auto i2 = l1.emplace( 'c' );
    auto i3 = i1->emplace( 'd' );
    auto i4 = i1->emplace( 'e' );
    auto i5 = i2->emplace( 'f' );

    node_type l2;
    l2.emplace( 'b' );
    l2.emplace( 'c' );
    l2 = l1.children(0);
    l2.insert( l1.children( 1 ) );

    TEST_NODE( l1 , 'a' , 2 , 6 , 3 , 0 , nullptr );
    TEST_NODE( *i1 , 'b' , 2 , 3 , 2 , 1 , &l1 );
    TEST_NODE( *i2 , 'c' , 1 , 2 , 2 , 1 , &l1 );
    TEST_NODE( *i3 , 'd' , 0 , 1 , 1 , 2 , &l1.children(0) );
    TEST_NODE( *i4 , 'e' , 0 , 1 , 1 , 2 , &l1.children(0) );
    TEST_NODE( *i5 , 'f' , 0 , 1 , 1 , 2 , &l1.children(1) );

    TEST_NODE( l2 , 'b' , 3 , 5 , 3 , 0 , nullptr );
    TEST_NODE( l2.children(0) , 'd' , 0 , 1 , 1 , 1 , &l2 );
    TEST_NODE( l2.children(1) , 'e' , 0 , 1 , 1 , 1 , &l2 );
    TEST_NODE( l2.children(2) , 'c' , 1 , 2 , 2 , 1 , &l2 );
    TEST_NODE( l2.children(2).children(0) , 'f' , 0 , 1 , 1 , 2 , &l2.children(2) );
}


TEST( tree_tests , linked_emplace_inconsistent )
{
    node_type l1( 'a' );
    auto i1 = l1.emplace_inconsistent( 'b' );
    auto i2 = l1.emplace_inconsistent( 'c' );
    auto i3 = i1->emplace_inconsistent( 'd' );
    auto i4 = i1->emplace_inconsistent( 'e' );
    auto i5 = i2->emplace_inconsistent( 'f' );

    TEST_NODE( l1 , 'a' , 2 , 1 , 1 , 0 , nullptr );
    TEST_NODE( *i1 , 'b' , 2 , 1 , 1 , 1 , &l1 );
    TEST_NODE( *i2 , 'c' , 1 , 1 , 1 , 1 , &l1 );
    TEST_NODE( *i3 , 'd' , 0 , 1 , 1 , 2 , &l1.children(0) );
    TEST_NODE( *i4 , 'e' , 0 , 1 , 1 , 2 , &l1.children(0) );
    TEST_NODE( *i5 , 'f' , 0 , 1 , 1 , 2 , &l1.children(1) );

    l1.make_consistent();

    TEST_NODE( l1 , 'a' , 2 , 6 , 3 , 0 , nullptr );
    TEST_NODE( *i1 , 'b' , 2 , 3 , 2 , 1 , &l1 );
    TEST_NODE( *i2 , 'c' , 1 , 2 , 2 , 1 , &l1 );
    TEST_NODE( *i3 , 'd' , 0 , 1 , 1 , 2 , &l1.children(0) );
    TEST_NODE( *i4 , 'e' , 0 , 1 , 1 , 2 , &l1.children(0) );
    TEST_NODE( *i5 , 'f' , 0 , 1 , 1 , 2 , &l1.children(1) );
}



TEST( tree_tests , swap_real_1 )
{
    test_tree tree;
    tree.data.swap( tree.data2 );

    TEST_NODE( tree.data                         , "minus" , 2 , 4 , 3 , 0 , nullptr );
    TEST_NODE( tree.data.children(0)             , "cos"   , 1 , 2 , 2 , 1 , &tree.data );
    TEST_NODE( tree.data.children(0).children(0) , "y"     , 0 , 1 , 1 , 2 , tree.data.children_ptr(0) );
    TEST_NODE( tree.data.children(1)             , "x"     , 0 , 1 , 1 , 1 , &tree.data );

    TEST_NODE( tree.data2                         , "plus"  , 2 , 6 , 3 , 0 , nullptr );
    TEST_NODE( tree.data2.children(0)             , "sin"   , 1 , 2 , 2 , 1 , &tree.data2 );
    TEST_NODE( tree.data2.children(0).children(0) , "x"     , 0 , 1 , 1 , 2 , tree.data2.children_ptr(0) );
    TEST_NODE( tree.data2.children(1)             , "minus" , 2 , 3 , 2 , 1 , &tree.data2 );
    TEST_NODE( tree.data2.children(1).children(0) , "y"     , 0 , 1 , 1 , 2 , tree.data2.children_ptr(1) );
    TEST_NODE( tree.data2.children(1).children(1) , "2"     , 0 , 1 , 1 , 2 , tree.data2.children_ptr(1) );
}

TEST( tree_tests , swap_real_2 )
{
    test_tree tree;
    tree.data.swap( tree.data );

    TEST_NODE( tree.data2                         , "minus" , 2 , 4 , 3 , 0 , nullptr );
    TEST_NODE( tree.data2.children(0)             , "cos"   , 1 , 2 , 2 , 1 , &tree.data2 );
    TEST_NODE( tree.data2.children(0).children(0) , "y"     , 0 , 1 , 1 , 2 , tree.data2.children_ptr(0) );
    TEST_NODE( tree.data2.children(1)             , "x"     , 0 , 1 , 1 , 1 , &tree.data2 );

    TEST_NODE( tree.data                         , "plus"  , 2 , 6 , 3 , 0 , nullptr );
    TEST_NODE( tree.data.children(0)             , "sin"   , 1 , 2 , 2 , 1 , &tree.data );
    TEST_NODE( tree.data.children(0).children(0) , "x"     , 0 , 1 , 1 , 2 , tree.data.children_ptr(0) );
    TEST_NODE( tree.data.children(1)             , "minus" , 2 , 3 , 2 , 1 , &tree.data );
    TEST_NODE( tree.data.children(1).children(0) , "y"     , 0 , 1 , 1 , 2 , tree.data.children_ptr(1) );
    TEST_NODE( tree.data.children(1).children(1) , "2"     , 0 , 1 , 1 , 2 , tree.data.children_ptr(1) );
}



TEST( tree_tests , swap_one_1 )
{
    test_tree tree;
    tree.data.swap( tree.data2.at(1) );

    TEST_NODE( tree.data                           , "cos" , 1 , 2 , 2 , 0 , nullptr );
    TEST_NODE( tree.data.children(0)               , "y"   , 0 , 1 , 1 , 1 , &tree.data );

    TEST_NODE( tree.data2                          , "minus" , 2 , 8 , 4 , 0 , nullptr );
    TEST_NODE( tree.data2.children(0)              , "plus"  , 2 , 6 , 3 , 1 , &tree.data2 );
    TEST_NODE( tree.data2.children(0).children(0)  , "sin"   , 1 , 2 , 2 , 2 , tree.data2.children_ptr(0) );
    TEST_NODE( tree.data2.children(0).children(1)  , "minus" , 2 , 3 , 2 , 2 , tree.data2.children_ptr(0) );
    TEST_NODE( tree.data2.children(1)              , "x"     , 0 , 1 , 1 , 1 , &tree.data2 );

    TEST_NODE( tree.data2.children(0).children(0).children(0)  , "x" , 0 , 1 , 1 , 3 , tree.data2.children(0).children_ptr(0) );
    TEST_NODE( tree.data2.children(0).children(1).children(0)  , "y" , 0 , 1 , 1 , 3 , tree.data2.children(0).children_ptr(1) );
    TEST_NODE( tree.data2.children(0).children(1).children(1)  , "2" , 0 , 1 , 1 , 3 , tree.data2.children(0).children_ptr(1) );
}

TEST( tree_tests , swap_one_2 )
{
    test_tree tree;
    tree.data2.swap( tree.data.at(3) );

    TEST_NODE( tree.data                         , "plus"  , 2 , 7 , 4 , 0 , nullptr );
    TEST_NODE( tree.data.children(0)             , "sin"   , 1 , 2 , 2 , 1 , &tree.data );
    TEST_NODE( tree.data.children(0).children(0) , "x"     , 0 , 1 , 1 , 2 , tree.data.children_ptr(0) );
    TEST_NODE( tree.data.children(1)             , "minus" , 2 , 4 , 3 , 1 , &tree.data );
    TEST_NODE( tree.data.children(1).children(0) , "cos"   , 1 , 2 , 2 , 2 , tree.data.children_ptr(1) );
    TEST_NODE( tree.data.children(1).children(1) , "x"     , 0 , 1 , 1 , 2 , tree.data.children_ptr(1) );

    TEST_NODE( tree.data.children(1).children(0).children(0) , "y" , 0 , 1 , 1 , 3 , tree.data.children(1).children_ptr(0) );

    TEST_NODE( tree.data2                         , "minus"  , 2 , 3 , 2 , 0 , nullptr );
    TEST_NODE( tree.data2.children(0)             , "y"      , 0 , 1 , 1 , 1 , &tree.data2 );
    TEST_NODE( tree.data2.children(1)             , "2"      , 0 , 1 , 1 , 1 , &tree.data2 );
}


TEST( tree_tests , swap_two_1 )
{
    test_tree tree;
    tree.data.at(1).swap( tree.data2.at(3) );

    TEST_NODE( tree.data                         , "plus"  , 2 , 5 , 3 , 0 , nullptr );
    TEST_NODE( tree.data.children(0)             , "x"     , 0 , 1 , 1 , 1 , &tree.data );
    TEST_NODE( tree.data.children(1)             , "minus" , 2 , 3 , 2 , 1 , &tree.data );
    TEST_NODE( tree.data.children(1).children(0) , "y"     , 0 , 1 , 1 , 2 , tree.data.children_ptr(1) );
    TEST_NODE( tree.data.children(1).children(1) , "2"     , 0 , 1 , 1 , 2 , tree.data.children_ptr(1) );

    TEST_NODE( tree.data2                         , "minus" , 2 , 5 , 3 , 0 , nullptr );
    TEST_NODE( tree.data2.children(0)             , "cos"   , 1 , 2 , 2 , 1 , &tree.data2 );
    TEST_NODE( tree.data2.children(1)             , "sin"   , 1 , 2 , 2 , 1 , &tree.data2 );
    TEST_NODE( tree.data2.children(0).children(0) , "y"     , 0 , 1 , 1 , 2 , tree.data2.children_ptr(0) );
    TEST_NODE( tree.data2.children(1).children(0) , "x"     , 0 , 1 , 1 , 2 , tree.data2.children_ptr(1) );
}

TEST( tree_tests , swap_two_2 )
{
    test_tree tree;
    tree.data.at(3).swap( tree.data2.at(3) );

    TEST_NODE( tree.data                         , "plus"  , 2 , 4 , 3 , 0 , nullptr );
    TEST_NODE( tree.data.children(0)             , "sin"   , 1 , 2 , 2 , 1 , &tree.data );
    TEST_NODE( tree.data.children(1)             , "x"     , 0 , 1 , 1 , 1 , &tree.data );
    TEST_NODE( tree.data.children(0).children(0) , "x"     , 0 , 1 , 1 , 2 , tree.data.children_ptr(0) );

    TEST_NODE( tree.data2                         , "minus" , 2 , 6 , 3 , 0 , nullptr );
    TEST_NODE( tree.data2.children(0)             , "cos"   , 1 , 2 , 2 , 1 , &tree.data2 );
    TEST_NODE( tree.data2.children(1)             , "minus" , 2 , 3 , 2 , 1 , &tree.data2 );
    TEST_NODE( tree.data2.children(0).children(0) , "y"     , 0 , 1 , 1 , 2 , tree.data2.children_ptr(0) );
    TEST_NODE( tree.data2.children(1).children(0) , "y"     , 0 , 1 , 1 , 2 , tree.data2.children_ptr(1) );
    TEST_NODE( tree.data2.children(1).children(1) , "2"     , 0 , 1 , 1 , 2 , tree.data2.children_ptr(1) );
}



TEST( tree_tests , swap_two_3 )
{
    test_tree tree;
    tree.data.at(1).swap( tree.data.at(3) );

    TEST_NODE( tree.data                         , "plus"  , 2 , 6 , 3 , 0 , nullptr );
    TEST_NODE( tree.data.children(0)             , "minus" , 2 , 3 , 2 , 1 , &tree.data );
    TEST_NODE( tree.data.children(1)             , "sin"   , 1 , 2 , 2 , 1 , &tree.data );
    TEST_NODE( tree.data.children(0).children(0) , "y"     , 0 , 1 , 1 , 2 , tree.data.children_ptr(0) );
    TEST_NODE( tree.data.children(0).children(1) , "2"     , 0 , 1 , 1 , 2 , tree.data.children_ptr(0) );
    TEST_NODE( tree.data.children(1).children(0) , "x"     , 0 , 1 , 1 , 2 , tree.data.children_ptr(1) );
}
