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
#include <gpcxx/operator/any_genetic_operator.hpp>

#include <cassert>
#include <random>
#include <vector>
#include <functional>

namespace gpcxx {


template< typename Population , typename Fitness , typename Rng >
class dynamic_pipeline
{
public:
    
    using population_type = Population;
    using individual_type = typename population_type::value_type;
    using fitness_type = Fitness;
    using rng_type = Rng;
    using genetic_operator_type = any_genetic_operator< population_type , fitness_type >;
    using index_vector = std::vector< size_t >;
    using operator_observer_type = std::function< void( int , index_vector const& , index_vector const& ) >;
    


    dynamic_pipeline( rng_type &rng , size_t number_elite , operator_observer_type op = []( int choice , index_vector const& in , index_vector const& out ) {} )
        : m_rng( rng )
        , m_number_elite( number_elite ) 
        , m_rates() , m_operators()
        , m_observer( std::move( op ) )
    { }
    
    void add_operator( genetic_operator_type const& op , double rate )
    {
        assert( op.arity() > 0 );
        m_operators.push_back( op );
        m_rates.push_back( rate / double( op.arity() ) );
    }
    
    operator_observer_type& operator_observer( void )
    {
        return m_observer;
    }
    
    genetic_operator_type const& operator_observer( void ) const
    {
        return m_observer;
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
        index_vector elite_in_indices;
        index_vector elite_out_indices;
        for( size_t i=0 ; i<m_number_elite ; ++i )
        {
            size_t index = indices[i] ;
            elite_in_indices.push_back( index );
            elite_out_indices.push_back( new_pop.size() );
            new_pop.push_back( pop[ index ] );
        }
        m_observer( -1 , elite_in_indices , elite_out_indices );

        size_t n = pop.size();
        std::discrete_distribution< int > dist( m_rates.begin() , m_rates.end() );
        while( new_pop.size() < n )
        {
            int choice = dist( m_rng );
            auto& op = m_operators[ choice ];
            
            auto selection = op.selection( pop , fitness );
            index_vector in;
            for( auto s : selection ) in.push_back( s - pop.begin() );
            
            auto trees = op.operation( selection );
            index_vector out;
            for( auto iter = trees.begin() ; ( iter != trees.end() ) && ( new_pop.size() < n ) ; ++iter )
            {
                out.push_back( new_pop.size() );
                new_pop.push_back( std::move( *iter ) );
            }
            m_observer( choice , in , out );
        }
        
        pop = std::move( new_pop );
    }

    rng_type& m_rng;
    double m_number_elite;
    std::vector< double > m_rates;
    std::vector< genetic_operator_type > m_operators;
    operator_observer_type m_observer;
};


} // namespace gpcxx


#endif // GPCXX_EVOLVE_DYNAMIC_PIPELINE_HPP_INCLUDED
