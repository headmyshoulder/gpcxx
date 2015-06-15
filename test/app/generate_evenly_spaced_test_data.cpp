/*
 * generate_evenly_spaced_test_data.cpp
 * Date: 2013-02-23
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include <gpcxx/app/generate_evenly_space_test_data.hpp>

#include <gtest/gtest.h>

#define TESTNAME generate_evenly_spaced_test_data_tests

using namespace std;

TEST( TESTNAME , generate_1d )
{
    auto data = gpcxx::generate_evenly_spaced_test_data< 1 >( {{ std::make_tuple( -1.0 , 1.0 , 0.5 ) }} , []( double x ) { return x; } );
    ASSERT_EQ( size_t( 5 ) , data.x[0].size() );
    ASSERT_EQ( size_t( 5 ) , data.y.size() );
    EXPECT_DOUBLE_EQ( -1.0 , data.x[0][0] );
    EXPECT_DOUBLE_EQ( -1.0 , data.y[0] );
    EXPECT_DOUBLE_EQ( -0.5 , data.x[0][1] );
    EXPECT_DOUBLE_EQ( -0.5 , data.y[1] );
    EXPECT_DOUBLE_EQ( -0.0 , data.x[0][2] );
    EXPECT_DOUBLE_EQ( -0.0 , data.y[2] );
    EXPECT_DOUBLE_EQ(  0.5 , data.x[0][3] );
    EXPECT_DOUBLE_EQ(  0.5 , data.y[3] );
    EXPECT_DOUBLE_EQ(  1.0 , data.x[0][4] );
    EXPECT_DOUBLE_EQ(  1.0 , data.y[4] );
}

TEST( TESTNAME , generate_1d_2 )
{
    auto data = gpcxx::generate_evenly_spaced_test_data< 1 >( -1.0 , 1.0 , 0.5 , []( double x ) { return x; } );
    ASSERT_EQ( size_t( 5 ) , data.x[0].size() );
    ASSERT_EQ( size_t( 5 ) , data.y.size() );
    EXPECT_DOUBLE_EQ( -1.0 , data.x[0][0] );
    EXPECT_DOUBLE_EQ( -1.0 , data.y[0] );
    EXPECT_DOUBLE_EQ( -0.5 , data.x[0][1] );
    EXPECT_DOUBLE_EQ( -0.5 , data.y[1] );
    EXPECT_DOUBLE_EQ( -0.0 , data.x[0][2] );
    EXPECT_DOUBLE_EQ( -0.0 , data.y[2] );
    EXPECT_DOUBLE_EQ(  0.5 , data.x[0][3] );
    EXPECT_DOUBLE_EQ(  0.5 , data.y[3] );
    EXPECT_DOUBLE_EQ(  1.0 , data.x[0][4] );
    EXPECT_DOUBLE_EQ(  1.0 , data.y[4] );
}

TEST( TESTNAME , generate_2d )
{
    auto data = gpcxx::generate_evenly_spaced_test_data< 2 >( {{
        std::make_tuple( -1.0 , 1.0 , 0.5 ) ,
        std::make_tuple( -1.0 , 1.0 , 2.0 ) }} ,
        []( double x , double y ) { return x + y; } );
    ASSERT_EQ( size_t( 10 ) , data.x[0].size() );
    ASSERT_EQ( size_t( 10 ) , data.x[1].size() );
    ASSERT_EQ( size_t( 10 ) , data.y.size() );

    EXPECT_DOUBLE_EQ( -1.0 , data.x[0][0] );
    EXPECT_DOUBLE_EQ( -1.0 , data.x[1][0] );
    EXPECT_DOUBLE_EQ( -2.0 , data.y[0] );
    
    EXPECT_DOUBLE_EQ( -1.0 , data.x[0][1] );
    EXPECT_DOUBLE_EQ(  1.0 , data.x[1][1] );
    EXPECT_DOUBLE_EQ(  0.0 , data.y[1] );

    EXPECT_DOUBLE_EQ( -0.5 , data.x[0][2] );
    EXPECT_DOUBLE_EQ( -1.0 , data.x[1][2] );
    EXPECT_DOUBLE_EQ( -1.5 , data.y[2] );
    
    EXPECT_DOUBLE_EQ( -0.5 , data.x[0][3] );
    EXPECT_DOUBLE_EQ(  1.0 , data.x[1][3] );
    EXPECT_DOUBLE_EQ(  0.5 , data.y[3] );

    EXPECT_DOUBLE_EQ(  0.0 , data.x[0][4] );
    EXPECT_DOUBLE_EQ( -1.0 , data.x[1][4] );
    EXPECT_DOUBLE_EQ( -1.0 , data.y[4] );

    EXPECT_DOUBLE_EQ(  0.0 , data.x[0][5] );
    EXPECT_DOUBLE_EQ(  1.0 , data.x[1][5] );
    EXPECT_DOUBLE_EQ(  1.0 , data.y[5] );

    EXPECT_DOUBLE_EQ(  0.5 , data.x[0][6] );
    EXPECT_DOUBLE_EQ( -1.0 , data.x[1][6] );
    EXPECT_DOUBLE_EQ( -0.5 , data.y[6] );
    
    EXPECT_DOUBLE_EQ(  0.5 , data.x[0][7] );
    EXPECT_DOUBLE_EQ(  1.0 , data.x[1][7] );
    EXPECT_DOUBLE_EQ(  1.5 , data.y[7] );

    EXPECT_DOUBLE_EQ(  1.0 , data.x[0][8] );
    EXPECT_DOUBLE_EQ( -1.0 , data.x[1][8] );
    EXPECT_DOUBLE_EQ( -0.0 , data.y[8] );
    
    EXPECT_DOUBLE_EQ(  1.0 , data.x[0][9] );
    EXPECT_DOUBLE_EQ(  1.0 , data.x[1][9] );
    EXPECT_DOUBLE_EQ(  2.0 , data.y[9] );
}