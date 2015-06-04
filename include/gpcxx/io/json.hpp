/*
 * gpcxx/io/json.hpp
 * Date: 2015-05-21
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_IO_JSON_HPP_INCLUDED
#define GPCXX_IO_JSON_HPP_INCLUDED


#include <gpcxx/util/identity.hpp>
#include <gpcxx/util/indent.hpp>

#include <ostream>
#include <sstream>

namespace gpcxx {
    
template< typename Cursor , typename SymbolMapper >
void write_json_cursor( std::ostream& out , Cursor t , std::string const& newline , std::string const& indent_string , size_t indent_i , SymbolMapper const& mapper )
{
    out << indent( indent_i , indent_string ) << "{";
    if( t.valid() )
    {
        out << newline;
        out << indent( indent_i + 1 , indent_string ) << "\"value\" : \"" << mapper( *t ) << "\" ," << newline;
        out << indent( indent_i + 1 , indent_string ) << "\"children\" : [";
        if( !t.empty() ) out << newline;
        for( size_t i=0 ; i<t.size() ; ++i )
        {
            write_json_cursor( out , t.children(i) , newline , indent_string , indent_i + 2 , mapper );
            if( i != t.size() - 1 ) out << " ,";
            out << newline;
        }
        if( !t.empty() ) out << indent( indent_i + 1 , indent_string );
        out << "]" << newline;
        out << indent( indent_i , indent_string );
    }
    out << "}";
}
    
    
template< typename Tree , typename SymbolMapper >
void write_json( std::ostream& out , Tree const& t , std::string const& newline , std::string const& indent , size_t indent_i , SymbolMapper const& mapper )
{
    write_json_cursor( out , t.root() , newline , indent , indent_i , mapper );
}

template< typename Tree , typename SymbolMapper = gpcxx::identity >
std::string json_string( Tree const& t , std::string const& newline = "\n" , std::string const& indent = "  " , SymbolMapper const& mapper = SymbolMapper {} )
{
    std::ostringstream str;
    write_json( str , t , newline , indent , 0 , mapper );
    return str.str();
}


namespace detail {
    
template< typename Tree , typename SymbolMapper >
struct json_writer
{
    Tree const& m_t;
    std::string const& m_newline;
    std::string const& m_indent;
    size_t m_indent_i;
    SymbolMapper const& m_mapper;
    json_writer( Tree const& t , std::string const& newline , std::string const& indent , size_t indent_i , SymbolMapper const& mapper )
    : m_t( t ) , m_newline( newline ) , m_indent( indent ) , m_indent_i( indent_i ) , m_mapper( mapper ) { }

    std::ostream& operator()( std::ostream& out ) const
    {
        write_json( out , m_t , m_newline , m_indent , m_indent_i , m_mapper );
        return out;
    }
};


template< typename T , typename SymbolMapper >
std::ostream& operator<<( std::ostream& out , json_writer< T , SymbolMapper > const& p )
{
    return p( out );
}

} // namespace detail




template< typename T , typename SymbolMapper = gpcxx::identity >
detail::json_writer< T , SymbolMapper > json( T const& t , std::string const& newline = "\n" , std::string const& indent= "  " , size_t indent_i = 0 , SymbolMapper const &mapper = SymbolMapper() )
{
    return detail::json_writer< T , SymbolMapper >( t , newline , indent , indent_i , mapper );
}





} // namespace gpcxx


#endif // GPCXX_IO_JSON_HPP_INCLUDED
