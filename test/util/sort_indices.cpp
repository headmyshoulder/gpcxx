/*
 * sort_indices.cpp
 * Date: 2013-02-20
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include <gpcxx/util/sort_indices.hpp>
#include <vector>

#include <gtest/gtest.h>

using namespace std;

#define TESTNAME sort_indices_tests

TEST( TESTNAME , sort_indices1 )
{
    std::vector< double > c = { 2.0 , 5.0 , 2.5 , 1.0 , -2.0 , 4.5 };
    std::vector< size_t >  ind;
    gpcxx::sort_indices( c , ind );
    EXPECT_EQ( ind.size() , size_t( 6 ) );
    EXPECT_EQ( ind[0] , size_t( 4 ) );
    EXPECT_EQ( ind[1] , size_t( 3 ) );
    EXPECT_EQ( ind[2] , size_t( 0 ) );
    EXPECT_EQ( ind[3] , size_t( 2 ) );
    EXPECT_EQ( ind[4] , size_t( 5 ) );
    EXPECT_EQ( ind[5] , size_t( 1 ) );
}

TEST( TESTNAME , sort_indices2 )
{
    std::vector< double > c = { 2.0 , 5.0 , 2.5 , NAN , 1.0 , -2.0 , INFINITY , -INFINITY , 4.5 };
    std::vector< size_t >  ind;
    gpcxx::sort_indices( c , ind );
    EXPECT_EQ( ind.size() , size_t( 9 ) );
    EXPECT_EQ( ind[0] , size_t( 5 ) );
    EXPECT_EQ( ind[1] , size_t( 4 ) );
    EXPECT_EQ( ind[2] , size_t( 0 ) );
    EXPECT_EQ( ind[3] , size_t( 2 ) );
    EXPECT_EQ( ind[4] , size_t( 8 ) );
    EXPECT_EQ( ind[5] , size_t( 1 ) );
}
