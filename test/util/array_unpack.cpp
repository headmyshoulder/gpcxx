/*
 test/util/array_unpack.cpp

 Copyright 2013 Karsten Ahnert

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/util/array_unpack.hpp>

#include <gtest/gtest.h>

#include <sstream>

#define TESTNAME array_unpack_tests

using namespace std;

struct mock_functor
{
    double operator()( double a1 ) const { return a1 * 2.0 ; }
    double operator()( double a1 , double a2 ) const { return ( a1 + a2 ) * 2.0; }
    double operator()( double a1 , double a2 , double a3 ) const { return ( a1 + a2 + a3 ) * 0.25; }
};

TEST( TESTNAME , unpack1 )
{
    mock_functor f;
    std::array< double , 1 > arr = {{ 2.2 }};
    EXPECT_DOUBLE_EQ( 4.4 , gpcxx::array_unpack( arr , f ) );
}

TEST( TESTNAME , unpack2 )
{
    mock_functor f;
    std::array< double , 2 > arr = {{ 2.2 , 4.4 }};
    EXPECT_DOUBLE_EQ( 13.2 , gpcxx::array_unpack( arr , f ) );
}

TEST( TESTNAME , unpack3 )
{
    mock_functor f;
    std::array< double , 3 > arr = {{ 2.0 , 3.5 , 1.5 }};
    EXPECT_DOUBLE_EQ( 1.75 , gpcxx::array_unpack( arr , f ) );
}
