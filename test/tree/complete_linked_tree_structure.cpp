/*
 * complete_linked_tree_structure.cpp
 * Date: 2013-02-20
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include <gp/tree/complete_linked_tree_structure.hpp>

#include <gtest/gtest.h>

using namespace std;

struct test_tree
{
    typedef gp::linked_node_tree< std::string > tree_type;
    typedef tree_type::node_type node_type;

    test_tree( void )
    {
        data.set_data(
            new node_type (
                "plus" , 
                new node_type( "sin" ,
                               new node_type( "x" ) ) ,
                new node_type( "minus" ,
                               new node_type( "y" ) ,
                               new node_type( "2" ) 
                    )
                ) );
    }

    ~test_tree( void ) { }

    tree_type data;
};

TEST( tree_tests , complete_linked_tree_structure1 )
{
    test_tree tree;
    gp::complete_linked_tree_structure( tree.data );

    auto &root = tree.data.root();
    EXPECT_EQ( root.value , "plus" );
    EXPECT_EQ( root.num_elements , 6 );
    EXPECT_EQ( root.height , 3 );
    EXPECT_EQ( root.level , 0 );
    EXPECT_EQ( root.arity , 2 );

    auto c1 = root.children[0];
    EXPECT_EQ( c1->value , "sin" );
    EXPECT_EQ( c1->num_elements , 2 );
    EXPECT_EQ( c1->height , 2 );
    EXPECT_EQ( c1->level , 1 );
    EXPECT_EQ( c1->arity , 1 );

    auto c2 = root.children[1];
    EXPECT_EQ( c2->value , "minus" );
    EXPECT_EQ( c2->num_elements , 3 );
    EXPECT_EQ( c2->height , 2 );
    EXPECT_EQ( c2->level , 1 );
    EXPECT_EQ( c2->arity , 2 );

    auto c3 = c1->children[0];
    EXPECT_EQ( c3->value , "x" );
    EXPECT_EQ( c3->num_elements , 1 );
    EXPECT_EQ( c3->height , 1 );
    EXPECT_EQ( c3->level , 2 );
    EXPECT_EQ( c3->arity , 0 );

    auto c4 = c2->children[0];
    EXPECT_EQ( c4->value , "y" );
    EXPECT_EQ( c4->num_elements , 1 );
    EXPECT_EQ( c4->height , 1 );
    EXPECT_EQ( c4->level , 2 );
    EXPECT_EQ( c4->arity , 0 );

    auto c5 = c2->children[1];
    EXPECT_EQ( c5->value , "2" );
    EXPECT_EQ( c5->num_elements , 1 );
    EXPECT_EQ( c5->height , 1 );
    EXPECT_EQ( c5->level , 2 );
    EXPECT_EQ( c5->arity , 0 );
}

