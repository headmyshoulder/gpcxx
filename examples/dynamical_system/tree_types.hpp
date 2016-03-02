/*
 * gpcxx/examples/dynamical_system/tree_types.hpp
 * Date: 2016-02-26
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_EXAMPLES_DYNAMICAL_SYSTEM_TREE_TYPES_HPP_INCLUDED
#define GPCXX_EXAMPLES_DYNAMICAL_SYSTEM_TREE_TYPES_HPP_INCLUDED

#include "generate_data.hpp"

#include <gpcxx/eval.hpp>
#include <gpcxx/tree.hpp>
#include <gpcxx/intrusive_nodes.hpp>
#include <gpcxx/generate.hpp>
#include <gpcxx/io.hpp>

#include <random>



namespace dynsys {

using context_type = gpcxx::regression_context< double , dim >;
using node_type = gpcxx::intrusive_named_func_node< double , const context_type > ;
using tree_type = gpcxx::intrusive_tree< node_type >;
using individual_type = std::array< tree_type , dim >;

using rng_type = std::mt19937;


inline auto create_node_generator( void )
{
    auto erc_gen = gpcxx::make_intrusive_erc_generator< node_type >( []( auto& rng ) {
        std::normal_distribution<> dist( 0.0 , 1.0 );
        return dist( rng ); } );
    auto terminal_gen = gpcxx::make_uniform_symbol_erc< node_type >(
        std::vector< node_type >{
            node_type { gpcxx::array_terminal< 0 >{}                                 ,      "x" } ,
            node_type { gpcxx::array_terminal< 1 >{}                                 ,      "y" } ,
            node_type { gpcxx::array_terminal< 2 >{}                                 ,      "z" } } ,
            0.5 ,
            erc_gen );
    //]
    
    //[ define_function_set
    //     auto unary_gen = gpcxx::make_uniform_symbol( std::vector< node_type > {
    //         node_type { gpcxx::sin_func {}                                               ,      "s" } ,
    //         node_type { gpcxx::cos_func {}                                               ,      "c" } 
    // //         node_type { gpcxx::exp_func {}                                               ,      "e" } ,
    // //         node_type { gpcxx::log_func {}                                               ,      "l" }
    //     } );
    
    auto binary_gen = gpcxx::make_uniform_symbol( std::vector< node_type > {
        node_type { gpcxx::plus_func {}                                              ,      "+" } ,
        node_type { gpcxx::minus_func {}                                             ,      "-" } ,
        node_type { gpcxx::multiplies_func {}                                        ,      "*" } ,
        node_type { gpcxx::divides_func {}                                           ,      "/" }
    } );
    //]
    
    //[ define_node_generator
    auto node_generator = gpcxx::node_generator< node_type , rng_type , 2 > {
        { 1.0 , 0 , terminal_gen } ,
        //        { 1.0 , 1 , unary_gen } ,
        { 1.0 , 2 , binary_gen } };
        
    return node_generator;
}

template< typename Pop , typename Fitness >
void write_best_individuals( std::ostream &out , const Pop& p , const Fitness &f , size_t num_individuals , bool write_individual = false )
{
    std::vector< size_t > idx;
    gpcxx::sort_indices( f , idx );
    for( size_t i=0 ; i<num_individuals ; ++i )
    {
        if( write_individual )
        {
            out << i << " " << f[ idx[i] ] << " : \n";
            for( auto const& tree : p[ idx[i] ] )
                out << "\t" << gpcxx::simple( tree ) << "\n";
        }
        else
        {
            out << i << " " << f[ idx[i] ] << "\n";
        }
    }
    out << "\n" << std::flush;
}


inline bool is_number( double t )
{
    return ( ! std::isnan( t ) ) && ( ! std::isinf( t ) );
}




} // namespace dynsys


#endif // GPCXX_EXAMPLES_DYNAMICAL_SYSTEM_TREE_TYPES_HPP_INCLUDED
