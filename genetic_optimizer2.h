/*
 * genetic_optimizer.h
 * Date: 2013-01-27
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef GENETIC_OPTIMIZER2_H_INCLUDED
#define GENETIC_OPTIMIZER2_H_INCLUDED

#include "tree.h"
#include "tree_io.h"
#include "evaluation.h"
#include "generate_random_tree.h"
#include "mutation.h"
#include "cross_over.h"
#include "helpers.h"
#include "fitness.h"
#include "fitness_prob.h"

#include <random>


#include <iostream>
using namespace std;


/*
Algorithm

initialize random trees
for i:
  evaluate fitness
  reproduce
  mutate
  crossover
*/

#include <random>

class genetic_optimizer2
{
public:

    typedef std::vector< double > vector_t;
    typedef std::vector< size_t > index_vector_t;
    typedef tree< char > pheno_t;
    typedef std::vector< pheno_t > population_t;
    typedef std::vector< double > fitness_vector_t;

    genetic_optimizer2( size_t population_size , size_t min_height , size_t max_height ,
                       double elite_rate , double mutation_rate , double crossover_rate )
        : m_pop() , m_fitness() ,
          m_min_height( min_height ) , m_max_height( max_height ) ,
          m_elite_rate( elite_rate ) , m_mutation_rate( mutation_rate ) , m_crossover_rate( crossover_rate )
    {
        initialize( population_size );
    }


    void calc_fitness( const vector_t &y , const vector_t &x1 , const vector_t &x2 , const vector_t &x3 )
    {
        for( size_t i=0 ; i<m_pop.size() ; ++i )
            m_fitness_vector[i] = fitness_t::fitness( m_pop[i] , y , x1 , x2 , x3 );
    }

    void iterate( const vector_t &y , const vector_t &x1 , const vector_t &x2 , const vector_t &x3 )
    {
        reproduce( m_pop , m_fitness_vector );
        calc_fitness( y , x1 , x2 , x3 );
    }

    void reproduce( population_t &p , fitness_vector_t &fitness )
    {
        fitness_prob< fitness_vector_t > prob( fitness );
        size_t n = p.size();

        // for( size_t i=0 ; i<n ; ++i )
        //     cout << fitness[ prob.indices()[i] ] << endl;

        random_symbol_generator< char , std::mt19937 > terminal_gen( m_fitness.m_eval.terminal_symbols , m_fitness.m_rng , 0 );
        random_symbol_generator< char , std::mt19937 > unary_gen( m_fitness.m_eval.unary_symbols , m_fitness.m_rng , 1 );
        random_symbol_generator< char , std::mt19937 > binary_gen( m_fitness.m_eval.binary_symbols , m_fitness.m_rng , 2 );

        population_t new_pop;

        // elite
        size_t n_elite = size_t( double( n ) * m_elite_rate );
        for( size_t i=0 ; i<n_elite ; ++i )
            new_pop.push_back( p[ prob.indices()[i] ] );


        // mutate
        size_t n_mutate = size_t( double( n ) * m_mutation_rate );
        for( size_t i=0 ; i<n_mutate ; ++i )
        {
            new_pop.push_back(  p[ prob.random_index( m_fitness.m_rng ) ] );
            mutation( new_pop.back() , m_fitness.m_rng , terminal_gen , unary_gen , binary_gen );
        }

        // crossover
        size_t n_crossover = size_t( double( n ) * m_crossover_rate );
        for( size_t i=0 ; i<n_crossover ; i+=2 )
        {
            new_pop.push_back( p[ prob.random_index( m_fitness.m_rng ) ] );
            new_pop.push_back( p[ prob.random_index( m_fitness.m_rng ) ] );
            ::cross_over( new_pop[ new_pop.size()-1 ] , new_pop[ new_pop.size()-2 ] , m_fitness.m_rng , m_max_height );
        }

        // random
        for( size_t i=new_pop.size() ; i<n ; ++i )
        {
            new_pop.push_back( pheno_t() );
            generate_random_tree( new_pop.back() , unary_gen , binary_gen , terminal_gen , m_fitness.m_rng , m_min_height , m_max_height );
        }

        p = new_pop;
    }

    void report_population( std::ostream &out )
    {
        index_vector_t idx;
        auto iter = sort_indexes( m_fitness_vector , idx );
//        for( size_t i=0 ; i<m_fitness_vector.size() ; ++i )
        for( size_t i=0 ; i<5 ; ++i )
        {
            out << i << " " << m_fitness_vector[ idx[i] ] << " : ";
            print_formula( m_pop[ idx[i] ] , out );
            out << endl;
        }
    }

    void report_statistics( std::ostream &out )
    {
        double height_mean = 0.0 , height_sqmean = 0.0;
        for( const auto &p : m_pop )
        {
            double h = double( p.m_data->height );
            height_mean += h;
            height_sqmean += h * h; 
        }
        height_mean /= double( m_pop.size() );
        height_sqmean /= double( m_pop.size() );
        out << "Mean height : " << height_mean << " , height stdev " << sqrt( height_sqmean - height_mean * height_mean ) << "\n\n\n";
    }


    const fitness_vector_t fitness_vector( void ) const { return m_fitness_vector; }
    const population_t& population( void ) const { return m_pop; }


private:

    void initialize( size_t pop_size )
    {
        random_symbol_generator< char , std::mt19937 > terminal_gen( m_fitness.m_eval.terminal_symbols , m_fitness.m_rng , 0 );
        random_symbol_generator< char , std::mt19937 > unary_gen( m_fitness.m_eval.unary_symbols , m_fitness.m_rng , 1 );
        random_symbol_generator< char , std::mt19937 > binary_gen( m_fitness.m_eval.binary_symbols , m_fitness.m_rng , 2 );

        m_pop.resize( pop_size );
        m_fitness_vector.resize( pop_size );
        for( size_t i=0 ; i<pop_size ; ++i )
            generate_random_tree( m_pop[i] , unary_gen , binary_gen , terminal_gen , m_fitness.m_rng , m_min_height , m_max_height );
    }

    population_t m_pop;
    fitness_t m_fitness;
    fitness_vector_t m_fitness_vector;
    size_t m_min_height , m_max_height;
    double m_elite_rate , m_mutation_rate , m_crossover_rate;
};

#endif // GENETIC_OPTIMIZER2_H_INCLUDED
