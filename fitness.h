/*
 * fitness.h
 * Date: 2013-01-31
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef FITNESS_H_INCLUDED
#define FITNESS_H_INCLUDED

#include "tree.h"
#include "evaluation.h"

struct fitness_t
{
    typedef eval_three eval_t;
    typedef eval_t::context_t context_t;
    typedef tree< char > pheno_t;
    typedef std::vector< double > vector_t;

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


#endif // FITNESS_H_INCLUDED
