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


struct crossover
{
    template< class Tree >
    static void crossover_impl( Tree &v1 , Tree &v2 , size_t i1 , size_t i2 )
    {
        typedef typename Tree::node_type node_type;
        v1[ i1 ].swap( v2[i2] );
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

        n1->swap( n2 );
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
