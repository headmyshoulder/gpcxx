/*
 test/eval/basic_eval.cpp

 Copyright 2013 Karsten Ahnert

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gp/eval/basic_eval.hpp>

#include "../common/test_tree.hpp"

#include <boost/fusion/include/make_vector.hpp>
#include <boost/math/constants/constants.hpp>

#include <gtest/gtest.h>

#include <array>
#include <sstream>
#include <cmath>
#include <functional>

#define TESTNAME basic_eval_tests

using namespace std;

namespace fusion = boost::fusion;






TEST( TESTNAME , test1 )
{
    typedef double value_type;
    typedef std::array< double , 2 > context_type;
    typedef std::string attribute_type;
    
    test_tree< basic_tree_tag > trees;
    
    auto eval = gp::make_basic_eval< value_type , context_type , attribute_type >(
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

    
    context_type context1 = {{ 0.5 * boost::math::double_constants::pi , 3.5 }};
    double val1 = eval( trees.data , context1 );
    EXPECT_DOUBLE_EQ( val1 , 2.5 );
    
    context_type context2 = {{ 5.0 , boost::math::double_constants::pi }};
    double val2 = eval( trees.data2 , context2 );
    EXPECT_DOUBLE_EQ( val2 , -6.0 );
}



TEST( TESTNAME , test2 )
{
    typedef double value_type;
    typedef std::array< double , 2 > context_type;
    typedef std::string attribute_type;
    
    test_tree< basic_tree_tag > trees;
    
    auto eval = gp::make_basic_eval< value_type , context_type , attribute_type >(
        fusion::make_vector(
                 fusion::make_vector( "1" , []( context_type const& t ) { return 1.0; } )
               , fusion::make_vector( "2" , []( context_type const& t ) { return 2.0; } )
//               , fusion::make_vector( "x" , []( context_type const& t ) { return t[0]; } )
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

    
    
    context_type context1 = {{ 0.5 * boost::math::double_constants::pi , 3.5 }};
    EXPECT_THROW( 
        double val1 = eval( trees.data , context1 )
        , std::runtime_error );
    
    
    context_type context2 = {{ 5.0 , boost::math::double_constants::pi }};
    EXPECT_THROW(
        double val2 = eval( trees.data2 , context2 )
        , std::runtime_error );
    
}

