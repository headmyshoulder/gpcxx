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

#include <gpcxx/io/polish.hpp>

#include <utility>
#include <unordered_map>
#include <functional>

struct canonic_test_trees
{
    using node_type = gpcxx::algebraic_node< double , std::array< double , 2 > const >;
    using tree_type = gpcxx::intrusive_tree< node_type >;
    using intrusive_mapped_func = std::pair< size_t , std::function< node_type() > >;
    using intrusive_mapper_map_type = std::unordered_map< std::string , intrusive_mapped_func >;
    using intrusive_mapper_type = std::function< intrusive_mapped_func( std::string const& ) > ;


    static intrusive_mapper_type get_mapper( void )
    {
        auto mapper_map = intrusive_mapper_map_type {
            std::make_pair( "+2" , std::make_pair( 2 , []() { return node_type::make_binary_operation( gpcxx::plus_func {} , "+" ); } ) ) ,
            std::make_pair( "+3" , std::make_pair( 3 , []() { return node_type::make_binary_operation( gpcxx::plus_func {} , "+" ); } ) ) ,
            std::make_pair( "-" , std::make_pair( 2 , []() { return node_type::make_binary_operation( gpcxx::minus_func {} , "-" ); } ) ) ,
            std::make_pair( "*2" , std::make_pair( 2 , []() { return node_type::make_binary_operation( gpcxx::multiplies_func {} , "*" ) ; } ) ) ,
            std::make_pair( "*3" , std::make_pair( 3 , []() { return node_type::make_binary_operation( gpcxx::multiplies_func {} , "*" ); } ) ) ,
            std::make_pair( "/" , std::make_pair( 2 , []() { return node_type::make_binary_operation( gpcxx::divides_func {} , "/" ) ; } ) ) ,
            std::make_pair( "cos" , std::make_pair( 1 , []() { return node_type::make_unary_operation( gpcxx::cos_func {} , "cos" ) ; } ) ) ,
            std::make_pair( "sin" , std::make_pair( 1 , []() { return node_type::make_unary_operation( gpcxx::sin_func {} , "sin" )  ; } ) ) ,
            std::make_pair( "x" , std::make_pair( 0 , []() { return node_type::make_variable_terminal( gpcxx::array_terminal< 0 >{} , "x" ) ; } ) ) ,
            std::make_pair( "y" , std::make_pair( 0 , []() { return node_type::make_variable_terminal( gpcxx::array_terminal< 1 >{} , "y" ) ; } ) )
        };
            
        auto mapper = [mapper_map]( std::string const& str ) -> intrusive_mapped_func {
            auto iter = mapper_map.find( str );
            if( iter != mapper_map.end() )
                return iter->second;
            else
                return std::make_pair( size_t( 0 ) , [str]() {
                    return node_type::make_constant_terminal( gpcxx::double_terminal<>{ std::stod( str ) } , str ); } );
        };
        
        return mapper;
    }
    
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
    
    
    static tree_type test_tree4( void )
    {
        tree_type tree;
        auto root = tree.insert_below( tree.root() , node_type::make_binary_operation( gpcxx::plus_func {} , "-" ) );
        /* auto n1 = */ tree.insert_below( root , node_type::make_variable_terminal( gpcxx::array_terminal<0> {} , "x" ) );
        /* auto n2 = */ tree.insert_below( root , node_type::make_variable_terminal( gpcxx::array_terminal<0> {} , "y" ) );
        return tree;
    }

    static tree_type test_tree5( void )
    {
        tree_type tree;
        auto root =      tree.insert_below( tree.root() , node_type::make_identity_operation( gpcxx::unary_minus_func {} , "um" ) );
        auto n1 =        tree.insert_below( root , node_type::make_identity_operation( gpcxx::unary_minus_func {} , "um" ) );
        /* auto n2 = */  tree.insert_below( n1 , node_type::make_variable_terminal( gpcxx::array_terminal<0> {} , "x" ) );
        return tree;
    }
    
    static tree_type test_tree6( void )
    {
        tree_type tree;
        auto root =      tree.insert_below( tree.root() , node_type::make_unary_operation( gpcxx::sin_func {} , "sin" ) );
        auto n1 =        tree.insert_below( root , node_type::make_identity_operation( gpcxx::unary_minus_func {} , "um" ) );
        auto n2 =        tree.insert_below( n1 , node_type::make_identity_operation( gpcxx::unary_minus_func {} , "um" ) );
        /* auto n3 = */  tree.insert_below( n2 , node_type::make_variable_terminal( gpcxx::array_terminal<0> {} , "x" ) );
        return tree;
    }

    static tree_type test_tree7( void )
    {
        tree_type tree;
        auto root =      tree.insert_below( tree.root() , node_type::make_unary_operation( gpcxx::sin_func {} , "sin" ) );
        /* auto n1 = */  tree.insert_below( root , node_type::make_constant_terminal( gpcxx::double_terminal< double > { 1.0 } , "1.0" ) );
        return tree;
    }
    
    static tree_type test_tree8( void )
    {
        tree_type tree;
        auto root =      tree.insert_below( tree.root() , node_type::make_binary_operation( gpcxx::plus_func {} , "+" ) );
        /* auto n1 = */  tree.insert_below( root , node_type::make_variable_terminal( gpcxx::array_terminal<0> {} , "x" ) );
        auto n2 =        tree.insert_below( root , node_type::make_unary_operation( gpcxx::sin_func {} , "sin" ) );
        /* auto n3 = */  tree.insert_below( n2 , node_type::make_constant_terminal( gpcxx::double_terminal< double > { 1.0 } , "1.0" ) );
        return tree;
    }
    
    static tree_type test_tree9( void )
    {
        tree_type tree;
        auto root =      tree.insert_below( tree.root() , node_type::make_binary_operation( gpcxx::plus_func {} , "+" ) );
        /* auto n1 = */  tree.insert_below( root , node_type::make_constant_terminal( gpcxx::double_terminal< double > { 1.0 } , "1.0" ) );
        auto n2 =        tree.insert_below( root , node_type::make_unary_operation( gpcxx::sin_func {} , "sin" ) );
        /* auto n3 = */  tree.insert_below( n2 , node_type::make_constant_terminal( gpcxx::double_terminal< double > { 1.0 } , "1.0" ) );
        return tree;
    }


    static tree_type test_tree10( void )
    {
        auto mapper = get_mapper();
        tree_type tree;
        gpcxx::read_polish( "+3|1.0|2.0|3.0" , tree , mapper );
        return tree;
    }
    
    static tree_type test_tree11( void )
    {
        // order
        // lift
        // summarize
        // inverse functions
        auto mapper = get_mapper();
        tree_type tree;
        gpcxx::read_polish( "+3|y|x|*2|2|*2|4|8" , tree , mapper );
        return tree;
    }
    
    static tree_type test_tree12( void )
    {
        // order
        // lift
        // summarize
        // inverse functions
        auto mapper = get_mapper();
        tree_type tree;
        gpcxx::read_polish( "+3|y|x|*2|2|*2|x|8" , tree , mapper );
        return tree;
    }
    
    static tree_type test_tree13( void )
    {
        auto mapper = get_mapper();
        tree_type tree;
        gpcxx::read_polish( "+2|y|+2|2|*2|x|*2|4|8" , tree , mapper );
        return tree;
    }

    
};



#endif // GPCXX_TEST_CANONIC_CANONIC_TEST_TREES_HPP_INCLUDED
