/*
 * eval_perf.cpp
 * Date: 2013-05-10
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include "eval_perf.hpp"

#include <Amboss/Util/Timer.h>
#include <gp/tree/generate_random_linked_tree.hpp>
#include <gp/io/polish.hpp>

#include <iostream>
#include <functional>
#include <fstream>

#define tab "\t"

using namespace std;

typedef std::vector< double > vector_t;
typedef tree_eval::context_type context_type;
typedef std::mt19937 rng_type ;



template< class Rng >
void generate_test_data( vector_t &x1 , vector_t &x2 , vector_t &x3 , size_t n , Rng &rng )
{
    std::normal_distribution< double > dist1( 0.0 , 1.0 ) , dist2( 0.0 , 1.0 ) , dist3( 0.0 , 1.0 );
    x1.resize( n ) ; x2.resize( n ) ; x3.resize( n );
    for( size_t i = 0 ; i < n ; ++i )
    {
        x1[i] = dist1( rng );
        x2[i] = dist2( rng );
        x3[i] = dist3( rng );
    }
}

/// \return time for create of tree, time for evaluation
std::pair< double , double > run_test( rng_type &rng , size_t height  ,
                                       const vector_t &x1 , const vector_t &x2 , const vector_t &x3 ,
                                       const std::string &tree_filename , const std::string &result_filename )
{
    Amboss::Util::Timer timer;
    std::pair< double , double > res;

    generators< rng_type > gen( rng );
    std::array< int , 3 > weights = {{ 2 * int( gen.gen0.num_symbols() ) ,
                                           int( gen.gen1.num_symbols() ) ,
                                           int( gen.gen2.num_symbols() ) }};
    std::function< void( tree_type& ) > tree_generator = gp::make_tree_generator_binder( rng , gen.gen0 , gen.gen1 , gen.gen2 ,
                                                                                          height , height , weights );

    tree_type tree;

    timer.restart();
    tree_generator( tree );
    res.first = timer.seconds();

    ofstream fout1( tree_filename );
    fout1.precision( 14 );
    fout1 << gp::polish( tree );

    size_t n = x1.size();
    vector_t y( n );

    timer.restart();
    for( size_t i=0 ; i<n ; ++i )
    {
        context_type c { x1[i] , x2[i] , x3[i] };
        y[i] = tree_eval::eval( tree.data() , c );
    }
    res.second = timer.seconds();

    ofstream fout2( result_filename );
    fout2.precision( 14 );
    for( size_t i=0 ; i<n ; ++i )
        fout2 << x1[i] << tab << x2[i] << tab << x3[i] << tab << y[i] << endl;

    return res;
}



int main( int argc , char *argv[] )
{
    rng_type rng;
    size_t n = 1024 * 1024 * 8;

    vector_t x1, x2 , x3;

    // generate test data
    generate_test_data( x1 , x2 , x3 , n , rng );

    ofstream fout( "result.dat" );
    fout.precision( 14 );
    for( size_t height=2 ; height<10 ; ++height )
    {
        cout.precision( 14 );
        cout << "Starting test with height " << height << endl;
        std::pair< double , double > times = run_test( rng , height , x1 , x2 , x3 ,
                                                       std::string( "tree_" ) + std::to_string( height ) + ".dat" ,
                                                       std::string( "data_" ) + std::to_string( height ) + ".dat" );
        cout << tab << "Finished!" << endl;
        cout << tab << "Tree creation time " << times.first << endl;
        cout << tab << "Evaluation time " << times.second << endl << endl;

        fout << height << tab << times.first << tab << times.second << endl;
    }

    
    return 0;
}
