/*
 * simple.hpp
 * Date: 2013-02-03
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef SIMPLE_HPP_INCLUDED
#define SIMPLE_HPP_INCLUDED

#include <ostream>


namespace gp {


template< typename Cursor >
void print_simple_cursor( Cursor t , std::ostream &out )
{
    if( t.size() == 0 ) out << *t;
    else if( t.size() == 1 )
    {
        Cursor child = t.begin();
        out << *t << "( ";
        print_simple_cursor( child , out );
        out << " )";
    }
    else if( t.size() == 2 )
    {
        Cursor left = t.begin();
        Cursor right = left + 1;

        if( left.size() == 2 ) out << "( ";
        print_simple_cursor( left , out );
        if( left.size() == 2 ) out << " )";

        out << " " << *t << " ";
        
        if( right.size() == 2 ) out << "( ";
        print_simple_cursor( right , out );
        if( right.size() == 2 ) out << " )";
    }

}


template< typename Tree >
void print_simple( Tree const& t , std::ostream &out )
{
    print_simple_cursor( t.root() , out );
}


// ToDo: schick machen
template< class Tree >
struct simple_printer
{
    Tree const& m_t;
    simple_printer( Tree const& t ) : m_t( t ) { }
    std::ostream& operator()( std::ostream& out ) const
    {
        print_simple( m_t , out );
        return out;
    }
};

template< class T >
simple_printer< T > simple( T const& t ) { return simple_printer< T >( t ); }

template< class T >
std::ostream& operator<<( std::ostream& out , simple_printer< T > const& p )
{
    return p( out );
}




} // namespace gp

#endif // SIMPLE_HPP_INCLUDED
