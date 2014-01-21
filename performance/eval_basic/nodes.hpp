/*
 * gpcxx/performance/eval_basic_ecj_cmp/nodes.hpp
 * Date: 2013-09-30
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_PERFORMANCE_EVAL_BASIC_ECJ_CMP_NODES_HPP_INCLUDED
#define GPCXX_PERFORMANCE_EVAL_BASIC_ECJ_CMP_NODES_HPP_INCLUDED

#include <functional>


    

template< typename T , typename Context >
using i_node = std::function< T( Context const& ) >;


template< size_t I , typename T , typename Context >
class terminal_node
{
public:
    
    T operator()( Context const& context ) const { return context[I]; }
};

template< typename T >
inline T my_log( T v )
{
    return ( std::abs( v ) < 1.0e-20 ) ? 0.0 : std::log( std::abs( v ) );
}


#define UNARY_NODE( NAME , FUNC )                                                                                 \
template< typename T , typename Context >                                                                         \
struct NAME                                                                                                       \
{                                                                                                                 \
    i_node< T , Context > m_child;                                                                                \
    NAME( i_node< T , Context > child ) : m_child( std::move( child ) ) { }                                       \
    T operator()( Context const &context ) const { return FUNC( m_child( context ) ); }                           \
}


#define BINARY_NODE( NAME , FUNC )                                                                                \
template< typename T , typename Context >                                                                         \
struct NAME                                                                                                       \
{                                                                                                                 \
    i_node< T , Context > m_left , m_right;                                                                       \
    NAME( i_node< T , Context > left , i_node< T , Context > right )                                              \
    : m_left( std::move( left ) ) , m_right( std::move( right ) ) { }                                             \
    T operator()( Context const & context ) const { return m_left( context ) FUNC m_right( context ); } \
}


UNARY_NODE( sin_node , std::sin );
UNARY_NODE( cos_node , std::cos );
UNARY_NODE( exp_node , std::exp );
UNARY_NODE( log_node , my_log );

BINARY_NODE( plus_node , + );
BINARY_NODE( minus_node , - );
BINARY_NODE( multiplies_node , * );
BINARY_NODE( divides_node , / );







#endif // GPCXX_PERFORMANCE_EVAL_BASIC_ECJ_CMP_NODES_HPP_INCLUDED
