/*
 * eval_perf.cpp
 * Date: 2013-05-10
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#define FUSION_MAX_VECTOR_SIZE 20

#include "ecj_exp_parser.hpp"

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




void generate_test_data( vector_type &x1 , vector_type &x2 , vector_type &x3 , double rmin , double rmax , double stepsize )
{
    x1.clear() ;
    x2.clear() ;
    x3.clear();
    
    for( double xx = rmin ; xx <= rmax ; xx += stepsize )
    {
        for( double yy = rmin ; yy <= rmax ; yy += stepsize )
        {
            for( double zz = rmin ; zz <= rmax ; zz += stepsize )
            {
                x1.push_back( xx );
                x2.push_back( yy );
                x3.push_back( zz );
            }
        }
    }
}

inline value_type my_log( value_type v )
{
    return ( std::abs( v ) < 1.0e-20 ) ? 0.0 : std::log( std::abs( v ) );
}

template< typename Cursor >
inline value_type eval_cursor( Cursor const &c , context_type const &context )
{
    switch( *c )
    {
        case 'x' : return context[0]; break;
        case 'y' : return context[1]; break;
        case 'z' : return context[2]; break;
        case 'e' : return exp( eval_cursor( c.children(0) , context ) ); break;
        case 'l' : return my_log( eval_cursor( c.children(0) , context ) ); break;
        case 's' : return sin( eval_cursor( c.children(0) , context ) ); break;
        case 'c' : return cos( eval_cursor( c.children(0) , context ) ); break;
        case '+' : return eval_cursor( c.children(0) , context ) + eval_cursor( c.children(1) , context ); break;
        case '-' : return eval_cursor( c.children(0) , context ) - eval_cursor( c.children(1) , context ); break;
        case '*' : return eval_cursor( c.children(0) , context ) * eval_cursor( c.children(1) , context ); break;
        case '/' : return eval_cursor( c.children(0) , context ) / eval_cursor( c.children(1) , context ); break;
    }
}

template< typename Cursor >
inline value_type eval_cursor2( Cursor const &c , context_type const &context )
{
    if( c.size() == 0 )
    {
        switch( *c )
        {
            case 'x' : return context[0]; break;
            case 'y' : return context[1]; break;
            case 'z' : return context[2]; break;
        }
    }
    else if( c.size() == 1 )
    {
        switch( *c )
        {
            case 'e' : return exp( eval_cursor( c.children(0) , context ) ); break;
            case 'l' : return my_log( eval_cursor( c.children(0) , context ) ); break;
            case 's' : return sin( eval_cursor( c.children(0) , context ) ); break;
            case 'c' : return cos( eval_cursor( c.children(0) , context ) ); break;
        }
    }
    else if( c.size() == 2 )
    {
        switch( *c )
        {
            case '+' : return eval_cursor( c.children(0) , context ) + eval_cursor( c.children(1) , context ); break;
            case '-' : return eval_cursor( c.children(0) , context ) - eval_cursor( c.children(1) , context ); break;
            case '*' : return eval_cursor( c.children(0) , context ) * eval_cursor( c.children(1) , context ); break;
            case '/' : return eval_cursor( c.children(0) , context ) / eval_cursor( c.children(1) , context ); break;
        }
    }
}

template< typename Tree >
inline value_type eval_tree( Tree const &tree , context_type const &c )
{
    return eval_cursor( tree.root() , c );
}





/// \return time for evaluation of tree, result sum
template< typename Trees >
std::tuple< double , double > run_test( Trees const &trees , const vector_type &x1 , const vector_type &x2 , const vector_type &x3 )
{
//     auto eval = gpcxx::make_static_eval< value_type , symbol_type , context_type >(
//         fusion::make_vector(
//             fusion::make_vector( 'x' , []( context_type const& t ) { return t[0]; } )
//           , fusion::make_vector( 'y' , []( context_type const& t ) { return t[1]; } )
//           , fusion::make_vector( 'z' , []( context_type const& t ) { return t[2]; } )          
//           ) ,
//         fusion::make_vector(
//             fusion::make_vector( 's' , []( double v ) -> double { return std::sin( v ); } )
//           , fusion::make_vector( 'c' , []( double v ) -> double { return std::cos( v ); } ) 
//           , fusion::make_vector( 'e' , []( double v ) -> double { return std::exp( v ); } ) 
//           , fusion::make_vector( 'l' , []( double v ) -> double { return ( std::abs( v ) < 1.0e-20 ) ? 0.0 : std::log( std::abs( v ) ); } ) 
//           ) ,
//         fusion::make_vector(
//             fusion::make_vector( '+' , std::plus< double >() )
//           , fusion::make_vector( '-' , std::minus< double >() )
//           , fusion::make_vector( '*' , std::multiplies< double >() ) 
//           , fusion::make_vector( '/' , std::divides< double >() ) 
//           ) );

    std::tuple< double , double > res;

    size_t number_of_datapoints = x1.size();
    size_t number_of_trees = trees.size();
    std::vector< vector_type > y( number_of_trees );
    for( size_t t=0 ; t<number_of_trees ; ++t ) y[t] = vector_type( number_of_datapoints );

    // 
    // EVALUATION
    //
    gpcxx::timer timer;
    for( size_t t=0 ; t<number_of_trees ; ++t )
    {
        for( size_t i=0 ; i<number_of_datapoints ; ++i )
        {
            context_type c { x1[i] , x2[i] , x3[i] };
            y[t][i] = eval_tree( trees[t] , c );
        }
    }
    std::get< 0 >( res ) = timer.seconds();

    double sum = 0.0;
    for( size_t t=0 ; t<number_of_trees ; ++t )
    {
        for( size_t i=0 ; i<number_of_datapoints ; ++i )
            sum += y[t][i];
    }
    std::get< 1 >( res ) = sum;

    return res;
}



int main( int argc , char *argv[] )
{
    typedef gpcxx::basic_tree< char > tree_type;

    if( argc != 2 )
    {
        cerr << "usage : " << argv[0] << " infile" << endl;
        return -1;
    }

    size_t number_of_datapoints = 1024 ;

    vector_type x1, x2 , x3;

    // generate test data
    generate_test_data( x1 , x2 , x3 , -5.0 , 5.0 + 0.1 , 0.4 );

    std::vector< tree_type > trees;

    // read trees
    ifstream fin( argv[1] );
    std::string line;
    while( std::getline( fin , line ) )
    {
        trees.push_back( tree_type() );
        parser::parse_tree( line , trees.back() );
    }


    cout.precision( 14 );
    cout << "Starting test " << endl;
    auto times = run_test( trees , x1 , x2 , x3 );
    cout << tab << "Finished!" << endl;
    cout << tab << "Evaluation time " << std::get< 0 >( times ) << endl;
    cout << tab << "Result sum " << std::get< 1 >( times ) << endl << endl;

    return 0;
}
