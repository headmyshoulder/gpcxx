/*
 * one_point_crossover_strategy.h
 * Date: 2013-01-26
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Author: Gerard Choinka (gerard.choinka+gpcxx@gmail.com)
 */

#ifndef ONE_POINT_CROSSOVER_PIP_STRATEGY_H_INCLUDED
#define ONE_POINT_CROSSOVER_PIP_STRATEGY_H_INCLUDED


#include <gpcxx/io.hpp>

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


        template< class Cursor >
        void seperate_nodes( Cursor c , std::vector< Cursor > & internal, std::vector< Cursor > & terminal)
        {
            if( c.size() > 1 )
            {
                internal.emplace_back( c );
                for( int i = 0 ; i < c.size() ; ++i )
                    seperate_nodes( c.children( i ) , internal , terminal );
            }
            else
            {
                terminal.emplace_back( c );
            }
        }

        template< class Tree >
        void operator()( Tree & t1 , Tree & t2 )
        {
            typedef typename Tree::cursor cursor;

            Tree * const trees[] = { &t1 , &t2 };
            size_t const n_trees = sizeof( trees ) / sizeof( trees[ 0 ] );

            std::uniform_real_distribution< double > internal_node_favor_dist( 0.0 , 1.0 );
            std::vector< cursor > selectable_nodes[ n_trees ];
            std::uniform_int_distribution< size_t > dist[ n_trees ];
            for( int i = 0 ; i < n_trees; ++i )
            {
                std::vector< cursor > internel_nodes , terminal_nodes;

                seperate_nodes( trees[ i ]->root() , internel_nodes , terminal_nodes );

                bool select_internal_node = internal_node_favor_dist( m_rng ) <= m_internal_node_favor_rate;
                if( select_internal_node and not internel_nodes.empty() )
                    selectable_nodes[ i ] = std::move ( internel_nodes );
                else
                    selectable_nodes[ i ] = std::move ( terminal_nodes );

                dist[ i ] = std::uniform_int_distribution< size_t >( 0 , selectable_nodes[ i ].size() - 1 );
            }


            bool good = true;
            size_t iteration = 0;
            cursor cur[ n_trees ];
            do
            {
                for( int i = 0 ; i < n_trees; ++i )
                    cur[ i ] = selectable_nodes[ i ][ dist[ i ]( m_rng ) ];

                size_t new_height[] = {  cur[ 0 ].level() + cur[ 1 ].height() - 1 , cur[ 1 ].level() + cur[ 0 ].height() - 1 };

                good = true;
                for( int i = 0 ; i < n_trees; ++i )
                    good = good && ( new_height[i] <= m_max_height );
            }
            while( not good and ( iteration < m_max_iterations ) );

            if( good )
                swap_subtrees( *( trees[ 0 ] ) , cur[ 0 ] , *( trees[ 1 ] ) , cur[ 1 ] );
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
