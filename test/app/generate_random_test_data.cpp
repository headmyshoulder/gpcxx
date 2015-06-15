/*
 * generate_random_test_data.cpp
 * Date: 2013-02-23
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include <gpcxx/app/generate_random_test_data.hpp>

#include <gtest/gtest.h>
#include <random>

#define TESTNAME generate_random_test_data_tests

using namespace std;

TEST( TESTNAME , generate_1d )
{
    size_t num_of_points = 500;
    std::mt19937 rng;
    
    using param_type = std::array< std::tuple< double , double > , 1 >;
    auto data = gpcxx::generate_random_test_data< 1 , std::uniform_real_distribution< double > >( rng , num_of_points ,
        param_type( {{ std::make_tuple( 2.5 , 2.8 ) }} ),
        []( double x ) { return x; } );
    ASSERT_EQ( size_t( num_of_points ) , data.x[0].size() );
    ASSERT_EQ( size_t( num_of_points ) , data.y.size() );
    for( size_t i=0 ; i<num_of_points ; ++i )
    {
        EXPECT_GT( data.x[0][i] , 2.5 );
        EXPECT_GT( data.y[i] , 2.5 );
        EXPECT_DOUBLE_EQ( data.x[0][i] , data.y[i] );
        EXPECT_LT( data.x[0][i] , 2.8 );
        EXPECT_LT( data.y[i] , 2.8 );
    }
}

TEST( TESTNAME , generate_2d )
{
    size_t num_of_points = 500;
    std::mt19937 rng;
    
    using param_type = std::array< std::tuple< double , double > , 2 >;
    auto data = gpcxx::generate_random_test_data< 2 , std::uniform_real_distribution< double > >( rng , num_of_points ,
        param_type( {{ std::make_tuple( 2.5 , 2.8 ) ,
                       std::make_tuple( -1.35 , -1.3 ) }} ),
        []( double x , double y ) { return x + y; } );
    ASSERT_EQ( size_t( num_of_points ) , data.x[0].size() );
    ASSERT_EQ( size_t( num_of_points ) , data.y.size() );
    for( size_t i=0 ; i<num_of_points ; ++i )
    {
        EXPECT_GT( data.x[0][i] , 2.5 );
        EXPECT_GT( data.x[1][i] , -1.35 );
        EXPECT_DOUBLE_EQ( data.x[0][i] + data.x[1][i] , data.y[i] );
        EXPECT_LT( data.x[0][i] , 2.8 );
        EXPECT_LT( data.x[1][i] , -1.3 );
        
    }
}

