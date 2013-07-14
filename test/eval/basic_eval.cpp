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
              fusion::make_vector( "plus" , 2 , std::plus< double >() )
            , fusion::make_vector( "minus" , 2 , std::minus< double >() ) 
            , fusion::make_vector( "sin" , 1 , []( double v ) -> double { return std::sin( v ); } ) 
            , fusion::make_vector( "x" , 0 , []( context_type const& t ) { return t[0]; } )
            , fusion::make_vector( "y" , 0 , []( context_type const& t ) { return t[1]; } )
            , fusion::make_vector( "1" , 0 , []( context_type const& t ) { return 1.0; } )
            , fusion::make_vector( "2" , 0 , []( context_type const& t ) { return 1.0; } )
        )
    );
    
    context_type context = {{ 1.5 , 2.5 }};
    double val1 = eval( trees.data , context );
    double val2 = eval( trees.data2 , context );
    
    EXPECT_DOUBLE_EQ( val1 , 1.0 );
    EXPECT_DOUBLE_EQ( val1 , 2.0 );
}

