/*
 * gpcxx/evolve/dynamic_pipeline.hpp
 * Date: 2015-04-05
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_EVOLVE_DYNAMIC_PIPELINE_HPP_INCLUDED
#define GPCXX_EVOLVE_DYNAMIC_PIPELINE_HPP_INCLUDED

#include <gpcxx/util/sort_indices.hpp>

#include <functional>
#include <cassert>
#include <random>
#include <vector>

namespace gpcxx {


template< typename Population , typename Fitness , typename Rng >
class dynamic_pipeline
{
public:
    
    using population_type = Population;
    using individual_type = typename population_type::value_type;
    using fitness_type = Fitness;
    using rng_type = Rng;
    using genetic_operator_type = std::function< std::vector< individual_type >( population_type const& , fitness_type const& ) >;

    dynamic_pipeline( rng_type &rng , size_t number_elite )
        : m_rng( rng )
        , m_number_elite( number_elite ) 
        , m_rates() , m_operators()
    { }
    
    void add_operator( genetic_operator_type const& op , double rate )
    {
        m_operators.push_back( op );
        m_rates.push_back( rate );
    }

    void next_generation( population_type &pop , fitness_type &fitness )
    {
        reproduce( pop , fitness );
    }

private:


    void reproduce( population_type& pop , fitness_type& fitness )
    {
        assert( pop.size() == fitness.size() );
        assert( m_rates.size() == m_operators.size() );
        assert( m_operators.size() > 0 );

        std::vector< size_t > indices;
        sort_indices( fitness , indices );
 
        population_type new_pop;
        new_pop.reserve( pop.size() );
 
        // elite
        for( size_t i=0 ; i<m_number_elite ; ++i )
        {
            size_t index = indices[i] ;
            new_pop.push_back( pop[ index ] );
        }

        size_t n = pop.size();
        std::discrete_distribution< int > dist( m_rates.begin() , m_rates.end() );
        while( new_pop.size() < n )
        {
            int choice = dist( m_rng );
            auto& op = m_operators[ choice ];
            auto trees = op( pop , fitness );
            for( auto iter = trees.begin() ; ( iter != trees.end() ) && ( new_pop.size() < n ) ; ++iter )
                new_pop.push_back( std::move( *iter ) );
        }
        
        pop = std::move( new_pop );
    }

    rng_type& m_rng;
    double m_number_elite;
    std::vector< double > m_rates;
    std::vector< genetic_operator_type > m_operators;
};


} // namespace gpcxx


#endif // GPCXX_EVOLVE_DYNAMIC_PIPELINE_HPP_INCLUDED
