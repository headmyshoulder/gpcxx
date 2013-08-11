/*
 test/eval/static_eval_erc.cpp

 Copyright 2013 Karsten Ahnert

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/eval/static_eval_erc.hpp>
#include <gpcxx/tree/basic_tree.hpp>

#include <boost/fusion/include/make_vector.hpp>
#include <boost/math/constants/constants.hpp>

#include <gtest/gtest.h>

#include <array>
#include <sstream>
#include <cmath>
#include <random>

#define TESTNAME basic_eval_erc_tests

using namespace std;
namespace fusion = boost::fusion;

// class basic_eval_erc_test : public ::testing::Test
// {
// protected:
//   
//     virtual void SetUp( void )
//     {
//     }
//     
//     virtual void TearDown( void )
//     {
//     }
// };

TEST( TESTNAME , TestCase )
{
    typedef double value_type;
    typedef std::array< double , 2 > context_type;
    typedef std::string symbol_type;
    
    
    auto eval = gpcxx::make_static_eval_erc< value_type , symbol_type , context_type >(
        fusion::make_vector( 1.0 , std::normal_distribution<>( 0.0 , 1.0 ) ) ,
        fusion::make_vector(
                 fusion::make_vector( "1" , []( context_type const& t ) { return 1.0; } )
               , fusion::make_vector( "2" , []( context_type const& t ) { return 2.0; } )
               , fusion::make_vector( "x" , []( context_type const& t ) { return t[0]; } )
               , fusion::make_vector( "y" , []( context_type const& t ) { return t[1]; } )
                ) ,
        fusion::make_vector(
                 fusion::make_vector( "sin" , []( double v ) -> double { return std::sin( v ); } )
               , fusion::make_vector( "cos" , []( double v ) -> double { return std::cos( v ); } ) 
                ) ,
        fusion::make_vector(
                 fusion::make_vector( "plus" , std::plus< double >() )
               , fusion::make_vector( "minus" , std::minus< double >() ) 
                ) );
    
    typedef decltype( eval ) eval_type;
    typedef eval_type::node_attribute_type node_attribute_type;
    typedef gpcxx::basic_tree< node_attribute_type > tree_type;
    
    std::mt19937 rng;
    
    EXPECT_EQ( eval.get_terminal_symbols() , std::vector< std::string >( { "1" , "2" , "x" , "y" } ) );
    EXPECT_EQ( eval.get_unary_symbols() , std::vector< std::string >( { "sin" , "cos" } ) );
    EXPECT_EQ( eval.get_binary_symbols() , std::vector< std::string >( { "plus" , "minus" } ) );
    
    auto terminal_dist = eval.get_terminal_symbol_distribution();
    auto unary_dist = eval.get_unary_symbol_distribution();
    auto binary_dist = eval.get_binary_symbol_distribution();
    
    node_attribute_type v1 = terminal_dist( rng );
    node_attribute_type v2 = unary_dist( rng );
    node_attribute_type v3 = binary_dist( rng );
    
    tree_type tree;
    auto c1 = tree.insert_below( tree.root() , "plus" );
    auto c2 = tree.insert_below( c1 , "sin" );
    auto c3 = tree.insert_below( c2 , "x" );
    auto c4 = tree.insert_below( c1 , "minus" );
    auto c5 = tree.insert_below( c4 , 12.2 );
    auto c6 = tree.insert_below( c4 , "y" );
    
    context_type context = {{ 0.5 * boost::math::double_constants::pi , 2.0  }};
    value_type val1 = eval( tree , context );
    EXPECT_DOUBLE_EQ( val1 , 11.2 );
}

