/*
 * gpcxx/io/bracket.hpp
 * Date: 2015-08-18
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_IO_BRACKET_HPP_INCLUDED
#define GPCXX_IO_BRACKET_HPP_INCLUDED

#include <gpcxx/util/identity.hpp>
#include <gpcxx/io/detail/read_bracket.hpp>

#include <ostream>
#include <string>
#include <sstream>


namespace gpcxx {

template< typename Cursor , typename SymbolMapper >
void write_bracket_cursor( std::ostream &out , Cursor t , std::string const &opening , std::string const& closing , SymbolMapper const& mapper )
{
    out << opening;
    out << mapper( *t );
    if( t.size() != 0 )
    {
        write_bracket_cursor( out , t.children( 0 ) , opening , closing , mapper );
    }
    for( size_t i=1 ; i<t.size(); ++i )
    {
        write_bracket_cursor( out , t.children( i ) , opening , closing , mapper );
    }
    out << closing;
}


template< typename Tree , typename SymbolMapper >
void write_bracket( std::ostream &out , Tree const& t , std::string const &opening , std::string const& closing , SymbolMapper const& mapper )
{
    if( !t.empty() )
        write_bracket_cursor( out , t.root() , opening , closing , mapper );
}

template< typename Tree , typename SymbolMapper = gpcxx::identity >
std::string bracket_string( Tree const& t , std::string const &opening = "{" , std::string const& closing = "}" , SymbolMapper const &mapper = SymbolMapper() )
{
    std::ostringstream str;
    write_bracket( str , t , opening , closing , mapper );
    return str.str();
}



namespace detail {
    
template< typename Tree , typename SymbolMapper >
struct bracket_writer
{
    Tree const& m_t;
    std::string const &m_opening;
    std::string const &m_closing;
    SymbolMapper const& m_mapper;
    bracket_writer( Tree const& t , std::string const &opening , std::string const& closing , SymbolMapper const& mapper )
    : m_t( t ) , m_opening( opening ) , m_closing( closing ) , m_mapper( mapper ) { }

    std::ostream& operator()( std::ostream& out ) const
    {
        write_bracket( out , m_t , m_opening , m_closing , m_mapper );
        return out;
    }
};


template< typename T , typename SymbolMapper >
std::ostream& operator<<( std::ostream& out , bracket_writer< T , SymbolMapper > const& p )
{
    return p( out );
}

} // namespace detail




template< typename T , typename SymbolMapper = gpcxx::identity >
detail::bracket_writer< T , SymbolMapper > bracket( T const& t , std::string const &opening = "{" , std::string const& closing = "}" , SymbolMapper const &mapper = SymbolMapper() )
{
    return detail::bracket_writer< T , SymbolMapper >( t , opening , closing , mapper );
}





template< typename Rng , typename Tree , typename NodeMapper = gpcxx::identity >
typename boost::range_iterator< Rng const >::type read_bracket( Rng const& rng , Tree &tree , NodeMapper const& mapper = NodeMapper {} , std::string const &opening = "{" , std::string const& closing = "}" )
{
    using iterator = typename boost::range_iterator< Rng >::type;
    if( ! boost::empty( rng ) )
    {
        auto irng = boost::make_iterator_range( boost::begin( rng ) , boost::end( rng ) );
        return detail::read_bracket( irng , tree , mapper , opening , closing );
    }
    return iterator {};
}



} // namespace gpcxx


#endif // GPCXX_IO_BRACKET_HPP_INCLUDED
