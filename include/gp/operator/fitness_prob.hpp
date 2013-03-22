/*
 * fitness_prob.hpp
 * Date: 2013-02-21
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef FITNESS_PROB_HPP_INCLUDED
#define FITNESS_PROB_HPP_INCLUDED

#include <vector>
#include <random>
#include <algorithm>
#include <cassert>
#include <cmath>

namespace gp {

template< class Fitness , class Rng >
class fitness_prob
{
public:

    typedef Fitness fitness_type;
    typedef std::vector< size_t > index_type;

    fitness_prob( const fitness_type &fitness , Rng &rng )
        : m_fitness( fitness ) , m_indices( fitness.size() ) , m_disc_dist() ,
          m_rng( rng )
    {
        initialize();
    }


    size_t random_index( void )
    {
        size_t ind = m_disc_dist( m_rng );
        return m_indices[ ind ];
    }

    const index_type& indices( void ) const { return m_indices; }

private:
    
    void initialize( void )
    {
        if( m_fitness.empty() ) return;

        for( size_t i=0 ; i<m_indices.size() ; ++i ) m_indices[i] = i;
        std::vector< size_t >::iterator iter = std::partition( m_indices.begin() , m_indices.end() ,
                                                               [this]( size_t i ) { return std::isfinite( m_fitness[i] ); } );
        auto n = std::distance( m_indices.begin() , iter );
        assert( n >= 0 );

        std::sort( m_indices.begin() , iter ,
                   [this]( size_t i , size_t j ) { return m_fitness[i] < m_fitness[j]; } );

        std::vector< double > weights;
        for( size_t i=0 ; i<n ; ++i )
            weights.push_back( 0.5 * double( n-1-i ) / double( n * ( n + 1 ) ) );

        m_disc_dist.param( std::discrete_distribution<>::param_type( weights.begin() , weights.end() ) );
    }

    const fitness_type &m_fitness ;
    index_type m_indices;
    std::discrete_distribution<> m_disc_dist;
    Rng m_rng;
};

} // namespace gp

#endif // FITNESS_PROB_HPP_INCLUDED
