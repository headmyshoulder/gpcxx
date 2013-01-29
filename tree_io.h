/*
 * tree_io.h
 * Date: 2013-01-25
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef TREE_IO_H_INCLUDED
#define TREE_IO_H_INCLUDED

#include <fstream>
#include <iostream>


namespace detail {

inline std::string indent( size_t level )
{
    std::string ret = "";
    for( size_t i=0 ; i<level ; ++i ) ret += "  ";
    return ret;    
}

// template< class Vec >
// void pretty_print( const Vec &data , std::ostream &out , size_t index , size_t level )
// {
//     const typename Vec::value_type &n = data[index];
//     out << indent( level ) << n.value << "\n";
//     if( n.arity > 0 )
//         pretty_print( data , out , index + 1 , level + 1 );
//     for( size_t i=1 ; i<n.arity ; ++i )
//         pretty_print( data , out , n.child_index[i-1] , level + 1 );
// }


    template< class Node >
    void print_graph_node( const Node *n , std::ostream &out , size_t &count )
    {
        size_t cc = count;
        out << "NODE" << cc << " [ label = \"" << n->value 
            << "(" << n->num_elements << " " << n->height << " " << n->level << ")"
            << "\" ]\n";
        
        for( size_t i=0 ; i<n->arity ; ++i )
        {
            ++count;
            out << "NODE" << cc << " -> " << "NODE" << count << "\n";
            print_graph_node( n->children[i] , out , count );
        }
    }


} // namespace detail



// template< class T >
// void pretty_print( const tree< T > &t , std::ostream &out = std::cout )
// {
//     const typename tree< T >::vector_type &data = t.data();
//     if( !t.data().empty() )
//         detail::pretty_print( data , out , size_t ( 0 ) , size_t( 0 ) );
// }


template< class T >
void print_graph( const tree< T > &t , const std::string &filename )
{
    std::ofstream fout( "__tmp__.dot" );
    fout << "digraph G\n{\n";
    if( t.m_data != 0 )
    {
        size_t count = 0;
        detail::print_graph_node( t.m_data , fout , count );
    }
    fout << "}\n";
    fout.close();

    system ( "dot -Tps2 __tmp__.dot > __tmp__.ps" );
    system ( ( std::string( "ps2pdf __tmp__.ps " ) + filename ).c_str() );
    system ( "rm __tmp__.dot" );
    system ( "rm __tmp__.ps" );
}


template< class T >
void print_formula( const tree_node< T > *t , std::ostream &out )
{
    if( t->arity == 0 ) out << t->value;
    else if( t->arity == 1 )
    {
        out << " " << t->value << " ( ";
        print_formula( t->children[0] , out );
        out << " ) ";
    }
    else if( t->arity == 2 )
    {
        out << " ( ";
        print_formula( t->children[0] , out );
        out << " ) " << t->value << " ( ";
        print_formula( t->children[1] , out );
        out << " ) ";
    }

}


template< class T >
void print_formula( const tree< T >& t , std::ostream &out )
{
    print_formula( t.m_data , out );
}



#endif // TREE_IO_H_INCLUDED
