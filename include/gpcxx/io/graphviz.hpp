/*
 * graphviz.hpp
 * Date: 2013-02-03
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef GRAPHVIZ_HPP_INCLUDED
#define GRAPHVIZ_HPP_INCLUDED

#include <gpcxx/util/identity.hpp>

#include <fstream>



namespace gpcxx {


namespace detail {

template< typename Cursor , typename SymbolMapper >
void print_graphviz_cursor( Cursor const &n , std::ostream &out , size_t &count , SymbolMapper const& mapper , bool print_node_info )
{
    size_t cc = count;
    out << "NODE" << cc << " [ label = \"" << mapper( *n );
    if( print_node_info )
    {
        out << "(" << n.height() << " " << n.level() << ")";
    }
    out << "\" ]\n";
        
    for( size_t i=0 ; i<n.size() ; ++i )
    {
        ++count;
        out << "NODE" << cc << " -> " << "NODE" << count << "\n";
        print_graphviz_cursor( n.children( i ) , out , count , mapper , print_node_info );
    }
}

} // namespace detail



template< typename Tree , typename SymbolMapper >
void print_graphviz( Tree const& t , std::ostream &out , SymbolMapper const& mapper , bool print_node_info )
{
    out << "digraph G\n";
    out << "{\n";
    if( !t.empty() )
    {
        size_t count = 0;
        detail::print_graphviz_cursor( t.root() , out , count , mapper , print_node_info );
    }
    out << "}\n";
}


template< typename Tree , typename SymbolMapper >
struct graphviz_printer
{
    Tree const& m_t;
    SymbolMapper const& m_mapper;
    bool m_print_node_info;
    
    graphviz_printer( Tree const& t , SymbolMapper const& mapper , bool print_node_info )
    : m_t( t ) , m_mapper( mapper ) , m_print_node_info( print_node_info ) { }
    
    std::ostream& operator()( std::ostream& out ) const
    {
        print_graphviz( m_t , out , m_mapper , m_print_node_info );
        return out;
    }
};

template< typename T , typename SymbolMapper = gpcxx::identity >
graphviz_printer< T , SymbolMapper > graphviz( T const& t , bool print_node_info = false , SymbolMapper const &mapper = SymbolMapper() )
{
    return graphviz_printer< T , SymbolMapper >( t , mapper , print_node_info );
}


template< typename T , typename SymbolMapper >
std::ostream& operator<<( std::ostream& out , graphviz_printer< T , SymbolMapper > const& p )
{
    return p( out );
}



template< typename Tree , typename SymbolMapper = gpcxx::identity >
void generate_graphviz_pdf( const Tree &t , const std::string &filename , SymbolMapper const& mapper = SymbolMapper() )
{
    std::ofstream fout( "__tmp__.dot" );
    print_graphviz( t , fout , mapper );
    fout.close();

    system ( "dot -Tps2 __tmp__.dot > __tmp__.ps" );
    system ( ( std::string( "ps2pdf __tmp__.ps " ) + filename ).c_str() );
    system ( "rm __tmp__.dot" );
    system ( "rm __tmp__.ps" );
}




} // namespace gpcxx

#endif // GRAPHVIZ_HPP_INCLUDED
