/*
 * genetic_optimizer.h
 * Date: 2013-01-27
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef GENETIC_OPTIMIZER_H_INCLUDED
#define GENETIC_OPTIMIZER_H_INCLUDED

/*
Algorithm

initialize random trees
for i:
  evaluate fitness
  reproduce
  mutate
  crossover
*/

struct genetic_optimizer
{
    typedef std::vector< double > vector_t;
    typedef tree< char > pheno_t;
    typedef std::vector< pheno_t > population_t;
    typedef std::vector< double > fitness_vector_t;

    struct fitness_t
    {
        typedef eval_three eval_t;
        typedef eval_t::context_t context_t;

        static void evaluate( const pheno_t &p , vector_t &y , const vector_t &x1 , const vector_t &x2 , const vector_t &x3 ) const
        {
            for( size_t i=0 ; i<y1.size() ; ++i )
            {
                context_t c;
                c.x = x1[i] ; c.y = x2[i] ; c.z = x3[i];
                y[i] = eval_t::eval( p.m_data , c );
            }
        }

        static double error( const vector_t &y1 , const vector_t &y2 ) const
        {
            double err = 0.0;
            for( size_t i=0 ; i<y1.size() ; ++i ) err += ( y1[i] - y2[i] ) * ( y1[i] - y2[i] );
            return sqrt( err / double( y1.size() ) );
        }

        static double fitness( const pheno_t & ,  const vector_t &y , const vector_t &x1 , const vector_t &x2 , const vector_t &x3 )
        {
            size_t N = y.size();
            vector_t ye( N );
            evaluate( pop , ye , x1 , x2 , x3 );
            double err = error( y , y2 );
            return err;
        }
    };

    void initialize( population_t &p )
    {
    }

    void iterate( const vector_t &y , const vector_t &x1 , const vector_t &x2 , const vector_t &x3 ) const
    {
        for( size_t i=0 ; i<population.size() ; ++i )
        {
            fitness[i] = fitness_struct::fitness( population[i] , y , x1 , x2 , x3 );
            reproduce( population , errors );
            mutate( population );
            cross_over( population );
        }
    }

    void reproduce( population_t &p , const fitness_vector_t &fitness  )
    {
    }

    void mutate( population_t &p )
    {
    }

    void cross_over( population_t &p )
    {
    }

};

#endif // GENETIC_OPTIMIZER_H_INCLUDED
