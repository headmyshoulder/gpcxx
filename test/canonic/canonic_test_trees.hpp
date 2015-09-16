/*
 * gpcxx/test/canonic/canonic_test_trees.hpp
 * Date: 2015-09-08
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_TEST_CANONIC_CANONIC_TEST_TREES_HPP_INCLUDED
#define GPCXX_TEST_CANONIC_CANONIC_TEST_TREES_HPP_INCLUDED

#include <gpcxx/canonic/algebraic_node.hpp>
#include <gpcxx/tree/intrusive_tree.hpp>
#include <gpcxx/tree/intrusive_functions.hpp>


struct canonic_test_trees
{
    using node_type = gpcxx::algebraic_node< double , std::array< double , 2 > const >;
    using tree_type = gpcxx::intrusive_tree< node_type >;
    
    static tree_type test_tree1( void )
    {
        tree_type tree;
        auto root = tree.insert_below( tree.root() , node_type::make_binary_operation( gpcxx::plus_func {} , "+" ) );
        /* auto n1 = */ tree.insert_below( root , node_type::make_variable_terminal( gpcxx::array_terminal<1> {} , "y" ) );
        /* auto n1 = */ tree.insert_below( root , node_type::make_variable_terminal( gpcxx::array_terminal<0> {} , "x" ) );
        return tree;
    }
    
    static tree_type test_tree2( void )
    {
        tree_type tree = test_tree1();
        /* auto n1 = */ tree.insert_below( tree.root() , node_type::make_constant_terminal( gpcxx::double_terminal<> { 1.5 } , "1.5" ) );
        /* auto n2 = */ tree.insert_below( tree.root() , node_type::make_variable_terminal( gpcxx::array_terminal<2> {} , "z" ) );
        return tree;
    }
    
    static tree_type test_tree3( void )
    {
        tree_type tree;
        auto root = tree.insert_below( tree.root() , node_type::make_binary_operation( gpcxx::plus_func {} , "+" ) );
        auto n1 = tree.insert_below( root , node_type::make_binary_operation( gpcxx::plus_func {} , "+" ) );
        /* auto n2 = */ tree.insert_below( root , node_type::make_variable_terminal( gpcxx::array_terminal<0> {} , "x" ) );
        /* auto n3 = */ tree.insert_below( n1 , node_type::make_variable_terminal( gpcxx::array_terminal<0> {} , "z" ) );
        /* auto n4 = */ tree.insert_below( n1 , node_type::make_variable_terminal( gpcxx::array_terminal<0> {} , "y" ) );
        return tree;
    }


//     static tree_type test_tree1( void )
//     {
//         tree_type tree;
//         auto root = tree.insert_below( tree.root() , node_type( "inv" , false , false , false , 10 ) );
//         auto n1 = tree.insert_below( root , node_type( "inv" , false , false , false , 10 ) );
//         auto n2 = tree.insert_below( n1 , node_type( "x" , false , false , false , 20 ) );
//         return tree;
//     }
// 
//     static tree_type test_tree2( void )
//     {
//         tree_type tree;
//         auto root = tree.insert_below( tree.root() , node_type( "sin" , false , false , false , 11 ) );
//         auto n1 = tree.insert_below( root , node_type( "inv" , false , false , false , 10 ) );
//         auto n2 = tree.insert_below( n1 , node_type( "inv" , false , false , false , 10 ) );
//         auto n3 = tree.insert_below( n2 , node_type( "x" , false , false , false , 20 ) );
//         return tree;
//     }
// 
//     static tree_type test_tree3( void )
//     {
//         tree_type tree;
//         auto root = tree.insert_below( tree.root() , node_type { "*" , true , true , false , 0 } );
//         auto n1 = tree.insert_below( root , node_type { "x" , false , false , false , 20 } );
//         auto n2 = tree.insert_below( root , node_type( "inv" , false , false , false , 10 ) );
//         auto n3 = tree.insert_below( n2 , node_type( "x" , false , false , false , 20 ) );
//         return tree;    
//     }
// 
//     static tree_type test_tree4( void )
//     {
//         tree_type tree;
//         auto root = tree.insert_below( tree.root() , node_type( "sin" , false , false , false , 11 ) );
//         auto n1 = tree.insert_below( root , node_type { "*" , true , true , false , 0 } );
//         auto n2 = tree.insert_below( n1 , node_type { "x" , false , false , false , 20 } );
//         auto n3 = tree.insert_below( n1 , node_type( "inv" , false , false , false , 10 ) );
//         auto n4 = tree.insert_below( n3 , node_type( "x" , false , false , false , 20 ) );
//         return tree;
//     }
    
};



#endif // GPCXX_TEST_CANONIC_CANONIC_TEST_TREES_HPP_INCLUDED
