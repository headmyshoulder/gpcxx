/*
 * eval_perf.cpp
 * Date: 2013-05-10
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#define FUSION_MAX_VECTOR_SIZE 20

#include <gpcxx/eval/static_eval.hpp>
#include <gpcxx/generate/basic_generate_strategy.hpp>
#include <gpcxx/generate/uniform_symbol.hpp>
#include <gpcxx/io/simple.hpp>
#include <gpcxx/tree/basic_tree.hpp>
#include <gpcxx/app/timer.hpp>

#include <boost/fusion/include/make_vector.hpp>

#include <iostream>
#include <functional>
#include <fstream>
#include <vector>
#include <random>
#include <cmath>

#define tab "\t"

using namespace std;
namespace fusion = boost::fusion;

typedef double value_type;
typedef std::vector< value_type > vector_type;
typedef std::array< double , 3 > context_type;
typedef char symbol_type;
typedef std::mt19937 rng_type ;


template< class Rng >
void generate_test_data( vector_type &x1 , vector_type &x2 , vector_type &x3 , size_t n , Rng &rng )
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
template< typename Tree >
std::tuple< double , double , double > run_test( rng_type &rng , size_t number_of_trees , size_t height  ,
                                       const vector_type &x1 , const vector_type &x2 , const vector_type &x3 ,
                                       const std::string &tree_filename , const std::string &result_filename )
{
    auto eval = gpcxx::make_static_eval< value_type , symbol_type , context_type >(
        fusion::make_vector(
            fusion::make_vector( '1' , []( context_type const& t ) { return 1.0; } )
          , fusion::make_vector( '2' , []( context_type const& t ) { return 2.0; } )
          , fusion::make_vector( '3' , []( context_type const& t ) { return 3.0; } )
          , fusion::make_vector( '4' , []( context_type const& t ) { return 4.0; } )
          , fusion::make_vector( '5' , []( context_type const& t ) { return 5.0; } )
          , fusion::make_vector( '6' , []( context_type const& t ) { return 6.0; } )
          , fusion::make_vector( '7' , []( context_type const& t ) { return 7.0; } )
          , fusion::make_vector( '8' , []( context_type const& t ) { return 8.0; } )
          , fusion::make_vector( '9' , []( context_type const& t ) { return 9.0; } )
          , fusion::make_vector( 'x' , []( context_type const& t ) { return t[0]; } )
          , fusion::make_vector( 'y' , []( context_type const& t ) { return t[1]; } )
          , fusion::make_vector( 'z' , []( context_type const& t ) { return t[2]; } )          
          ) ,
        fusion::make_vector(
            fusion::make_vector( 's' , []( double v ) -> double { return std::sin( v ); } )
          , fusion::make_vector( 'c' , []( double v ) -> double { return std::cos( v ); } ) 
          ) ,
        fusion::make_vector(
            fusion::make_vector( '+' , std::plus< double >() )
          , fusion::make_vector( '-' , std::minus< double >() )
          , fusion::make_vector( '*' , std::multiplies< double >() ) 
          , fusion::make_vector( '/' , std::divides< double >() ) 
          ) );

    gpcxx::timer timer;
    std::tuple< double , double , double > res;

    auto terminal_gen = eval.get_terminal_symbol_distribution();
    auto unary_gen = eval.get_unary_symbol_distribution();
    auto binary_gen = eval.get_binary_symbol_distribution();
    
    std::array< int , 3 > weights = {{ 2 * int( terminal_gen.num_symbols() ) ,
                                           int( unary_gen.num_symbols() ) ,
                                           int( binary_gen.num_symbols() ) }};
    std::function< void( Tree& ) > tree_generator = gpcxx::make_basic_generate_strategy(
        rng , terminal_gen , unary_gen , binary_gen , height , height , weights );


    // 
    // GENERATION
    //
    std::vector< Tree > tree( number_of_trees );
    timer.restart();
    for( size_t t=0 ; t<number_of_trees ; ++t )
        tree_generator( tree[t] );
    std::get< 0 >( res ) = timer.seconds();

    ofstream fout1( tree_filename );
    fout1.precision( 14 );
    for( size_t t=0 ; t<number_of_trees ; ++t )
        fout1 << t << " : " << gpcxx::simple( tree[t] ) << endl;

    size_t number_of_datapoints = x1.size();
    std::vector< vector_type > y( number_of_trees );
    for( size_t t=0 ; t<number_of_trees ; ++t ) y[t] = vector_type( number_of_datapoints );


    // 
    // EVALUATION
    //
    timer.restart();
    for( size_t t=0 ; t<number_of_trees ; ++t )
    {
        for( size_t i=0 ; i<number_of_datapoints ; ++i )
        {
            context_type c { x1[i] , x2[i] , x3[i] };
            y[t][i] = eval( tree[t] , c );
        }
    }
    std::get< 1 >( res ) = timer.seconds();
    
    double sum = 0.0;
    for( size_t t=0 ; t<number_of_trees ; ++t )
    {
        for( size_t i=0 ; i<number_of_datapoints ; ++i )
            sum += y[t][i];
    }
    std::get< 2 >( res ) = sum;
    

//     ofstream fout2( result_filename );
//     fout2.precision( 14 );
//     for( size_t t=0 ; t<number_of_trees ; ++t )
//     {
//         for( size_t i=0 ; i<number_of_datapoints ; ++i )
//             fout2 << t << tab << i << tab << x1[i] << tab << x2[i] << tab << x3[i] << tab << y[t][i] << endl;
//         fout2 << endl;
//     }

    return res;
}



int main( int argc , char *argv[] )
{
    rng_type rng;
    size_t number_of_datapoints = 1024 ;
    size_t number_of_trees = 1024;

    
    vector_type x1, x2 , x3;

    // generate test data
    generate_test_data( x1 , x2 , x3 , number_of_datapoints , rng );

    ofstream fout( "result.dat" );
    fout.precision( 14 );
    cout.precision( 14 );
    for( size_t height=2 ; height<10 ; ++height )
    {
        cout.precision( 14 );
        cout << "Starting test with height " << height << endl;
        auto times = run_test< gpcxx::basic_tree< char > >(
            rng , number_of_trees , height , x1 , x2 , x3 ,
            std::string( "tree_" ) + std::to_string( height ) + ".dat" ,
            std::string( "data_" ) + std::to_string( height ) + ".dat" );
        cout << tab << "Finished!" << endl;
        cout << tab << "Tree creation time " << std::get< 0 >( times ) << endl;
        cout << tab << "Evaluation time " << std::get< 1 >( times ) << endl;
        cout << tab << "Result sum " << std::get< 2 >( times ) << endl << endl;
        

        fout << height << tab << std::get< 0 >( times ) << tab << std::get< 1 >( times ) << tab << std::get< 2 >( times ) << endl;
    }

    
    return 0;
}
