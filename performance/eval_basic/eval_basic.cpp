/*
 * eval_basic.cpp
 * Date: 2013-05-10
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#define FUSION_MAX_VECTOR_SIZE 20

#include "parser.hpp"
#include "generate_data.hpp"

#include <gpcxx/eval/static_eval.hpp>
#include <gpcxx/generate/basic_generate_strategy.hpp>
#include <gpcxx/generate/uniform_symbol.hpp>
#include <gpcxx/io/simple.hpp>
#include <gpcxx/tree/basic_tree.hpp>
#include <gpcxx/app/timer.hpp>
#include <gpcxx/stat/node_statistics.hpp>

#include <boost/fusion/include/make_vector.hpp>

#include <iostream>
#include <functional>
#include <fstream>
#include <vector>
#include <random>
#include <cmath>
#include <array>

using namespace std;
namespace fusion = boost::fusion;

typedef double value_type;
typedef std::vector< value_type > vector_type;
typedef std::array< double , 3 > context_type;
typedef char symbol_type;
typedef std::mt19937 rng_type ;


const std::string tab = "\t";

inline value_type my_log( value_type v )
{
    return ( std::abs( v ) < 1.0e-20 ) ? 0.0 : std::log( std::abs( v ) );
}

struct my_div
{
    template< typename T >
    inline T operator()( T const& t1 , T const &t2 ) const
    {
        if( t2 == 0.0 ) return 1.0;
        else return t1 / t2;
    }
};

double objective_function( double x1 , double x2 , double x3 )
{
    return  1.0 / ( 1.0 + pow( x1 , -4.0 ) ) + 1.0 / ( 1.0 + pow( x2 , -4.0 ) ) + 1.0 / ( 1.0 + pow( x3 , -4.0 ) );
}

struct eval_cursor1
{
    template< typename Cursor >
    static inline value_type eval_cursor( Cursor const& c , context_type const &context )
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
            case '/' : return my_div()( eval_cursor( c.children(0) , context ) , eval_cursor( c.children(1) , context ) ); break;
        }
        return value_type( 0.0 );
    }
    
    template< typename Tree >
    inline value_type operator()( Tree const &t , context_type const &context ) const
    {
        return eval_cursor1::eval_cursor( t.root() , context );
    }
};



struct eval_cursor2
{
    template< typename Cursor >
    inline static value_type eval_cursor( Cursor const &c , context_type const &context )
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
    inline value_type operator()( Tree const &t , context_type const &context ) const
    {
        return eval_cursor( t.root() , context );
    }
};



template< typename Cursor >
struct eval_cursor3
{
    typedef Cursor cursor_type;
    typedef eval_cursor3< cursor_type > evaluator;
    
    
    static inline double eval_x( cursor_type const &c , context_type const& context ) { return context[0]; }
    static inline double eval_y( cursor_type const &c , context_type const& context ) { return context[0]; }
    static inline double eval_z( cursor_type const &c , context_type const& context ) { return context[0]; }
    static inline double eval_sin( cursor_type const &c , context_type const& context ) { return sin( eval_cursor( c.children(0) , context ) ); }
    static inline double eval_cos( cursor_type const &c , context_type const& context ) { return cos( eval_cursor( c.children(0) , context ) ); }
    static inline double eval_exp( cursor_type const &c , context_type const& context ) { return exp( eval_cursor( c.children(0) , context ) ); }
    static inline double eval_log( cursor_type const &c , context_type const& context ) { return my_log( eval_cursor( c.children(0) , context ) ); }
    static inline double eval_plus( cursor_type const &c , context_type const& context )
    { return  eval_cursor( c.children(0) , context ) + eval_cursor( c.children(1) , context ); }
    static inline double eval_minus( cursor_type const &c , context_type const& context )
    { return eval_cursor( c.children(0) , context ) - eval_cursor( c.children(1) , context ); }
    static inline double eval_multiplies( cursor_type const &c , context_type const& context )
    { return eval_cursor( c.children(0) , context ) * eval_cursor( c.children(1) , context ); }
    static inline double eval_divides( cursor_type const &c , context_type const& context )
    { return eval_cursor( c.children(0) , context ) / eval_cursor( c.children(1) , context ); }
    
    typedef double( *func_type )( cursor_type const& , context_type const& );
    typedef std::array< func_type , 128 > lookup_table_type;
    static lookup_table_type const& get_table( void )
    {
        static lookup_table_type tbl;
        std::fill( tbl.begin() , tbl.end() , nullptr );
        tbl[ size_t( 'x' ) ] = &evaluator::eval_x;
        tbl[ size_t( 'y' ) ] = &evaluator::eval_y;
        tbl[ size_t( 'z' ) ] = &evaluator::eval_z;
        tbl[ size_t( 's' ) ] = &evaluator::eval_sin;
        tbl[ size_t( 'c' ) ] = &evaluator::eval_cos;
        tbl[ size_t( 'e' ) ] = &evaluator::eval_exp;
        tbl[ size_t( 'l' ) ] = &evaluator::eval_log;
        tbl[ size_t( '+' ) ] = &evaluator::eval_plus;
        tbl[ size_t( '-' ) ] = &evaluator::eval_minus;
        tbl[ size_t( '*' ) ] = &evaluator::eval_multiplies;
        tbl[ size_t( '/' ) ] = &evaluator::eval_divides;
        return tbl;
    }
    
    static double eval_cursor( cursor_type const &cursor , context_type const& c ) 
    {
        lookup_table_type const& tbl = get_table();
        func_type f = tbl[ size_t( *cursor ) ];
        return (*f)( cursor , c );
    }
    
    template< typename Tree >
    double operator()( Tree const& t , context_type const & c ) const
    {
        return eval_cursor( t.root() , c );
    }
};


auto eval_cursor4 = gpcxx::make_static_eval< value_type , symbol_type , context_type >(
    fusion::make_vector(
        fusion::make_vector( 'x' , []( context_type const& t ) { return t[0]; } )
        , fusion::make_vector( 'y' , []( context_type const& t ) { return t[1]; } )
        , fusion::make_vector( 'z' , []( context_type const& t ) { return t[2]; } )
        ) ,
    fusion::make_vector(
        fusion::make_vector( 's' , []( double v ) -> double { return std::sin( v ); } )
        , fusion::make_vector( 'c' , []( double v ) -> double { return std::cos( v ); } )
        , fusion::make_vector( 'e' , []( double v ) -> double { return std::exp( v ); } )
        , fusion::make_vector( 'l' , []( double v ) -> double { return my_log( v ); } )
        ) ,
    fusion::make_vector(
        fusion::make_vector( '+' , std::plus< double >() )
        , fusion::make_vector( '-' , std::minus< double >() )
        , fusion::make_vector( '*' , std::multiplies< double >() ) 
        , fusion::make_vector( '/' , std::divides< double >() ) 
        ) );






/// \return time for evaluation of tree, result sum
template< typename Evaluator , typename Trees >
std::tuple< double , double > run_test( Evaluator const &eval , Trees const &trees , const vector_type &x1 , const vector_type &x2 , const vector_type &x3 , std::vector< double > &fitness )
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
            y[t][i] = eval( trees[t] , c );
        }
    }
    std::get< 0 >( res ) = timer.seconds();

    double sum = 0.0;
    
    for( size_t t=0 ; t<number_of_trees ; ++t )
    {
        double chi2 = 0.0;
        for( size_t i=0 ; i<number_of_datapoints ; ++i )
        {
            sum += y[t][i];
            double diff = y[t][i] - objective_function( x1[i] , x2[i] , x3[i] );
            chi2 += std::abs( diff );
        }
        chi2 /= double( x1.size() );
        fitness.push_back( 1.0 / ( 1.0 + chi2 ) );
    }
    std::get< 1 >( res ) = sum;

    return res;
}

template< typename Tree , typename Evaluator >
void run_tree_type( std::string const &name , Evaluator const& eval , vector_type const &x1 , vector_type const &x2 , vector_type const &x3 , std::string const & filename )
{
    std::vector< Tree > trees;

    // read trees
    ifstream fin( filename );
    std::string line;
    while( std::getline( fin , line ) )
    {
        Tree tree;
        trees.push_back( tree );
        parser::tree_transformator< Tree > trafo( trees.back() , trees.back().root() );
        parser::parse_tree( line , trafo );
    }
    
    std::vector< double > fitness;

    cout.precision( 14 );
    cout << "Starting test " << name << endl;
    auto times = run_test( eval , trees , x1 , x2 , x3 , fitness );
    cout << tab << "Finished!" << endl;
    cout << tab << "Evaluation time " << std::get< 0 >( times ) << endl;
    cout << tab << "Result sum " << std::get< 1 >( times ) << endl << endl;
    
    std::ofstream fout( std::string( "eval_" ) + name + ".dat" );
    fout.precision( 14 );
    for( size_t i=0 ; i<fitness.size() ; ++i )
    {
        gpcxx::node_statistics stat = gpcxx::calc_node_statistics_tree( trees[i] );
        
        fout << i;
        fout << tab << fitness[i];
        fout << tab << trees[i].root().height();
        fout << tab << stat.num_nodes;
        fout << tab << gpcxx::simple( trees[i] );
        fout << "\n";
    }
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
    // generate_test_data( x1 , x2 , x3 , -5.0 , 5.0 + 0.1 , 1.0 );
    generate_test_data( x1 , x2 , x3 , -5.0 , 5.0 + 0.1 , 0.4 );

    // run test for several tree tests
    run_tree_type< gpcxx::basic_tree< char > >( "basic_tree_eval1" , eval_cursor1() , x1 , x2 , x3 , argv[1] );
//     run_tree_type< gpcxx::basic_tree< char > >( "basic_tree_eval2" , eval_cursor2() , x1 , x2 , x3 , argv[1] );
//     run_tree_type< gpcxx::basic_tree< char > >( "basic_tree_eval3" , eval_cursor3< gpcxx::basic_tree< char >::const_cursor >() , x1 , x2 , x3 , argv[1] );
//     run_tree_type< gpcxx::basic_tree< char > >( "basic_tree_eval4" , eval_cursor4 , x1 , x2 , x3 , argv[1] );

    return 0;
}
