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

#include <gpcxx/util.hpp>

#include <iostream>
#include <sstream>

int main( int argc , char** argv )
{
    const static size_t dim = 1;
    using result_type = double;
    using context_type = std::array< double , dim >;
    using node_type = gpcxx::algebraic_node< result_type , context_type const >;
    using tree_type = gpcxx::intrusive_tree< node_type >;
    
    tree_type t;
    GPCXX_UNUSED_VAR( t );
    
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
    
    using rule = std::function< gpcxx::rule_result( tree_type& , tree_type::cursor ) >;
    using rule_container = std::vector< rule >;

    auto constant_factory = []( double t ) {
        std::ostringstream str;
        str << t;
        return node_type::make_constant_terminal( gpcxx::double_terminal< double >( t ) , str.str() ); };

    rule_container rules {
          gpcxx::make_sort_commutative( algebras )
        , gpcxx::make_left_lift( algebras )
        , gpcxx::make_remove_double_inverse_functions( algebras )
        , gpcxx::make_remove_inverse_operations( algebras )
        , gpcxx::make_summarize_constant( constant_factory )
        };
    
    return 0;
}

