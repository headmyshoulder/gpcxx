/*
 * eval_basic_virtual.cpp
 * Date: 2013-05-10
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#define FUSION_MAX_VECTOR_SIZE 20

#include "parser.hpp"
#include "nodes.hpp"
#include "generate_data.hpp"

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

typedef i_node< double , context_type > node_type;












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
            y[t][i] = trees[t]( c );
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

template< typename T , typename Context >
struct tree_creator : public boost::static_visitor< void >
{
    typedef i_node< T , Context > node_type;
    
    tree_creator( node_type &node ) : m_node( node ) { }
        
    void operator()( boost::spirit::qi::info::nil ) const {}
        
    void operator()( char n ) const
    {
        switch( n )
        {
            case 'x' : m_node = terminal_node< 0 , T , Context >(); break;
            case 'y' : m_node = terminal_node< 1 , T , Context >(); break;
            case 'z' : m_node = terminal_node< 2 , T , Context >(); break;
        }
    }

    void operator()( parser::expression_ast const& ast ) const
    {
        boost::apply_visitor( *this , ast.expr );
    }

    void operator()( parser::binary_op const& expr ) const
    {
        node_type left , right;
        boost::apply_visitor( tree_creator( left ) , expr.left.expr );
        boost::apply_visitor( tree_creator( right ) , expr.right.expr );

        switch( expr.op )
        {
            case '+' : m_node = plus_node< T , Context >( left , right ); break;
            case '-' : m_node = minus_node< T , Context >( left , right ); break;
            case '*' : m_node = multiplies_node< T , Context >( left , right ); break;
            case '/' : m_node = divides_node< T , Context >( left , right ); break;
        }
    }

    void operator()( parser::unary_op const& expr ) const
    {
        node_type child;
        boost::apply_visitor( tree_creator( child ) , expr.subject.expr );

        switch( expr.op )
        {
            case 's' : m_node = sin_node< T , Context >( child ); break;
            case 'c' : m_node = cos_node< T , Context >( child ); break;
            case 'e' : m_node = exp_node< T , Context >( child ); break;
            case 'l' : m_node = log_node< T , Context >( child ); break;
        }
    }

    node_type &m_node;
};

void run_tree_type( std::string const &name , vector_type const &x1 , vector_type const &x2 , vector_type const &x3 , std::string const & filename )
{
    std::vector< node_type > trees;

    // read trees
    ifstream fin( filename );
    std::string line;
    while( std::getline( fin , line ) )
    {
        node_type tree;
        tree_creator< value_type , context_type > trafo( tree );
        parser::parse_tree( line , trafo );
        trees.push_back( tree );
        
//         parser::ast_print trafo2;
//         parser::parse_tree( line , trafo2 );
//         break;
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
    run_tree_type( "virtual tree" , x1 , x2 , x3 , argv[1] );


    return 0;
}
