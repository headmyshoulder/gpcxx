/*
 * gpcxx/performance/eval_basic_ecj_cmp/intrusive_nodes.hpp
 * Date: 2013-10-11
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_PERFORMANCE_EVAL_BASIC_ECJ_CMP_INTRUSIVE_NODES_HPP_INCLUDED
#define GPCXX_PERFORMANCE_EVAL_BASIC_ECJ_CMP_INTRUSIVE_NODES_HPP_INCLUDED

#include <gpcxx/tree/intrusive_tree.hpp>


using context_type = std::array< double , 3 >;

struct my_node : public gpcxx::intrusive_node< my_node >
{
    typedef std::function< double( context_type const& , my_node const& ) > func_type;
    
    my_node( func_type f )
    : m_func( std::move( f ) ) { }
    
    double eval( context_type const & context ) const
    {
        return m_func( context , *this );
    }
    
    func_type m_func;
};

#define UNARY_FUNC( NAME , FUNC )                                                                     \
struct NAME                                                                                           \
{                                                                                                     \
    double operator()( context_type const& c , my_node const& node ) const                            \
    {                                                                                                 \
        return FUNC( node.children( 0 )->eval( c ) );                                                 \
    }                                                                                                 \
}

#define BINARY_FUNC( NAME , FUNC )                                                                    \
struct NAME                                                                                           \
{                                                                                                     \
    double operator()( context_type const& c , my_node const& node ) const                            \
    {                                                                                                 \
        return node.children( 0 )->eval( c ) FUNC + node.children( 1 )->eval( c );                    \
    }                                                                                                 \
}

template< typename T >
inline T my_log( T v )
{
    double v2 = std::abs( v );
    return ( v2 < 1.0e-20 ) ? std::log( 1.0e-20 ) : std::log( v2 );
//     return std::log( std::log( max( 1.0e-20 , abs( v ) ) ) );
}


UNARY_FUNC( sin_func , sin );
UNARY_FUNC( cos_func , cos );
UNARY_FUNC( exp_func , exp );
UNARY_FUNC( log_func , my_log );

BINARY_FUNC( plus_func , + );
BINARY_FUNC( minus_func , - );
BINARY_FUNC( multiplies_func , * );
BINARY_FUNC( divides_func , / );


template< size_t I >
struct terminal
{
    double operator()( context_type const& c , my_node const& node ) const
    {
        return c[I];
    }
};

using terminal_x = terminal< 0 >;
using terminal_y = terminal< 1 >;
using terminal_z = terminal< 2 >;






#endif // GPCXX_PERFORMANCE_EVAL_BASIC_ECJ_CMP_INTRUSIVE_NODES_HPP_INCLUDED
