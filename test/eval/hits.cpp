/*
 * test/eval/hits.cpp
 * Date: 2015-03-26
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/eval/hits.hpp>

#include <gtest/gtest.h>

#include <vector>

#define TESTNAME hits_tests

using namespace std;

TEST( TESTNAME , hits_single )
{
    EXPECT_TRUE( gpcxx::hits_single( 0.9 , 0.11 ) );
    EXPECT_FALSE( gpcxx::hits_single( 0.9 , 0.09 ) );
}

TEST( TESTNAME , hits )
{
    std::vector< double > adjusted_fitness = { 0.9 , 0.8  , 0.0 , 0.5 , 1.0 , 0.84 , 0.86 };
    std::vector< bool > hits;
    gpcxx::hits( adjusted_fitness , hits , 0.15 );
    EXPECT_EQ( hits.size() , size_t( 7 ) );
    EXPECT_TRUE( hits[0] );
    EXPECT_FALSE( hits[1] );
    EXPECT_FALSE( hits[2] );
    EXPECT_FALSE( hits[3] );
    EXPECT_TRUE( hits[4] );
    EXPECT_FALSE( hits[5] );
    EXPECT_TRUE( hits[6] );
}

TEST( TESTNAME , hits_copy )
{
    std::vector< double > adjusted_fitness = { 0.9 , 0.8  , 0.0 , 0.5 , 1.0 , 0.84 , 0.86 };
    std::vector< bool > hits = gpcxx::hits( adjusted_fitness , 0.15 );
    EXPECT_EQ( hits.size() , size_t( 7 ) );
    EXPECT_TRUE( hits[0] );
    EXPECT_FALSE( hits[1] );
    EXPECT_FALSE( hits[2] );
    EXPECT_FALSE( hits[3] );
    EXPECT_TRUE( hits[4] );
    EXPECT_FALSE( hits[5] );
    EXPECT_TRUE( hits[6] );
}

