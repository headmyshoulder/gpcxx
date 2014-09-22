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
    
    one_point_crossover_strategy( Rng &rng , size_t max_height , size_t max_iterations )
    : m_rng( rng ) , m_max_height( max_height ) , m_max_iterations( max_iterations ) { }
    

    template< class Tree >
    void operator()( Tree &t1 , Tree &t2 )
    {
        typedef typename Tree::cursor cursor;

        std::uniform_int_distribution< size_t > dist1( 0 , t1.size() - 1 );
        std::uniform_int_distribution< size_t > dist2( 0 , t2.size() - 1 );

        bool good = true;
        size_t iter = 0;
        cursor n1  , n2 ;
        do
        {
            size_t i1 = dist1( m_rng );
            size_t i2 = dist2( m_rng );

            n1 = t1.rank_is( i1 );
            n2 = t2.rank_is( i2 );

            size_t nh1 = n1.level() + n2.height();
            size_t nh2 = n2.level() + n1.height();
            good = ( ( nh1 <= m_max_height ) && ( nh2 <= m_max_height ) );
            ++iter;
        }
        while( ( iter < m_max_iterations ) && ( good == false ) );

        if( good )
            swap_subtrees( t1 , n1 , t2 , n2 );
    }
    
private:
    
    Rng &m_rng;
    size_t m_max_height;
    size_t m_max_iterations;
};

template< class Rng >
one_point_crossover_strategy< Rng > make_one_point_crossover_strategy( Rng &rng , size_t max_height , size_t max_iterations = 100 )
{
    return one_point_crossover_strategy< Rng >( rng , max_height , max_iterations );
}

} // namespace gpcxx

#endif // ONE_POINT_CROSSOVER_STRATEGY_H_INCLUDED
