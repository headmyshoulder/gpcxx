/*
 * main.cpp
 * Date: 2013-01-25
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 *
 * ToDo :
 * OK * mutation
 * OK * evaluation, symbol
 * * genetic algorithm
 * * testdata for nonlinear function
 *
 */

#include "tree.h"
#include "tree_io.h"
#include "tree_samples.h"
#include "generate_random_tree.h"
#include "cross_over.h"
#include "evaluation.h"
#include "mutation.h"

#include "genetic_optimizer.h"

#include <iostream>

#define tab "\t"

using namespace std;


int main( int argc , char *argv[] )
{
    size_t pop_size = 600;
    size_t min_height = 12;
    size_t max_height = 15;
    genetic_optimizer optimizer( 600 , 12 , 15 );

    for( size_t i=0 ; i<10 ; ++i )
    {
        optimizer.iterate( y , x1 , x2 , x3 );
        genetic_optimizer::fitness_vector_t fitness = optimizer.fitness_vector();
        sort( fitness.begin() , fitness.end() );
        
    }

    return 0;
}
