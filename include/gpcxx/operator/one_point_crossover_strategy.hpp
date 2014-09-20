/*
 * one_point_crossover_strategy.h
 * Date: 2013-01-26
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef ONE_POINT_CROSSOVER_STRATEGY_H_INCLUDED
#define ONE_POINT_CROSSOVER_STRATEGY_H_INCLUDED


#include <random>
#include <stdexcept>
#include <algorithm>



namespace gpcxx {

template< typename Rng >
class one_point_crossover_strategy
{
public:
    
    one_point_crossover_strategy( Rng &rng , size_t max_height , size_t max_iterations, double internal_node_favor_rate = 0.9 )
    : m_rng( rng ) , m_max_height( max_height ) , m_max_iterations( max_iterations ) , m_internal_node_favor_rate ( internal_node_favor_rate ) { }
    

    template< class Tree >
    void operator()( Tree &t1 , Tree &t2 )
    {
        typedef typename Tree::cursor cursor;

        std::uniform_int_distribution< size_t > dist1( 0 , t1.size() - 1 );
        std::uniform_int_distribution< size_t > dist2( 0 , t2.size() - 1 );
        
        std::uniform_real_distribution< double > internal_node_favor_dist(0.0, 1.0);
        
        bool select_internal_node1 = internal_node_favor_dist( m_rng ) <= m_internal_node_favor_rate;
        bool select_internal_node2 = internal_node_favor_dist( m_rng ) <= m_internal_node_favor_rate;

        bool good = true;
        size_t iter = 0;
        cursor n1  , n2 ;
        do
        {
            bool is_internal;
            do
            {
                size_t i1 = dist1( m_rng );
                n1 = t1.rank_is( i1 );
                is_internal = n1.height() > 1;
            }
            while( select_internal_node1 && !is_internal );
            
            
            do
            {
                size_t i2 = dist2( m_rng );
                n2 = t2.rank_is( i2 );  
                is_internal = n2.height() > 1;
            }
            while( select_internal_node2 && !is_internal );

            size_t nh1 = n1.level() + n2.height() - 1;
            size_t nh2 = n2.level() + n1.height() - 1;
            good = ( ( nh1 <= m_max_height ) && ( nh2 <= m_max_height ) );
        }
        while( ( iter < m_max_iterations ) && ( good == false ) );

        if( good )
            swap_subtrees( t1 , n1 , t2 , n2 );
    }
    
private:
    
    Rng &m_rng;
    size_t m_max_height;
    size_t m_max_iterations;
    double m_internal_node_favor_rate;
};

template< class Rng >
one_point_crossover_strategy< Rng > make_one_point_crossover_strategy( Rng &rng , size_t max_height , size_t max_iterations = 100, double internal_node_favor_rate = 0.9 )
{
    return one_point_crossover_strategy< Rng >( rng , max_height , max_iterations , internal_node_favor_rate );
}

} // namespace gpcxx

#endif // ONE_POINT_CROSSOVER_STRATEGY_H_INCLUDED
