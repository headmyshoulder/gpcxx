/*
  gpcxx/evolve/static_pipeline.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GPCXX_EVOLVE_STATIC_PIPELINE_HPP_DEFINED
#define GPCXX_EVOLVE_STATIC_PIPELINE_HPP_DEFINED

#include <gpcxx/util/sort_indices.hpp>

#include <functional>
#include <cassert>


#include <gpcxx/io/simple.hpp>
#include <gpcxx/operator/one_point_crossover_strategy.hpp>
#include <gpcxx/operator/tournament_selector.hpp>
#include <iostream>
using namespace std;


namespace gpcxx {
    
    
template< typename Population , typename Fitness , typename Rng >
class static_pipeline
{
public:
    
    typedef Population population_type;
    typedef typename population_type::value_type individual_type;
    typedef Fitness fitness_type;
    typedef Rng rng_type;

    typedef std::function< individual_type( population_type const& , fitness_type const& ) > mutation_type;
    typedef std::function< std::pair< individual_type , individual_type >( population_type const& , fitness_type const& ) > crossover_type;
    typedef std::function< individual_type( population_type const& , fitness_type const& ) > reproduction_type;

    static_pipeline( size_t number_elite , double mutation_rate , double crossover_rate , double reproduction_rate , rng_type &rng )
        : m_number_elite( number_elite ) , m_mutation_rate( mutation_rate ) , m_crossover_rate( crossover_rate ) , m_reproduction_rate( reproduction_rate )
        , m_rng( rng )
        , m_mutation_function() , m_crossover_function() , m_reproduction_function()
    { }

    void next_generation( population_type &pop , fitness_type &fitness )
    {
        reproduce( pop , fitness );
    }


    mutation_type& mutation_function( void ) { return m_mutation_function; }
    crossover_type& crossover_function( void ) { return m_crossover_function; }
    reproduction_type& reproduction_function( void ) { return m_reproduction_function; }

    const mutation_type& mutation_function( void ) const { return m_mutation_function; }
    const crossover_type& crossover_function( void ) const { return m_crossover_function; }
    const reproduction_type& reproduction_function( void ) const { return m_reproduction_function; }
    

private:


    void reproduce( population_type &pop , fitness_type &fitness )
    {
        assert( pop.size() == fitness.size() );

        std::vector< size_t > indices;
        sort_indices( fitness , indices );
 
        population_type new_pop;
 
        // elite
        for( size_t i=0 ; i<m_number_elite ; ++i )
        {
            size_t index = indices[i] ;
            new_pop.push_back( pop[ index ] );
        }
        
        for( size_t j=0 ; j<new_pop.size() ; ++j )
            cout << new_pop.size() << "\t" << simple( new_pop[j] ) << endl;


        size_t n = pop.size();
        std::discrete_distribution< int > dist( { m_mutation_rate , m_crossover_rate , m_reproduction_rate } );
        while( new_pop.size() < n )
        {
            int choice = dist( m_rng );
            switch( choice )
            {
                case 0 : // mutation
                    {
                        individual_type mutated_tree = m_mutation_function( pop , fitness );
                        new_pop.push_back( std::move( mutated_tree ) );
                    }
                    break;
                case 1 : // crossover
                    {
//                        std::pair< individual_type , individual_type > trees = m_crossover_function( pop , fitness );
                        tournament_selector< Rng > selector( m_rng , 15 );
                        auto t1 = selector( pop , fitness );
                        auto t2 = selector( pop , fitness );
                        cout << "CC: \t" << simple( t1 ) << " " << t1.size() << endl;
                        cout << "CC: \t" << simple( t2 ) << " " << t2.size() << endl;
                        one_point_crossover_strategy< Rng > crossover( m_rng , 8 , 100 );
                        crossover( t1 , t2 );
                        cout << "XCC: \t" << simple( t1 ) << " " << t1.size() << endl;
                        cout << "XCC: \t" << simple( t2 ) << " " << t2.size() << endl;
                        if( new_pop.size() == ( n - 1 ) )
                        {
//                             new_pop.push_back( std::move( trees.first ) );
                            new_pop.push_back( std::move( t1 ) );
                        }
                        else
                        {
//                             new_pop.push_back( std::move( trees.first ) );
//                             new_pop.push_back( std::move( trees.second ) );
                            new_pop.push_back( std::move( t1 ) );
                            new_pop.push_back( std::move( t2 ) );
                        }
                    }
                    break;
                case 2 : // reproduction
                    {
                        individual_type reproduced_node = m_reproduction_function( pop , fitness );
                        new_pop.push_back( std::move( reproduced_node ) );
                    }
                    break;
            }
            
            for( size_t j=0 ; j<new_pop.size() ; ++j )
                cout << new_pop.size() << "\t" << choice << "\t" << simple( new_pop[j] ) << endl;
            for( size_t j=0 ; j<pop.size() ; ++j )
                cout << "xxx" << "\t" << simple( pop[j] ) << endl;
            cout << endl;
        }
        cout << endl << endl;
        
        pop = std::move( new_pop );
    }

    double m_number_elite;
    double m_mutation_rate;
    double m_crossover_rate;
    double m_reproduction_rate;
    rng_type &m_rng;
    mutation_type m_mutation_function;
    crossover_type m_crossover_function;
    reproduction_type m_reproduction_function;
};




} // namespace gpcxx


#endif // GPCXX_EVOLVE_STATIC_PIPELINE_HPP_DEFINED
