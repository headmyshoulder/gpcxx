/*
 * simple.hpp
 * Date: 2013-02-03
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef SIMPLE_HPP_INCLUDED
#define SIMPLE_HPP_INCLUDED

#include <ostream>

namespace gp {

namespace detail {

inline std::string indent( size_t level )
{
    std::string ret = "";
    for( size_t i=0 ; i<level ; ++i ) ret += "  ";
    return ret;    
}



} // namespace detail




template< class Node >
void print_simple_node( const Node &t , std::ostream &out )
{
    if( t.arity() == 0 ) out << t.value();
    else if( t.arity() == 1 )
    {
        out << t.value() << "( ";
        print_simple_node( t.children( 0 ) , out );
        out << " )";
    }
    else if( t.arity() == 2 )
    {
        if( t.children( 0 ).arity() == 2 ) out << "( ";
        print_simple_node( t.children( 0 ) , out );
        if( t.children( 0 ).arity() == 2 ) out << " )";

        out << " " << t.value() << " ";

        if( t.children( 1 ).arity() == 2 ) out << "( ";
        print_simple_node( t.children( 1 ) , out );
        if( t.children( 1 ).arity() == 2 ) out << " )";
    }

}


template< class Tree >
void print_simple( const Tree& t , std::ostream &out )
{
    print_simple_node( t , out );
}


// ToDo: schick machen
template< class Tree >
struct simple_printer
{
    const Tree &m_t;
    simple_printer( const Tree &t ) : m_t( t ) { }
    std::ostream& operator()( std::ostream& out ) const
    {
        print_simple( m_t , out );
        return out;
    }
};

template< class T >
simple_printer< T > simple( const T &t ) { return simple_printer< T >( t ); }

template< class T >
std::ostream& operator<<( std::ostream& out , const simple_printer< T > &p )
{
    return p( out );
}




} // namespace gp

#endif // SIMPLE_HPP_INCLUDED
