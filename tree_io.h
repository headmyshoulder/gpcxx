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

template< class Vec >
void pretty_print( const Vec &data , std::ostream &out , size_t index , size_t level )
{
    const typename Vec::value_type &n = data[index];
    out << indent( level ) << n.value << "\n";
    if( n.arity > 0 )
        pretty_print( data , out , index + 1 , level + 1 );
    for( size_t i=1 ; i<n.arity ; ++i )
        pretty_print( data , out , n.child_index[i-1] , level + 1 );
}


template< class Vec >
void print_graph_entry( const Vec &data , std::ostream &out , size_t index )
{
    const typename Vec::value_type &n = data[index];
    out << "NODE" << index << " [label = \"" << n.value 
//        << "(" << index << "-" << n.level << "-" << n.length << "-" << n.subtree_height << "-" << n.parent_index << ")"
        << " " << index 
        << "\"]\n";
    if( n.arity > 0 )
    {
        out << "NODE" << index << " -> " << "NODE" << index + 1 << "\n";
        print_graph_entry( data , out , index + 1 );
    }
    for( size_t i=1 ; i<n.arity ; ++i )
    {
        out << "NODE" << index << " -> " << "NODE" << n.child_index[i-1] << "\n";
        print_graph_entry( data , out , n.child_index[i-1] );
    }
}


} // namespace detail



template< class T >
void pretty_print( const tree< T > &t , std::ostream &out = std::cout )
{
    const typename tree< T >::vector_type &data = t.data();
    if( !t.data().empty() )
        detail::pretty_print( data , out , size_t ( 0 ) , size_t( 0 ) );
}


template< class T >
void print_graph( const tree< T > &t , const std::string &filename )
{
    const typename tree< T >::vector_type &data = t.data();
    std::ofstream fout( "__tmp__.dot" );
    fout << "digraph G\n{\n";
    if( !t.data().empty() )
        detail::print_graph_entry( data , fout , size_t( 0 ) );
    fout << "}\n";
    fout.close();

    system ( "dot -Tps2 __tmp__.dot > __tmp__.ps" );
    system ( ( std::string( "ps2pdf __tmp__.ps " ) + filename ).c_str() );
    // system ( "rm __tmp__.dot" );
    // system ( "rm __tmp__.ps" );
}


template< class T >
void print_details( const tree< T > &t , std::ostream &out = std::cout )
{
    const auto &data = t.data();
    size_t count = 0;
    for( const auto &n : data )
    {
        out << count << " " << n.value << " " << n.arity;
        for( size_t i=1 ; i<n.arity ; ++i ) out << " " << n.child_index[i-1];
        out << "\n";
        ++count;
    }
}

#endif // TREE_IO_H_INCLUDED
