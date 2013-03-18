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
        node_type *p1 = n1->parent_ptr();
        node_type *p2 = n2->parent_ptr();

        if( ( p1 == 0 ) && ( p2 == 0 ) )
        {
            v1.swap( v2 );
        }
        else if( ( p1 == 0 ) )
        {
            node_type *tmp = v1;
            v1.set_raw_data( n2 );
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
            node_type *tmp = v2.data();
            v2.set_raw_data( n1 );
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



struct crossover
{
    template< class Tree >
    static void crossover_impl( Tree &v1 , Tree &v2 , size_t i1 , size_t i2 )
    {
        typedef typename Tree::node_type node_type;
        node_type *n1 = &v1[ i1 ];
        node_type *n2 = &v2[ i2 ];
        if( ( n1 == 0 ) || ( n2 == 0 ) ) return;
        detail::crossover_impl( v1 , v2 , n1 , n2 );
    }

    template< class Tree , class Rng >
    void operator()( Tree &t1 , Tree &t2 , Rng &rng , size_t max_height )
    {
        typedef typename Tree::node_type node_type;

        std::uniform_int_distribution< size_t > dist1( 0 , t1.num_elements() - 1 );
        std::uniform_int_distribution< size_t > dist2( 0 , t2.num_elements() - 1 );

    

        bool good = true;
        size_t iter = 0;
        node_type *n1 = 0 , *n2 = 0;
        do
        {
            size_t i1 = dist1( rng );
            size_t i2 = dist2( rng );

            n1 = &t1[ i1 ];
            n2 = &t2[ i2 ];

            size_t nh1 = n1->level() + n2->height() - 1;
            size_t nh2 = n2->level() + n1->height() - 1;
            good = ( ( nh1 <= max_height ) && ( nh2 <= max_height ) );
        }
        while( ( iter < 1000 ) && ( good == false ) );

        if( iter == 1000 ) throw std::runtime_error( "No nodes suitable for cross over found!" );

        detail::crossover_impl( t1 , t2 , n1 , n2 );
    }
};

template< class Rng >
struct crossover_binder
{
    Rng &m_rng;
    size_t m_max_height;
    crossover_binder( Rng &rng , size_t max_height ) : m_rng( rng ) , m_max_height( max_height ) { }

    template< class Tree >
    void operator()( Tree &t1 , Tree &t2 ) const
    {
        return crossover()( t1 , t2 , m_rng , m_max_height );
    }
};

template< class Rng >
crossover_binder< Rng > make_crossover_binder( Rng &rng , size_t max_height )
{
    return crossover_binder< Rng >( rng , max_height );
}

} // namespace gp

#endif // CROSSOVER_H_INCLUDED
