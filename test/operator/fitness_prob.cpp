/*
 * fitness_prob.cpp
 * Date: 2013-02-21
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include <gp/operator/fitness_prob.hpp>

#include <gtest/gtest.h>

using namespace std;

TEST( operator_tests , fitness_prob1 )
{
    std::mt19937 rng;
    std::vector< double > fitness = { 5.0 , 2.0 , 4.0 , 1.0 , 7.0 };

    gp::fitness_prob< std::vector< double > , std::mt19937 > prob( fitness , rng );
    EXPECT_EQ( prob.indices().size() , 5 );
    EXPECT_EQ( prob.indices()[0] , 3 );
    EXPECT_EQ( prob.indices()[1] , 1 );
    EXPECT_EQ( prob.indices()[2] , 2 );
    EXPECT_EQ( prob.indices()[3] , 0 );
    EXPECT_EQ( prob.indices()[4] , 4 );

    for( size_t i=0 ; i<1000 ; ++i )
    {
        size_t ind = prob.random_index();
        EXPECT_TRUE( ind < 5 );
    }

}

TEST( operator_tests , fitness_prob2 )
{
    std::mt19937 rng;
    std::vector< double > fitness = { 5.0 , 2.0 , NAN , 1.0 , 7.0 };

    gp::fitness_prob< std::vector< double > , std::mt19937 > prob( fitness , rng );
    EXPECT_EQ( prob.indices().size() , 5 );
    EXPECT_EQ( prob.indices()[0] , 3 );
    EXPECT_EQ( prob.indices()[1] , 1 );
    EXPECT_EQ( prob.indices()[2] , 0 );
    EXPECT_EQ( prob.indices()[3] , 4 );
    EXPECT_EQ( prob.indices()[4] , 2 );

    for( size_t i=0 ; i<1000 ; ++i )
    {
        size_t ind = prob.random_index();
        EXPECT_TRUE( ind < 4 );
    }
}

