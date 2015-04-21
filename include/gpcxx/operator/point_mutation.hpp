/*
 * gpcxx/operator/point_mutation.hpp
 * Date: 2013-09-13
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_OPERATOR_POINT_MUTATION_HPP_INCLUDED
#define GPCXX_OPERATOR_POINT_MUTATION_HPP_INCLUDED

#include <gpcxx/operator/detail/operator_base.hpp>

#include <random>

namespace gpcxx {

    
template< typename Rng             // models RandomNumberEngine
        , typename TreeGenerator   // models TreeGenerator
        >
class point_mutation : public detail::operator_base< 1 >
{
public:

    point_mutation( Rng &rng , TreeGenerator &gen , size_t max_height , size_t max_trials )
    : m_rng( rng ) , m_gen( gen ) , m_max_height( max_height ) , m_max_trials( max_trials ) { }


    template< class Tree >
    void operator()( Tree &t )
    {
        typedef typename Tree::cursor cursor;

        if( t.empty() ) return;
        std::uniform_int_distribution< size_t > dist( 0 , t.size() - 1 );

        size_t count = 0;
        do
        {
            size_t index = dist( m_rng );
            cursor n = t.rank_is( index );

            Tree new_subtree;
            m_gen( new_subtree );

            if( ( n.level() + new_subtree.root().height() ) <= m_max_height )
            {
                t.swap_subtrees( n , new_subtree , new_subtree.root() );
                break;
            }
            ++count;
        }
        while( count < m_max_trials );
    }
    
private:
    
    Rng &m_rng;
    TreeGenerator &m_gen;
    size_t m_max_height;
    size_t m_max_trials;
};


template< typename Rng , typename TreeGenerator >
point_mutation< Rng , TreeGenerator > make_point_mutation( Rng &rng , TreeGenerator &gen , size_t max_height , size_t max_trials )
{
    return point_mutation< Rng , TreeGenerator >( rng , gen , max_height , max_trials );
}





} // namespace gpcxx


#endif // GPCXX_OPERATOR_POINT_MUTATION_HPP_INCLUDED
