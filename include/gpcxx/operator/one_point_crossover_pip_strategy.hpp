/*
 * one_point_crossover_strategy.h
 * Date: 2013-01-26
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef ONE_POINT_CROSSOVER_PIP_STRATEGY_H_INCLUDED
#define ONE_POINT_CROSSOVER_PIP_STRATEGY_H_INCLUDED


#include <random>
#include <stdexcept>
#include <algorithm>
#include <vector>



namespace gpcxx {


template< typename Rng >
class one_point_crossover_pip_strategy
{
public:

    one_point_crossover_pip_strategy( Rng &rng , size_t max_height , size_t max_iterations, double internal_node_favor_rate = 0.9 )
    : m_rng( rng ) , m_max_height( max_height ) , m_max_iterations( max_iterations ) , m_internal_node_favor_rate ( internal_node_favor_rate ) { }
    

    template< class Tree >
    void operator()( Tree &t1 , Tree &t2 )
    {
        typedef typename Tree::cursor cursor;

        std::vector<cursor> internel_nodes1;
        std::vector<cursor> terminal_nodes1;

        std::vector<cursor> internel_nodes2;
        std::vector<cursor> terminal_nodes2;

        for(int i = 0; i < t1.size(); ++i)
        {
            cursor n = t1.rank_is( i );
            if(n.height() > 1)
                internel_nodes1.emplace_back(n);
            else
                terminal_nodes1.emplace_back(n);
        }

        for(int i = 0; i < t2.size(); ++i)
        {
            cursor n = t2.rank_is( i );
            if(n.height() > 1)
                internel_nodes2.emplace_back(n);
            else
                terminal_nodes2.emplace_back(n);
        }

        std::uniform_real_distribution< double > internal_node_favor_dist(0.0, 1.0);

        bool select_internal_node1 = internal_node_favor_dist( m_rng ) <= m_internal_node_favor_rate;
        bool select_internal_node2 = internal_node_favor_dist( m_rng ) <= m_internal_node_favor_rate;

        std::vector<cursor> & to_select_from1  = select_internal_node1 && !internel_nodes1.empty() ? internel_nodes1 : terminal_nodes1;
        std::vector<cursor> & to_select_from2  = select_internal_node2 && !internel_nodes2.empty() ? internel_nodes2 : terminal_nodes2;

        std::uniform_int_distribution< size_t > dist1( 0 , to_select_from1.size() - 1 );
        std::uniform_int_distribution< size_t > dist2( 0 , to_select_from2.size() - 1 );
        


        bool good = true;
        size_t iter = 0;
        cursor n1  , n2 ;
        do
        {
            n1 = to_select_from1[ dist1( m_rng ) ];
            n2 = to_select_from1[ dist2( m_rng ) ];

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
one_point_crossover_pip_strategy< Rng > make_one_point_crossover_pip_strategy( Rng &rng , size_t max_height , size_t max_iterations = 100, double internal_node_favor_rate = 0.9 )
{
    return one_point_crossover_pip_strategy< Rng >( rng , max_height , max_iterations , internal_node_favor_rate );
}

} // namespace gpcxx

#endif // ONE_POINT_CROSSOVER_PIP_STRATEGY_H_INCLUDED
