/*
 * tree_algebra.cpp
 * Date: 2015-10-08
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/tree.hpp>
#include <gpcxx/canonic.hpp>
#include <gpcxx/io.hpp>

#include <iostream>
#include <sstream>
#include <unordered_map>



int main( int argc , char** argv )
{
    //[ algebraic_node
    const static size_t dim = 1;
    using result_type = double;
    using context_type = std::array< double , dim >;
    using node_type = gpcxx::algebraic_node< result_type , context_type const >;
    using tree_type = gpcxx::intrusive_tree< node_type >;
    //]
    
    
    //[ simplication_mapper_detail
    using intrusive_mapped_func = std::pair< size_t , std::function< node_type() > >;
    using intrusive_mapper_map_type = std::unordered_map< std::string , intrusive_mapped_func >;
    // using intrusive_mapper_type = std::function< intrusive_mapped_func( std::string const& ) > ;
    
    auto mapper_map = intrusive_mapper_map_type {
        std::make_pair( "+" , std::make_pair( 2 , []() { return node_type::make_binary_operation( gpcxx::plus_func {} , "+" ); } ) ) ,
        std::make_pair( "-" , std::make_pair( 2 , []() { return node_type::make_binary_operation( gpcxx::minus_func {} , "-" ); } ) ) ,
        std::make_pair( "*" , std::make_pair( 2 , []() { return node_type::make_binary_operation( gpcxx::multiplies_func {} , "*" ) ; } ) ) ,
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
    //]


    
    //[ algebras_definition
    using algebras_type = gpcxx::algebras< node_type >;
    using group_type = algebras_type::group_type;
    
    algebras_type algebras;
    algebras.add_abelian_group( group_type {
        node_type::make_binary_operation( gpcxx::plus_func {} , "+" ) ,
        node_type::make_constant_terminal( gpcxx::double_terminal<> { 0.0 } , "0" ) ,
        node_type::make_binary_operation( gpcxx::minus_func {} , "-" ) ,
        node_type::make_identity_operation( gpcxx::unary_minus_func {} , "um" )
        } );
    algebras.add_abelian_group( group_type {
        node_type::make_binary_operation( gpcxx::multiplies_func {} , "*" ) ,
        node_type::make_constant_terminal( gpcxx::double_terminal<> { 1.0 } , "1" ) ,
        node_type::make_binary_operation( gpcxx::divides_func {} , "/" ) ,
        node_type::make_identity_operation( gpcxx::unary_inverse_func {} , "inv" ) 
        } );
    //]
    
    //[ simplification_definitions_rule_container
    using rule = std::function< gpcxx::rule_result( tree_type& , tree_type::cursor ) >;
    using rule_container = std::vector< rule >;
    //]
    
    //[ simplification_rules_definitions
    auto constant_factory = []( double t ) {
        std::ostringstream str;
        str << t;
        return node_type::make_constant_terminal( gpcxx::double_terminal< double >( t ) , str.str() ); };

    rule_container rules {
          gpcxx::make_order_commutative( algebras )
        , gpcxx::make_left_lift( algebras )
        , gpcxx::make_remove_double_inverse_functions( algebras )
        , gpcxx::make_remove_inverse_operations( algebras )
        , gpcxx::make_summarize_constant( constant_factory )
        };
    //]
    
    
    auto str2expr = [&mapper]( std::string const& s ) {
            tree_type expr;
            gpcxx::read_polish( s , expr , mapper );
            return expr;
    };
        
    {
        auto get_expression = [str2expr]( void ) { return str2expr( "+|y|+|2|*|x|*|4|8" ); };
        std::cout << "Example1" << std::endl;
        //[ simplification_example1
        tree_type expr = get_expression();
        std::cout << gpcxx::simple( expr ) << std::endl;
        gpcxx::transform_tree( rules , expr );
        std::cout << gpcxx::simple( expr ) << std::endl;
        //]
        std::cout << std::endl;
    }
    
    {
        auto get_expression1 = [str2expr]( void ) { return str2expr( "+|y|sin|+|y|x" ); };
        auto get_expression2 = [str2expr]( void ) { return str2expr( "+|sin|+|x|y|y" ); };
        std::cout << "Example1" << std::endl;
        //[ simplification_example2
        tree_type expr1 = get_expression1();
        tree_type expr2 = get_expression2();
        std::cout << gpcxx::simple( expr1 ) << "    <->    " << gpcxx::simple( expr2 ) << std::endl;
        gpcxx::transform_tree( rules , expr1 );
        gpcxx::transform_tree( rules , expr2 );
        std::cout << gpcxx::simple( expr1 ) << "    <->    " << gpcxx::simple( expr2 ) << std::endl;
        //]
        std::cout << std::endl;
    }

    

    
    
    return 0;
}

