/*
 * main.cpp
 * Date: 2013-01-25
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 *
 * ToDo :
 * * mutation
 * * evaluation, symbol
 * * genetic algorithm
 * * testdata for nonlinear function
 *
 */

#include "tree.h"
#include "tree_io.h"
#include "tree_samples.h"
#include "generate_random_tree.h"
#include "cross_over.h"

#include <iostream>

#define tab "\t"

using namespace std;


int main( int argc , char *argv[] )
{
    std::mt19937 rng;

    // tree< char > t;
    // generate_simple_tree( t );
    // print_graph( t , "graph.pdf" );


    random_symbol_generator< char , std::mt19937 > unary_gen( { 'S' , 'C' , 'E' } , rng , 1 );
    random_symbol_generator< char , std::mt19937 > binary_gen( { 'P' , 'M' , 'S' } , rng , 2 );
    random_symbol_generator< char , std::mt19937 > terminal_gen( { 'x' , 'y' , 'c' } , rng , 0 );

     size_t min_height = 12 , max_height = 18;
     tree< char > t1 , t2;
     generate_random_tree( t1 , unary_gen , binary_gen , terminal_gen , rng , min_height , max_height );
     generate_random_tree( t2 , unary_gen , binary_gen , terminal_gen , rng , min_height , max_height );

     // print_graph( t1 , "graph1.pdf" );
     // print_graph( t2 , "graph2.pdf" );

     for( size_t i=0 ; i<1000 ; ++i )
         cross_over( t1 , t2 , rng , max_height * 2 );

     // cross_over( t1 , t2 , 2 , 18 );
     // cross_over( t1 , t2 , 0 , 2 );

//     cout << "\n\n\n";

//     print_details( t1 );
//     cout << "\n";
//     print_details( t2 );



    // print_graph( t1 , "graph1_after.pdf" );
    // print_graph( t2 , "graph2_after.pdf" );




    return 0;
}
