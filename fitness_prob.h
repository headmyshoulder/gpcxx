/*
 * fitness_prob.h
 * Date: 2013-01-31
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef FITNESS_PROB_H_INCLUDED
#define FITNESS_PROB_H_INCLUDED

#include <vector>
#include <random>
#include <algorithm>
#include <cassert>

#include <iostream>
#define tab "\t"
using namespace std;

template< class Fitness >
class fitness_prob
{
public:

    typedef Fitness fitness_t;
    typedef std::vector< size_t > index_t;

    fitness_prob( const fitness_t &fitness )
        : m_fitness( fitness ) , m_indices( fitness.size() ) , m_prob( fitness.size() ) , m_disc_dist()
    {
        initialize();
    }

    template< class Rng >
    size_t random_index( Rng &rng )
    {
        // if( m_fitness.empty() ) return -1;

        // std::uniform_real_distribution<> dist( 0.0 , 1.0 );

        // double val = dist( rng );

        // auto i2 = lower_bound( m_prob.begin() , m_prob.end() , val );
        // auto i1 = i2;
        // if( i1 != m_prob.begin() ) --i1;
        
        // assert( i1 != m_prob.end() );
        // assert( i2 != m_prob.end() );

        // auto i = ( ( val - *i1 ) > ( *i2 - val ) ) ? i2 : i1 ;
        // auto diff = std::distance( m_prob.begin() , i );
        // assert( diff >= 0 );
        // return m_indices[ size_t( diff ) ];

        size_t ind = m_disc_dist( rng );
        return m_indices[ ind ];
    }

    const index_t& indices( void ) const { return m_indices; }
    const fitness_t prob( void ) const { return m_prob; }

private:

    void initialize( void )
    {
        if( m_fitness.empty() ) return;

        for( size_t i=0 ; i<m_indices.size() ; ++i ) m_indices[i] = i;
        std::vector< size_t >::iterator iter = std::partition( m_indices.begin() , m_indices.end() ,
                                                               [&m_fitness]( size_t i ) { return isfinite( m_fitness[i] ); } );
        auto n = std::distance( m_indices.begin() , iter );
        assert( n >= 0 );

        std::sort( m_indices.begin() , iter ,
                   [&m_fitness]( size_t i , size_t j ) { return m_fitness[i] < m_fitness[j]; } );

        double s = std::accumulate( m_indices.begin() , iter , 0.0 ,
                                    [&m_fitness]( double s , size_t i ) { return s + m_fitness[i]; } );
        double start = m_fitness[ m_indices[0] ];
        m_prob.resize( n );
        std::transform( m_indices.begin() , iter , m_prob.begin() ,
                        [n,s,start,&m_fitness]( size_t i ) { return ( m_fitness[i] - start ) / ( s - double( n ) * start ); } );
        std::partial_sum( m_prob.begin() , m_prob.end() , m_prob.begin() );


        std::vector< double > weights;
        for( size_t i=0 ; i<n ; ++i )
            weights.push_back( 0.5 * double( n-1-i ) / double( n * ( n + 1 ) ) );

        m_disc_dist.param( std::discrete_distribution<>::param_type( weights.begin() , weights.end() ) );
    }

    const fitness_t &m_fitness ;
    index_t m_indices;
    fitness_t m_prob;
    std::discrete_distribution<> m_disc_dist;
};


#endif // FITNESS_PROB_H_INCLUDED
