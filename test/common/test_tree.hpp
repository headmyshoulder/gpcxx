/*
 * test_tree.hpp
 * Date: 2013-02-20
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef TEST_TREE_HPP_INCLUDED
#define TEST_TREE_HPP_INCLUDED

#include <gp/tree/linked_node.hpp>

#include <string>

struct test_tree
{
    typedef gp::linked_node< std::string > node_type;


    test_tree( void )
        : data() , data2()
    {
        {
            data.value() = "plus";
            auto i1 = data.emplace( "sin" );
            i1->emplace( "x" );
            auto i3 = data.emplace( "minus" );
            i3->emplace( "y" );
            i3->emplace( "2" );
        }

        {
            data2.value() = "minus";
            auto i1 = data2.emplace( "cos" );
            i1->emplace( "y" );
            data2.emplace( "x" );
        }
    }

    ~test_tree( void ) { }

    node_type data;
    node_type data2;
};


#endif // TEST_TREE_HPP_INCLUDED
