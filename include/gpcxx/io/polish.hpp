/*
 * polish.hpp
 * Date: 2013-02-03
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef POLISH_HPP_INCLUDED
#define POLISH_HPP_INCLUDED

#include <gpcxx/util/identity.hpp>

#include <ostream>


namespace gpcxx {

template< typename Cursor , typename SymbolMapper >
void print_polish_cursor( Cursor t , std::ostream &out , std::string const& sep , SymbolMapper const& mapper )
{
    out << mapper( *t );
    if( t.size() != 0 )
    {
        out << sep;
        print_polish_cursor( t.children( 0 ) , out , sep , mapper );
    }
    for( size_t i=1 ; i<t.size(); ++i )
    {
        out << sep;
        print_polish_cursor( t.children( i ) , out , sep , mapper );
    }
        
}


template< typename Tree , typename SymbolMapper >
void print_polish( Tree const& t , std::ostream &out , std::string const& sep , SymbolMapper const& mapper )
{
    print_polish_cursor( t.root() , out , sep , mapper );
}


template< typename Tree , typename SymbolMapper >
struct polish_printer
{
    Tree const& m_t;
    std::string const& m_sep;
    SymbolMapper const& m_mapper;
    polish_printer( Tree const& t , std::string const& sep , SymbolMapper const& mapper )
    : m_t( t ) , m_sep( sep ) , m_mapper( mapper ) { }
    std::ostream& operator()( std::ostream& out ) const
    {
        print_polish( m_t , out , m_sep , m_mapper );
        return out;
    }
};

template< typename T , typename SymbolMapper = gpcxx::identity >
polish_printer< T , SymbolMapper > polish( T const& t , std::string const& sep = "|" , SymbolMapper const &mapper = SymbolMapper() )
{
    return polish_printer< T , SymbolMapper >( t , sep , mapper );
}


template< typename T , typename SymbolMapper >
std::ostream& operator<<( std::ostream& out , polish_printer< T , SymbolMapper > const& p )
{
    return p( out );
}




    
    

}


#endif // POLISH_HPP_INCLUDED
