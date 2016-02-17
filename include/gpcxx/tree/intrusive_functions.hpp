/*
 * gpcxx/tree/intrusive_functions.hpp
 * Date: 2013-10-15
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_TREE_INTRUSIVE_FUNCTIONS_HPP_INCLUDED
#define GPCXX_TREE_INTRUSIVE_FUNCTIONS_HPP_INCLUDED

#include <gpcxx/util/assert.hpp>

#include <cmath>
#include <cstddef>



namespace gpcxx {

namespace detail {
    
    struct gpcxx_log_impl
    {
        template< typename T >
        T operator()( T v ) const
        {
            T v2 = std::abs( v );
            return ( v2 < 1.0e-20 ) ? std::log( 1.0e-20 ) : std::log( v2 );
        }
    };
    
    static constexpr auto gpcxx_log = gpcxx_log_impl {};

    struct gpcxx_rlog_impl
    {
        template< typename T >
        T operator()( T v ) const
        {
            T v2 = std::abs( v );
            return ( v2 < 1.0e-20 ) ? 0.0 : std::log( v2 );
        }
    };
    
    static constexpr auto gpcxx_rlog = gpcxx_rlog_impl {};
    
    struct unary_minus_impl
    {
        template< typename T >
        T operator()( T v ) const
        {
            return -v;
        }
    };
    
    static constexpr auto unary_minus = unary_minus_impl {};
    
    struct unary_inverse_impl
    {
        template< typename T >
        T operator()( T v ) const
        {
            return 1.0 / v;
        }
    };
    
    static constexpr auto unary_inverse = unary_inverse_impl {};
    
    
    
} // namespace detail


#define UNARY_FUNC( NAME , FUNC )                                                                     \
struct NAME                                                                                           \
{                                                                                                     \
    template< typename Context , typename Node >                                                      \
    inline typename Node::result_type operator()( Context const& c , Node const& node ) const         \
    {                                                                                                 \
        return FUNC( node.child( 0 ).eval( c ) );                                                     \
    }                                                                                                 \
}

#define BINARY_OPERATOR( NAME , OP )                                                                  \
struct NAME                                                                                           \
{                                                                                                     \
    template< typename Context , typename Node >                                                      \
    inline typename Node::result_type operator()( Context const& c , Node const& node ) const         \
    {                                                                                                 \
        return node.child( 0 ).eval( c ) OP node.child( 1 ).eval( c );                                \
    }                                                                                                 \
}

UNARY_FUNC( sin_func , std::sin );
UNARY_FUNC( cos_func , std::cos );
UNARY_FUNC( exp_func , std::exp );
UNARY_FUNC( log_func , detail::gpcxx_log );
UNARY_FUNC( rlog_func , detail::gpcxx_rlog );
UNARY_FUNC( unary_minus_func , detail::unary_minus );
UNARY_FUNC( unary_inverse_func , detail::unary_inverse );

// BINARY_OPERATOR( plus_func , + );
// BINARY_OPERATOR( multiplies_func , * );
BINARY_OPERATOR( minus_func , - );
BINARY_OPERATOR( divides_func , / );


struct plus_func
{
    template< typename Context , typename Node >
    inline typename Node::result_type operator()( Context const& c , Node const& node ) const
    {
        GPCXX_ASSERT( node.size() > 1.0 );
        typename Node::result_type res = node.child( 0 ).eval( c );
        for( size_t i=1 ; i<node.size() ; ++i )
            res += node.child( i ).eval( c );
        return res;
    }
};

struct multiplies_func
{
    template< typename Context , typename Node >
    inline typename Node::result_type operator()( Context const& c , Node const& node ) const
    {
        GPCXX_ASSERT( node.size() > 1.0 );
        typename Node::result_type res = node.child( 0 ).eval( c );
        for( size_t i=1 ; i<node.size() ; ++i )
            res *= node.child( i ).eval( c );
        return res;
    }
};


template< size_t I >
struct array_terminal
{
    template< typename Context , typename Node >
    typename Node::result_type operator()( Context const& c , Node const& node ) const
    {
        typedef typename Node::result_type result_type;
        return static_cast< result_type >( c[I] );
    }
};

template< int V >
struct constant_terminal
{
    template< typename Context , typename Node >
    typename Node::result_type operator()( Context const& c , Node const& node ) const
    {
        typedef typename Node::result_type result_type;
        return static_cast< result_type >( V );
    }
};

template< typename Value = double >
struct double_terminal
{
    Value m_value;
    double_terminal( Value value ) : m_value( value ) { }
    
    template< typename Context , typename Node >
    typename Node::result_type operator()( Context const& c , Node const& node ) const
    {
        typedef typename Node::result_type result_type;        
        return static_cast< result_type >( m_value );
    }
};


} // namespace gpcxx


#endif // GPCXX_TREE_INTRUSIVE_FUNCTIONS_HPP_INCLUDED

struct T;