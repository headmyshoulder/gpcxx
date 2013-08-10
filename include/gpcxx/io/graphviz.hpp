/*
 * graphviz.hpp
 * Date: 2013-02-03
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef GRAPHVIZ_HPP_INCLUDED
#define GRAPHVIZ_HPP_INCLUDED


/**************************************************
 **************************************************
 *
 * NOT TESTED. WILL NOT WORK RIGHT NOW!!!
 *
 **************************************************
 **************************************************/

namespace gpcxx {

namespace detail {

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



} // namespace gpcxx

#endif // GRAPHVIZ_HPP_INCLUDED
