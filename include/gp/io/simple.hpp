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



// template< class T >
// void pretty_print( const tree< T > &t , std::ostream &out = std::cout )
// {
//     const typename tree< T >::vector_type &data = t.data();
//     if( !t.data().empty() )
//         detail::pretty_print( data , out , size_t ( 0 ) , size_t( 0 ) );
// }




template< class Node >
void print_simple_node( const Node *t , std::ostream &out )
{
    if( t->arity == 0 ) out << t->value;
    else if( t->arity == 1 )
    {
        out << " " << t->value << " ( ";
        print_simple_node( t->children[0] , out );
        out << " ) ";
    }
    else if( t->arity == 2 )
    {
        out << " ( ";
        print_simple_node( t->children[0] , out );
        out << " ) " << t->value << " ( ";
        print_simple_node( t->children[1] , out );
        out << " ) ";
    }

}


template< class Tree >
void print_simple( const Tree& t , std::ostream &out )
{
    const auto *ptr = t.data();
    print_simple_node( ptr , out );
}


} // namespace gp

#endif // SIMPLE_HPP_INCLUDED
