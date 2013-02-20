/*
 * test_tree.hpp
 * Date: 2013-02-20
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef TEST_TREE_HPP_INCLUDED
#define TEST_TREE_HPP_INCLUDED

#include <gp/tree/linked_node_tree.hpp>
#include <gp/tree/complete_linked_tree_structure.hpp>

#include <string>

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

        data2.set_data(
            new node_type (
                "minus" ,
                new node_type( "cos" , new node_type( "y" ) ) ,
                new node_type( "x" )
                ) ) ;

        gp::complete_linked_tree_structure( data );
        gp::complete_linked_tree_structure( data2 );
    }

    ~test_tree( void ) { }

    tree_type data;
    tree_type data2;
};


#endif // TEST_TREE_HPP_INCLUDED
