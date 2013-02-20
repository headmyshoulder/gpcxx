/*
 * test_tree.hpp
 * Date: 2013-02-20
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef TEST_TREE_HPP_INCLUDED
#define TEST_TREE_HPP_INCLUDED

#include <gp/tree/linked_node_tree.hpp>

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
    }

    ~test_tree( void ) { }

    tree_type data;
};


#endif // TEST_TREE_HPP_INCLUDED
