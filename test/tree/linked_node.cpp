/*
 * linked_node.cpp
 * Date: 2013-02-20
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

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
