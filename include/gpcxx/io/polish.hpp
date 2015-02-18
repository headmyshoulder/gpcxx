/*
 * polish.hpp
 * Date: 2013-02-03
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef GPCXX_IO_POLISH_HPP_INCLUDED
#define GPCXX_IO_POLISH_HPP_INCLUDED

#include <gpcxx/util/identity.hpp>
#include <gpcxx/io/detail/read_polish.hpp>

#include <boost/algorithm/string/erase.hpp>
#include <boost/algorithm/string/find_iterator.hpp>
#include <boost/algorithm/string/finder.hpp>

#include <ostream>


namespace gpcxx {

    
template< typename Cursor , typename SymbolMapper >
void print_polish_cursor( Cursor t , std::ostream &out , std::string const& sep , std::string const &opening , std::string const& closing , SymbolMapper const& mapper )
{
    if( t.size() != 0 ) out << opening;
    out << mapper( *t );
    if( t.size() != 0 )
    {
        out << sep;
        print_polish_cursor( t.children( 0 ) , out , sep , opening , closing , mapper );
    }
    for( size_t i=1 ; i<t.size(); ++i )
    {
        out << sep;
        print_polish_cursor( t.children( i ) , out , sep , opening , closing , mapper );
    }
    if( t.size() != 0 ) out << closing;
}


template< typename Tree , typename SymbolMapper >
void print_polish( Tree const& t , std::ostream &out , std::string const& sep , std::string const &opening , std::string const& closing , SymbolMapper const& mapper )
{
    print_polish_cursor( t.root() , out , sep , opening , closing , mapper );
}


template< typename Tree , typename SymbolMapper >
struct polish_printer
{
    Tree const& m_t;
    std::string const &m_sep;
    std::string const &m_opening;
    std::string const &m_closing;
    SymbolMapper const& m_mapper;
    polish_printer( Tree const& t , std::string const& sep , std::string const &opening , std::string const& closing , SymbolMapper const& mapper )
    : m_t( t ) , m_sep( sep ) , m_opening( opening ) , m_closing( closing ) , m_mapper( mapper ) { }

    std::ostream& operator()( std::ostream& out ) const
    {
        print_polish( m_t , out , m_sep , m_opening , m_closing , m_mapper );
        return out;
    }
};


template< typename T , typename SymbolMapper = gpcxx::identity >
polish_printer< T , SymbolMapper > polish( T const& t , std::string const& sep = "|" , std::string const &opening = "" , std::string const& closing = "" , SymbolMapper const &mapper = SymbolMapper() )
{
    return polish_printer< T , SymbolMapper >( t , sep , opening , closing , mapper );
}


template< typename T , typename SymbolMapper >
std::ostream& operator<<( std::ostream& out , polish_printer< T , SymbolMapper > const& p )
{
    return p( out );
}


template< typename Tree , typename NodeMapper >
void read_polish( std::string str , Tree &tree , NodeMapper const& mapper , std::string const& sep = "|" , std::string const &opening = "" , std::string const& closing = "" )
{
    using iterator_t = boost::split_iterator< std::string::const_iterator >;
    
    if( opening != "" ) boost::algorithm::erase_all( str , opening );
    if( closing != "" ) boost::algorithm::erase_all( str , closing );
    
    iterator_t first = iterator_t( str , boost::first_finder( sep , boost::is_iequal() ) );
    
    detail::read_polish_impl( first , tree , tree.root() , mapper );
}
    

}


#endif // GPCXX_IO_POLISH_HPP_INCLUDED
