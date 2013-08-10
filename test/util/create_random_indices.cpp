/*
 * create_random_indices.cpp
 * Date: 2013-02-20
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include <gpcxx/util/create_random_indices.hpp>

#include <gtest/gtest.h>

using namespace std;

#define TESTNAME create_random_indices_tests

TEST( TESTNAME , create1 )
{
    std::vector< size_t >  ind;
    gpcxx::create_random_indices( ind , 10 , 5 );
    EXPECT_EQ( ind.size() , 5 );
}

TEST( TESTNAME , create2 )
{
    std::vector< size_t >  ind;
    gpcxx::create_random_indices( ind , 10 , 10 );
    EXPECT_EQ( ind.size() , 10 );
}

