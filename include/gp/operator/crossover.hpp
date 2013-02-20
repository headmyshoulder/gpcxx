/*
 * crossover.h
 * Date: 2013-01-26
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef CROSSOVER_H_INCLUDED
#define CROSSOVER_H_INCLUDED

#include <random>
#include <stdexcept>
#include <algorithm>



namespace gp {


namespace detail {

    template< class Tree >
    void crossover_impl( Tree &v1 , Tree &v2 , typename Tree::node_type *n1 , typename Tree::node_type *n2 )
    {
        typedef typename Tree::node_type node_type;
        node_type *p1 = n1->parent;
        node_type *p2 = n2->parent;

        if( ( p1 == 0 ) && ( p2 == 0 ) )
        {
            std::swap( v1.m_data , v2.m_data );
        }
        else if( ( p1 == 0 ) )
        {
            node_type *tmp = v1.m_data;
            v1.m_data = n2;
            for( size_t i=0 ; i<p2->arity ; ++i )
            {
                if( p2->children[i] == n2 )
                {
                    p2->children[i] = tmp;
                    break;
                }
            }
            n1->parent = p2;
            n2->parent = 0;
        }
        else if( p2 == 0 )
        {
            node_type *tmp = v2.m_data;
            v2.m_data = n1;
            for( size_t i=0 ; i<p1->arity ; ++i )
            {
                if( p1->children[i] == n1 )
                {
                    p1->children[i] = tmp;
                    break;
                }
            }
            n2->parent = p1;
            n1->parent = 0;
        }
        else
        {
            for( size_t i=0 ; i<p1->arity ; ++i )
                if( p1->children[i] == n1 ) { p1->children[i] = n2; n2->parent = p1; break; }
            for( size_t i=0 ; i<p2->arity ; ++i )
                if( p2->children[i] == n2 ) { p2->children[i] = n1; n1->parent = p2; break; }
        }
    }

} // namespace detail;


template< class Tree >
void crossover( Tree &v1 , Tree &v2 , size_t i1 , size_t i2 )
{
    typedef typename Tree::node_type node_type;
    node_type *n1 = find_node_to_index( v1.m_data , i1 );
    node_type *n2 = find_node_to_index( v2.m_data , i2 );
    if( ( n1 == 0 ) || ( n2 == 0 ) ) return;
    detail::crossover_impl( v1 , v2 , n1 , n2 );
    complete_tree_structure( v1 );
    complete_tree_structure( v2 );
}

template< class Tree , class Rng >
void crossover( Tree &t1 , Tree &t2 , Rng &rng , size_t max_height )
{
    typedef typename Tree::node_type node_type;

    if( t1.m_data == 0 ) return;
    if( t2.m_data == 0 ) return;

    std::uniform_int_distribution< size_t > dist1( 0 , t1.m_data->num_elements - 1 );
    std::uniform_int_distribution< size_t > dist2( 0 , t2.m_data->num_elements - 1 );

    

    bool good = true;
    size_t iter = 0;
    node_type *n1 = 0 , *n2 = 0;
    do
    {
        size_t i1 = dist1( rng );
        size_t i2 = dist2( rng );

        n1 = find_node_to_index( t1.m_data , i1 );
        n2 = find_node_to_index( t2.m_data , i2 );

        size_t nh1 = n1->level + n2->height - 1;
        size_t nh2 = n2->level + n1->height - 1;
        good = ( ( nh1 <= max_height ) && ( nh2 <= max_height ) );
    }
    while( ( iter < 1000 ) && ( good == false ) );

    if( iter == 1000 ) throw std::runtime_error( "No nodes suitable for cross over found!" );

    detail::crossover_impl( t1 , t2 , n1 , n2 );
    complete_tree_structure( t1 );
    complete_tree_structure( t2 );
}


} // namespace gp

#endif // CROSSOVER_H_INCLUDED
