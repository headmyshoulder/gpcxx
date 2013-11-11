/*
 * gpcxx/util/simple.hpp
 * Date: 2013-02-03
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef GPCXX_UTIL_SIMPLE_HPP_INCLUDED
#define GPCXX_UTIL_SIMPLE_HPP_INCLUDED


#include <gpcxx/util/identity.hpp>

#include <ostream>


namespace gpcxx {


template< typename Cursor , typename SymbolMapper >
void print_simple_cursor( Cursor t , std::ostream &out , SymbolMapper const& mapper )
{
    if( t.size() == 0 ) out << mapper( *t );
    else if( t.size() == 1 )
    {
        Cursor child = t.children( 0 );
        out << mapper( *t ) << "( ";
        print_simple_cursor( child , out , mapper );
        out << " )";
    }
    else if( t.size() == 2 )
    {
        Cursor left = t.children( 0 );
        Cursor right = t.children( 1 );

        if( left.size() == 2 ) out << "( ";
        print_simple_cursor( left , out , mapper );
        if( left.size() == 2 ) out << " )";

        out << " " << mapper( *t ) << " ";
        
        if( right.size() == 2 ) out << "( ";
        print_simple_cursor( right , out , mapper );
        if( right.size() == 2 ) out << " )";
    }

}


template< typename Tree , typename SymbolMapper >
void print_simple( Tree const& t , std::ostream &out , SymbolMapper const& mapper )
{
    print_simple_cursor( t.root() , out , mapper );
}


template< typename Tree , typename SymbolMapper >
struct simple_printer
{
    Tree const& m_t;
    SymbolMapper const& m_mapper;
    simple_printer( Tree const& t , SymbolMapper const& mapper ) : m_t( t ) , m_mapper( mapper ) { }
    std::ostream& operator()( std::ostream& out ) const
    {
        print_simple( m_t , out , m_mapper );
        return out;
    }
};

template< typename T , typename SymbolMapper = gpcxx::identity >
simple_printer< T , SymbolMapper > simple( T const& t , SymbolMapper const &mapper = SymbolMapper() ) { return simple_printer< T , SymbolMapper >( t , mapper ); }


template< typename T , typename SymbolMapper >
std::ostream& operator<<( std::ostream& out , simple_printer< T , SymbolMapper > const& p )
{
    return p( out );
}




} // namespace gpcxx

#endif // GPCXX_UTIL_SIMPLE_HPP_INCLUDED
