/*
 * genetic_optimizer.h
 * Date: 2013-01-27
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef GENETIC_OPTIMIZER_H_INCLUDED
#define GENETIC_OPTIMIZER_H_INCLUDED

#include "tree.h"
#include "evaluation.h"
#include "generate_random_tree.h"
#include "mutation.h"
#include "cross_over.h"
#include "helpers.h"

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

class genetic_optimizer
{
public:

    typedef std::vector< double > vector_t;
    typedef std::vector< size_t > index_vector_t;
    typedef tree< char > pheno_t;
    typedef std::vector< pheno_t > population_t;
    typedef std::vector< double > fitness_vector_t;

    genetic_optimizer( size_t population_size , size_t min_height , size_t max_height ,
                       double reproduction_rate , double mutation_rate , double crossover_rate )
        : m_pop() , m_fitness() ,
          m_min_height( min_height ) , m_max_height( max_height ) ,
          m_reproduction_rate( reproduction_rate ) , m_mutation_rate( mutation_rate ) , m_crossover_rate( crossover_rate )
    {
        initialize( population_size );
    }

    struct fitness_t
    {
        typedef eval_three eval_t;
        typedef eval_t::context_t context_t;

        static void evaluate( const pheno_t &p , vector_t &y , const vector_t &x1 , const vector_t &x2 , const vector_t &x3 )
        {
            for( size_t i=0 ; i<y.size() ; ++i )
            {
                context_t c;
                c.x = x1[i] ; c.y = x2[i] ; c.z = x3[i];
                y[i] = eval_t::eval( p.m_data , c );
            }
        }

        static double error( const vector_t &y1 , const vector_t &y2 )
        {
            double err = 0.0;
            for( size_t i=0 ; i<y1.size() ; ++i ) err += ( y1[i] - y2[i] ) * ( y1[i] - y2[i] );
            return sqrt( err / double( y1.size() ) );
        }

        static double fitness( const pheno_t &p ,  const vector_t &y , const vector_t &x1 , const vector_t &x2 , const vector_t &x3 )
        {
            size_t N = y.size();
            vector_t ye( N );
            evaluate( p , ye , x1 , x2 , x3 );
            double err = error( ye , y );
            return err;
        }

        eval_t m_eval;
        std::mt19937 m_rng;
    };

    void iterate( const vector_t &y , const vector_t &x1 , const vector_t &x2 , const vector_t &x3 )
    {
        for( size_t i=0 ; i<m_pop.size() ; ++i )
        {
            cout << i << " from " << m_pop.size() << endl;
            m_fitness_vector[i] = fitness_t::fitness( m_pop[i] , y , x1 , x2 , x3 );
        }

        reproduce( m_pop , m_fitness_vector );
//        mutate( m_pop );
        cross_over( m_pop );
    }

    void reproduce( population_t &p , fitness_vector_t &fitness )
    {
        index_vector_t idx;
        auto iter = sort_indexes( fitness , idx );
        size_t n_repro = size_t( double( m_pop.size() ) * m_reproduction_rate );
        auto iter2 = std::min( idx.begin() + n_repro , iter );
        std::uniform_int_distribution< size_t > dist( 0 , iter2 - idx.begin() - 1 );
        for_each( iter2 , idx.end() , [&]( size_t i ) { m_pop[i] = m_pop[ idx[ dist(m_fitness.m_rng) ] ]; } );
    }

    void mutate( population_t &p )
    {
        size_t n_mut = size_t( double( m_pop.size() ) * m_mutation_rate );
        index_vector_t idx;
        create_random_indexes( idx , m_pop.size() , n_mut );
        random_symbol_generator< char , std::mt19937 > terminal_gen( m_fitness.m_eval.terminal_symbols , m_fitness.m_rng , 0 );
        random_symbol_generator< char , std::mt19937 > unary_gen( m_fitness.m_eval.unary_symbols , m_fitness.m_rng , 1 );
        random_symbol_generator< char , std::mt19937 > binary_gen( m_fitness.m_eval.binary_symbols , m_fitness.m_rng , 2 );
        for( size_t i=0 ; i<idx.size() ; ++i )
            mutation( m_pop[ idx[i] ] , m_fitness.m_rng , terminal_gen , unary_gen , binary_gen );
    }

    void cross_over( population_t &p )
    {
        size_t n_cross = size_t( double( m_pop.size() ) * m_crossover_rate );
    }

    const fitness_vector_t fitness_vector( void ) const { return m_fitness_vector; }


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
    double m_reproduction_rate , m_mutation_rate , m_crossover_rate;
};

#endif // GENETIC_OPTIMIZER_H_INCLUDED
