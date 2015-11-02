/*
 * gpcxx/app/primitive_sets/koza_algebraic.hpp
 * Date: 2015-09-24
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_APP_PRIMITIVE_SETS_KOZA_ALGEBRAIC_HPP_INCLUDED
#define GPCXX_APP_PRIMITIVE_SETS_KOZA_ALGEBRAIC_HPP_INCLUDED

#include <gpcxx/generate/uniform_symbol.hpp>
#include <gpcxx/generate/uniform_symbol_erc.hpp>
#include <gpcxx/generate/node_generator.hpp>
#include <gpcxx/tree/intrusive_erc_generator.hpp>
#include <gpcxx/tree/intrusive_functions.hpp>




namespace gpcxx {
    
namespace koza_algebraic_detail {
    
template< typename Node , size_t NumTerminals >
struct koza_variable_set_generator;

template< typename Node >
struct koza_variable_set_generator< Node , 1 >
{
    static constexpr auto generate( void )
    {
        return std::vector< Node > {
            Node::make_variable_terminal( gpcxx::array_terminal< 0 > {} ,    "x" ) };
    }
};

template< typename Node >
struct koza_variable_set_generator< Node , 2 >
{
    static constexpr auto generate( void )
    {
        return std::vector< Node > {
            Node::make_variable_terminal( gpcxx::array_terminal< 0 > {} ,    "x" ) ,
            Node::make_variable_terminal( gpcxx::array_terminal< 1 > {} ,    "y" )
        };
    }
};

template< typename Node >
struct koza_variable_set_generator< Node , 3 >
{
    static constexpr auto generate( void )
    {
        return std::vector< Node > {
            Node::make_variable_terminal( gpcxx::array_terminal< 0 > {} ,    "x" ) ,
            Node::make_variable_terminal( gpcxx::array_terminal< 1 > {} ,    "y" ) ,
            Node::make_variable_terminal( gpcxx::array_terminal< 2 > {} ,    "z" )
        };
    }
};

template< typename Node , size_t NumTerminals >
constexpr auto koza_terminal_set( std::true_type )
{
    auto erc_gen = gpcxx::make_intrusive_erc_factory_generator(
        []( auto f , auto name ) { return Node::make_constant_terminal( f , name ); } ,
        []( auto& rng ) {
            std::uniform_real_distribution<> dist( -1.0 , 1.0 );
            return dist( rng ); } );
    return gpcxx::make_uniform_symbol_erc< Node >(
        koza_variable_set_generator< Node , NumTerminals >::generate() ,
        0.05 ,
        erc_gen );
}

template< typename Node , size_t NumTerminals >
constexpr auto koza_terminal_set( std::false_type )
{
    return gpcxx::make_uniform_symbol( koza_variable_set_generator< Node , NumTerminals >::generate() );
}


} // namespace koza_algebraic_detail


template< typename Node , typename Rng , size_t NumTerminals , bool Erc >
constexpr auto koza_algebraic_primitve_set( void )
{
    auto terminal_gen = koza_algebraic_detail::koza_terminal_set< Node , NumTerminals >( std::integral_constant< bool, Erc >{} );

    auto unary_gen = gpcxx::make_uniform_symbol( std::vector< Node > {
        Node::make_unary_operation(       gpcxx::sin_func {} ,             "sin" ) ,
        Node::make_unary_operation(       gpcxx::cos_func {} ,             "cos" ) ,
        Node::make_unary_operation(       gpcxx::exp_func {} ,             "exp" ) ,
        Node::make_unary_operation(       gpcxx::log_func {} ,             "log" )
    } );
    auto binary_gen = gpcxx::make_uniform_symbol( std::vector< Node > {
        Node::make_binary_operation(      gpcxx::plus_func {} ,            "+" ) ,
        Node::make_binary_operation(      gpcxx::minus_func {} ,           "-" ) ,
        Node::make_binary_operation(      gpcxx::multiplies_func {}  ,     "*" ) ,
        Node::make_binary_operation(      gpcxx::divides_func {} ,         "/" )
    } );

    auto node_generator = gpcxx::node_generator< Node , Rng , 3 > {
        { 1.0 , 0 , terminal_gen } ,
        { 1.0 , 1 , unary_gen } ,
        { 1.0 , 2 , binary_gen }
    };
        
    return node_generator;
}



} // namespace gpcxx


#endif // GPCXX_APP_PRIMITIVE_SETS_KOZA_ALGEBRAIC_HPP_INCLUDED
