/*
 * test_prob.cpp
 * Date: 2013-01-31
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include "fitness_prob.h"

#include <iostream>

#define tab "\t"



int main( int argc , char *argv[] )
{
    using namespace std;

    const size_t n = 32;
    cout.precision( 14 );

    std::mt19937 rng;
    std::chi_squared_distribution<> dist( n );

    
    std::vector< double > fitness( n );
    std::generate( fitness.begin() , fitness.end() , [&rng,&dist]( void ) { return dist( rng ); } );

    fitness_prob< std::vector< double > >  prob( fitness );

    for( size_t i=0 ; i<fitness.size() ; ++i )
        cout << i << tab << fitness[ i ] << tab << fitness[ prob.indices()[i] ] << tab << prob.prob()[i] << endl;

    size_t i = prob.random_index( rng );
    cout << endl << i << tab << fitness[i] << endl;

    return 0;
}
