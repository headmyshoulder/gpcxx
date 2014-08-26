/*
 * gpcxx/performance/eval_basic/std_function_nodes.hpp
 * Date: 2014-08-26
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_PERFORMANCE_EVAL_BASIC_STD_FUNCTION_NODES_HPP_INCLUDED
#define GPCXX_PERFORMANCE_EVAL_BASIC_STD_FUNCTION_NODES_HPP_INCLUDED

#include <gpcxx/tree/basic_tree.hpp>

#include <array>
#include <functional>


namespace functions
{
    template< typename T >
    inline T gpcxx_log( T v )
    {
        T v2 = std::abs( v );
        return ( v2 < 1.0e-20 ) ? std::log( 1.0e-20 ) : std::log( v2 );
    }



#define UNARY_FUNC( NAME , FUNC )                                                                     \
struct NAME                                                                                           \
{                                                                                                     \
    template< typename Context , typename Node >                                                      \
    inline typename Node::result_type operator()( Context const& c , Node const& node ) const         \
    {                                                                                                 \
        return FUNC( node.children( 0 )->eval( c ) );                                                 \
    }                                                                                                 \
}

#define BINARY_OPERATOR( NAME , OP )                                                                  \
struct NAME                                                                                           \
{                                                                                                     \
    template< typename Context , typename Node >                                                      \
    inline typename Node::result_type operator()( Context const& c , Node const& node ) const         \
    {                                                                                                 \
        return node.children( 0 )->eval( c ) OP node.children( 1 )->eval( c );                        \
    }                                                                                                 \
}

UNARY_FUNC( sin_func , std::sin );
UNARY_FUNC( cos_func , std::cos );
UNARY_FUNC( exp_func , std::exp );
UNARY_FUNC( log_func , detail::gpcxx_log );

BINARY_OPERATOR( plus_func , + );
BINARY_OPERATOR( minus_func , - );
BINARY_OPERATOR( multiplies_func , * );
BINARY_OPERATOR( divides_func , / );




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



namespace gpcxx {

struct func_node;
    
using value_type = double;
using context_type = std::array< value_type , 3 >;
using tree_type = basic_tree< func_node >;
using func_type = tree_type::cursor;

struct func_node
{
    using func_type = std::function< value_type( context_type const& , cursor ) >;
    
    func_node( func_type f ) : m_f( std::move( f ) )
    
    value_type eval( context_type const&context , cursor c ) const
    {
        return m_f( context , c );
    }
    
    func_type m_f;
};

struct transform
{
    func_node operator()( char t ) const
    {
        switch ( t )
        {
            case 'x':
                return func_node {  
                break;
            case 'y':
        }
    }
}


} // namespace gpcxx


#endif // GPCXX_PERFORMANCE_EVAL_BASIC_STD_FUNCTION_NODES_HPP_INCLUDED
