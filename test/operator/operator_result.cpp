/*
 * test/operator/operator_result.cpp
 * Date: 2015-03-23
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/operator/operator_result.hpp>

#include <gtest/gtest.h>

#define TESTNAME operator_result_tests

using namespace std;
using namespace gpcxx;

TEST( TESTNAME , construction_from_lvalue )
{
    std::vector< int > vec = { 1 , 2 , 3 , 10 , 3 };
    auto iter = vec.begin() + 2;
    auto res = make_operator_result( *iter , iter );
    EXPECT_EQ( res.iterator , vec.begin() + 2 );
    EXPECT_EQ( res.individuum , 3 );
}

TEST( TESTNAME , construction_from_rvalue )
{
    std::vector< int > vec = { 1 , 2 , 3 , 10 , 3 };
    auto iter = vec.begin() + 2;
    auto value = *iter;
    auto res = make_operator_result( std::move( value ) , iter );
    EXPECT_EQ( res.iterator , vec.begin() + 2 );
    EXPECT_EQ( res.individuum , 3 );
}

TEST( TESTNAME , casts )
{
    std::vector< int > vec = { 1 , 2 , 3 , 10 , 3 };
    auto iter = vec.begin() + 2;
    auto value = *iter;
    auto res = make_operator_result( std::move( value ) , iter );
    EXPECT_EQ( 3 , int( res ) );
    EXPECT_EQ( 3 , static_cast< int >( res ) );
}

