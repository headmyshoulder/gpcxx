/*
 * gpcxx/app/primitive_sets/koza_intrusive.hpp
 * Date: 2015-06-24
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_APP_PRIMITIVE_SETS_KOZA_INTRUSIVE_HPP_INCLUDED
#define GPCXX_APP_PRIMITIVE_SETS_KOZA_INTRUSIVE_HPP_INCLUDED


#include <gpcxx/generate/uniform_symbol.hpp>
#include <gpcxx/generate/uniform_symbol_erc.hpp>
#include <gpcxx/generate/node_generator.hpp>
#include <gpcxx/tree/intrusive_erc_generator.hpp>
#include <gpcxx/tree/intrusive_functions.hpp>



namespace gpcxx {

    
namespace koza_intrusive_detail {
    
template< typename Node , size_t NumTerminals >
struct koza_variable_set_generator;

template< typename Node >
struct koza_variable_set_generator< Node , 1 >
{
    static constexpr auto generate( void )
    {
        return std::vector< Node > {
            Node { gpcxx::array_terminal< 0 > {} ,    "x" } };
    }
};

template< typename Node >
struct koza_variable_set_generator< Node , 2 >
{
    static constexpr auto generate( void )
    {
        return std::vector< Node > {
            Node { gpcxx::array_terminal< 0 > {} ,    "x" } ,
            Node { gpcxx::array_terminal< 1 > {} ,    "y" }
        };
    }
};

template< typename Node >
struct koza_variable_set_generator< Node , 3 >
{
    static constexpr auto generate( void )
    {
        return std::vector< Node > {
            Node { gpcxx::array_terminal< 0 > {} ,    "x" } ,
            Node { gpcxx::array_terminal< 1 > {} ,    "y" } ,
            Node { gpcxx::array_terminal< 2 > {} ,    "z" }
        };
    }
};


template< typename Node , size_t NumTerminals >
constexpr auto koza_terminal_set( std::true_type )
{
    auto erc_gen = gpcxx::make_intrusive_erc_generator< Node >( []( auto& rng ) {
        std::uniform_real_distribution<> dist( -1.0 , 1.0 );
        return dist( rng ); } );
    return gpcxx::make_uniform_symbol_erc< Node >(
        koza_variable_set_generator< Node , NumTerminals >::generate() ,
        0.25 ,
        erc_gen );
}

template< typename Node , size_t NumTerminals >
constexpr auto koza_terminal_set( std::false_type )
{
    return gpcxx::make_uniform_symbol( koza_variable_set_generator< Node , NumTerminals >::generate() );
}

} // namespace koza_intrusive_detail

template< typename Node , typename Rng , size_t NumTerminals , bool Erc >
constexpr auto koza_intrusive_primitve_set( void )
{
    auto terminal_gen = koza_intrusive_detail::koza_terminal_set< Node , NumTerminals >( std::integral_constant< bool, Erc >{} );

    auto unary_gen = gpcxx::make_uniform_symbol( std::vector< Node > {
        Node { gpcxx::sin_func {}                                               ,      "sin" } ,
        Node { gpcxx::cos_func {}                                               ,      "cos" } ,
        Node { gpcxx::exp_func {}                                               ,      "exp" } ,
        Node { gpcxx::log_func {}                                               ,      "log" }
    } );
    auto binary_gen = gpcxx::make_uniform_symbol( std::vector< Node > {
        Node { gpcxx::plus_func {}                                              ,      "+" } ,
        Node { gpcxx::minus_func {}                                             ,      "-" } ,
        Node { gpcxx::multiplies_func {}                                        ,      "*" } ,
        Node { gpcxx::divides_func {}                                           ,      "/" }
    } );

    auto node_generator = gpcxx::node_generator< Node , Rng , 3 > {
        { 1.0 , 0 , terminal_gen } ,
        { 1.0 , 1 , unary_gen } ,
        { 1.0 , 2 , binary_gen } };
        
    return node_generator;
}



} // namespace gpcxx


#endif // GPCXX_APP_PRIMITIVE_SETS_KOZA_INTRUSIVE_HPP_INCLUDED
