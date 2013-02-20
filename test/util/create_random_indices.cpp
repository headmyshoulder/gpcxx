/*
 * create_random_indices.cpp
 * Date: 2013-02-20
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include <gp/util/create_random_indices.hpp>

#include <gtest/gtest.h>



using namespace std;

TEST( util_tests , create_random_indices1 )
{
    std::vector< size_t >  ind;
    gp::create_random_indices( ind , 10 , 5 );
    EXPECT_EQ( ind.size() , 5 );
}

TEST( util_tests , create_random_indices2 )
{
    std::vector< size_t >  ind;
    gp::create_random_indices( ind , 10 , 10 );
    EXPECT_EQ( ind.size() , 10 );
}

