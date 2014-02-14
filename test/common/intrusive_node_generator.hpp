/*
 * gpcxx/test/common/intrusive_node_generator.hpp
 * Date: 2013-10-15
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_TEST_COMMON_INTRUSIVE_NODE_GENERATOR_HPP_INCLUDED
#define GPCXX_TEST_COMMON_INTRUSIVE_NODE_GENERATOR_HPP_INCLUDED

#include <gpcxx/tree/intrusive_func.hpp>

#include <stdexcept>

using namespace gpcxx;


template< typename Node >
struct intrusive_node_generator
{
    using node_type = Node;
    
    node_type operator()( std::string const& s ) const
    {
        if( s == "plus" ) return node_type( plus_func() , "plus" );
        if( s == "minus" ) return node_type( minus_func() , "minus" );
        if( s == "multiplies" ) return node_type( minus_func() , "multiplies" );
        if( s == "+" ) return node_type( plus_func() , "+" );
        if( s == "-" ) return node_type( minus_func() , "-" );
        if( s == "*" ) return node_type( multiplies_func() , "*");
        if( s == "/" ) return node_type( divides_func() , "/");
        if( s == "x" ) return node_type( array_terminal< 0 >() , "x" );
        if( s == "y" ) return node_type( array_terminal< 1 >() , "y" );
        if( s == "z" ) return node_type( array_terminal< 2 >() , "z" );
        if( s == "sin" ) return node_type( sin_func() , "sin" );
        if( s == "cos" ) return node_type( cos_func() , "cos" );
        if( s == "exp" ) return node_type( exp_func() , "exp" );
        if( s == "log" ) return node_type( log_func() , "log" );
        
        if( s == "1" ) return node_type( constant_terminal< 1 >() , "1" );
        if( s == "2" ) return node_type( constant_terminal< 2 >() , "2" );
        if( s == "3" ) return node_type( constant_terminal< 3 >() , "3" );
        if( s == "4" ) return node_type( constant_terminal< 4 >() , "4" );
        if( s == "5" ) return node_type( constant_terminal< 5 >() , "5" );
        if( s == "6" ) return node_type( constant_terminal< 6 >() , "6" );
        if( s == "7" ) return node_type( constant_terminal< 7 >() , "7" );
        if( s == "8" ) return node_type( constant_terminal< 8 >() , "8" );
        if( s == "9" ) return node_type( constant_terminal< 9 >() , "9" );
        if( s == "10" ) return node_type( constant_terminal< 10 >() , "10" );
        if( s == "11" ) return node_type( constant_terminal< 11 >() , "11" );
        if( s == "12" ) return node_type( constant_terminal< 12 >() , "12" );
        if( s == "13" ) return node_type( constant_terminal< 13 >() , "13" );
        if( s == "14" ) return node_type( constant_terminal< 14 >() , "14" );
        if( s == "15" ) return node_type( constant_terminal< 15 >() , "15" );
        if( s == "16" ) return node_type( constant_terminal< 16 >() , "16" );
        if( s == "17" ) return node_type( constant_terminal< 17 >() , "17" );
        if( s == "18" ) return node_type( constant_terminal< 18 >() , "18" );
        if( s == "19" ) return node_type( constant_terminal< 19 >() , "19" );
        
        try
        {
            return node_type( double_terminal<>( std::stod( s ) ) , s );
        }
        catch( std::exception &e )
        {
        }
        
        
        throw std::runtime_error( std::string( "No node for " ) + s + " found!" );
    }
};


#endif // GPCXX_TEST_COMMON_INTRUSIVE_NODE_GENERATOR_HPP_INCLUDED
