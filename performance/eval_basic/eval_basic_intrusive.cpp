/*
 * eval_basic_intrusive.cpp
 * Date: 2013-05-10
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#define FUSION_MAX_VECTOR_SIZE 20

#include "parser.hpp"
#include "generate_data.hpp"

#include <gpcxx/io/simple.hpp>
#include <gpcxx/tree/intrusive_tree.hpp>
#include <gpcxx/tree/intrusive_func_node.hpp>
#include <gpcxx/tree/intrusive_functions.hpp>
#include <gpcxx/app/timer.hpp>

#include <iostream>
#include <functional>
#include <fstream>
#include <vector>
#include <random>
#include <cmath>
#include <array>

const std::string tab = "\t";

using namespace std;
using namespace parser;
namespace fusion = boost::fusion;


using value_type = double;
using vector_type = std::vector< value_type >;
using rng_type = std::mt19937;

using context_type = std::array< double , 3 >;
using node_type = gpcxx::intrusive_func_node< double , context_type >;

using terminal_x = gpcxx::array_terminal< 0 >;
using terminal_y = gpcxx::array_terminal< 1 >;
using terminal_z = gpcxx::array_terminal< 2 >;











/// \return time for evaluation of tree, result sum
template< typename Trees >
std::tuple< double , double > run_test( Trees const &trees , const vector_type &x1 , const vector_type &x2 , const vector_type &x3 )
{
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
            context_type c { { x1[i] , x2[i] , x3[i] } };
            y[t][i] = trees[t].root()->eval( c );
            if( fabs( y[t][i] ) > 1.0e5 ) y[t][i] = 1.0e5;
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


template< typename Tree >
struct tree_transformator2 : public boost::static_visitor< void >
{
    typedef Tree tree_type;
    typedef typename Tree::cursor cursor;
    
    
    tree_transformator2( tree_type &tree , cursor c ) : tree_( tree ) , c_( c ) { }
    

    void operator()( nil ) const {}
    
    void operator()( char n ) const
    {
        switch( n )
        {
            case 'x' : tree_.insert_below( c_ , node_type( terminal_x() ) ); break;
            case 'y' : tree_.insert_below( c_ , node_type( terminal_y() ) ); break;
            case 'z' : tree_.insert_below( c_ , node_type( terminal_z() ) ); break;
        }
    }

    void operator()( expression_ast const& ast ) const
    {
        boost::apply_visitor( *this , ast.expr );
    }

    void operator()( binary_op const& expr ) const
    {
        cursor c1;
        switch( expr.op )
        {
            case '+' : c1 = tree_.insert_below( c_ , node_type( gpcxx::plus_func() ) ); break;
            case '-' : c1 = tree_.insert_below( c_ , node_type( gpcxx::minus_func() ) ); break;
            case '*' : c1 = tree_.insert_below( c_ , node_type( gpcxx::multiplies_func() ) ); break;
            case '/' : c1 = tree_.insert_below( c_ , node_type( gpcxx::divides_func() ) ); break;
        }
        boost::apply_visitor( tree_transformator2( tree_ , c1 ) , expr.left.expr );
        boost::apply_visitor( tree_transformator2( tree_ , c1 ) , expr.right.expr );
    }

    void operator()( unary_op const& expr ) const
    {
        cursor c1;
        switch( expr.op )
        {
            case 's' : c1 = tree_.insert_below( c_ , node_type( gpcxx::sin_func() ) ); break;
            case 'c' : c1 = tree_.insert_below( c_ , node_type( gpcxx::cos_func() ) ); break;
            case 'e' : c1 = tree_.insert_below( c_ , node_type( gpcxx::exp_func() ) ); break;
            case 'l' : c1 = tree_.insert_below( c_ , node_type( gpcxx::log_func() ) ); break;
        }
        boost::apply_visitor( tree_transformator2( tree_ , c1 ) , expr.subject.expr );
    }

    tree_type &tree_;
    cursor c_;
};


void run_tree_type( std::string const &name , vector_type const &x1 , vector_type const &x2 , vector_type const &x3 , std::string const & filename )
{
    typedef gpcxx::intrusive_tree< node_type > tree_type;
    std::vector< tree_type > trees;

    // read trees
    ifstream fin( filename );
    std::string line;
    while( std::getline( fin , line ) )
    {
        tree_type tree;
        trees.push_back( tree );
        tree_transformator2< tree_type > trafo( trees.back() , trees.back().root() );
        parser::parse_tree( line , trafo );
    }

    cout.precision( 14 );
    cout << "Starting test " << name << endl;
    auto times = run_test( trees , x1 , x2 , x3 );
    cout << tab << "Finished!" << endl;
    cout << tab << "Evaluation time " << std::get< 0 >( times ) << endl;
    cout << tab << "Result sum " << std::get< 1 >( times ) << endl << endl;

}



int main( int argc , char *argv[] )
{
    if( argc != 2 )
    {
        cerr << "usage : " << argv[0] << " infile" << endl;
        return -1;
    }

    // generate test data
    vector_type x1, x2 , x3;
    generate_test_data( x1 , x2 , x3 , -5.0 , 5.0 + 0.1 , 0.4 );

    // run test for several tree tests
    run_tree_type( "intrusive tree" , x1 , x2 , x3 , argv[1] );


    return 0;
}
