/*
 * polish.hpp
 * Date: 2013-02-03
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef POLISH_HPP_INCLUDED
#define POLISH_HPP_INCLUDED

#include <ostream>


namespace gp {


    template< class Node >
    void print_polish_node( const Node *t , std::ostream &out )
    {
         if( t->arity == 0 )
         {
             out << t->value;
         }
         else
         {
             out << "(" << t->value;
             for( size_t i=0 ; i<t->arity ; ++i )
             {
                 out << " ";
                 print_polish_node( t->children[i] , out );
             }
             out << ")";
         }
    }


    template< class Tree >
    void print_polish( const Tree& t , std::ostream &out )
    {
        const auto *ptr = t.data();
        print_polish_node( ptr , out );
    }


    // ToDo: schick machen
    template< class Tree >
    struct polish_printer
    {
        const Tree &m_t;
        polish_printer( const Tree &t ) : m_t( t ) { }
        std::ostream& operator()( std::ostream& out ) const
        {
            print_polish( m_t , out );
            return out;
        }
    };

    template< class T >
    polish_printer< T > polish( const T &t ) { return polish_printer< T >( t ); }

    template< class T >
    std::ostream& operator<<( std::ostream& out , const polish_printer< T > &p )
    {
        return p( out );
    }


}


#endif // POLISH_HPP_INCLUDED
