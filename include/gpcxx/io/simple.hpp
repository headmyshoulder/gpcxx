/*
 * gpcxx/util/simple.hpp
 * Date: 2013-02-03
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef GPCXX_IO_SIMPLE_HPP_INCLUDED
#define GPCXX_IO_SIMPLE_HPP_INCLUDED


#include <gpcxx/util/identity.hpp>

#include <ostream>
#include <sstream>


namespace gpcxx {


template< typename Cursor , typename SymbolMapper >
void write_simple_cursor( std::ostream &out , Cursor t , bool write_infix , SymbolMapper const& mapper )
{
    if( ( t.size() == 2 ) && write_infix )
    {
        Cursor left = t.children( 0 );
        Cursor right = t.children( 1 );
        
        if( left.size() == 2 ) out << "( ";
        write_simple_cursor( out , left , write_infix , mapper );
        if( left.size() == 2 ) out << " )";

        out << " " << mapper( *t ) << " ";
    
        if( right.size() == 2 ) out << "( ";
        write_simple_cursor( out , right , write_infix , mapper );
        if( right.size() == 2 ) out << " )";
    }
    else
    {
        out << mapper( *t );
        if( t.size() > 0 )
        {
            out << "( ";
            for( size_t i=0 ; i<t.size() ; ++i )
            {
                if( i != 0 ) out << " , ";
                write_simple_cursor( out , t.children(i) , write_infix , mapper );
            }
            out << " )";
        }
    }

}


template< typename Tree , typename SymbolMapper >
void write_simple( std::ostream &out , Tree const& t , bool write_infix , SymbolMapper const& mapper )
{
    if( !t.empty() )
        write_simple_cursor( out , t.root() , write_infix , mapper );
}

template< typename Tree , typename SymbolMapper = gpcxx::identity >
std::string simple_string( Tree const& t , bool write_infix = true , SymbolMapper const &mapper = SymbolMapper() )
{
    std::ostringstream str;
    write_simple( str , t , write_infix , mapper );
    return str.str();
}



namespace detail {


template< typename Tree , typename SymbolMapper >
struct simple_writer
{
    Tree const& m_t;
    bool m_write_infix;
    SymbolMapper const& m_mapper;
    simple_writer( Tree const& t , bool write_infix , SymbolMapper const& mapper )
    : m_t( t ) , m_write_infix( write_infix ) , m_mapper( mapper ) { }
    std::ostream& operator()( std::ostream& out ) const
    {
        write_simple( out , m_t , m_write_infix , m_mapper );
        return out;
    }
};

template< typename T , typename SymbolMapper >
std::ostream& operator<<( std::ostream& out , simple_writer< T , SymbolMapper > const& p )
{
    return p( out );
}


} // namespace detail



template< typename T , typename SymbolMapper = gpcxx::identity >
detail::simple_writer< T , SymbolMapper > simple( T const& t , bool write_infix = true , SymbolMapper const &mapper = SymbolMapper() )
{
    return detail::simple_writer< T , SymbolMapper >( t , write_infix , mapper );
}







} // namespace gpcxx

#endif // GPCXX_IO_SIMPLE_HPP_INCLUDED
