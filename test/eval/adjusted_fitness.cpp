/*
 * test/eval/adjusted_fitness.cpp
 * Date: 2015-03-25
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/eval/adjusted_fitness.hpp>

#include <gtest/gtest.h>

#include <vector>

#define TESTNAME adjusted_fitness_tests

using namespace std;

TEST( TESTNAME , single_adjusted_fitness )
{
    EXPECT_NEAR( 0.3333333333 , gpcxx::adjusted_fitness_single( 2.0 ) , 1.0e-10 );
    EXPECT_DOUBLE_EQ( 1.0 , gpcxx::adjusted_fitness_single( 0.0 ) );
    EXPECT_NEAR( 1.0e-10 , gpcxx::adjusted_fitness_single( 1.0e10 ) , 1.0e-10 );
}

TEST( TESTNAME , adjusted_fitness )
{
    std::vector< double > sf = { 2.0 , 0.0 , 1.0e10 };
    std::vector< double > af;
    gpcxx::adjusted_fitness( sf , af );
    EXPECT_EQ( af.size() , size_t( 3 ) );
    EXPECT_NEAR( 0.3333333333 , af[0] , 1.0e-10 );
    EXPECT_DOUBLE_EQ( 1.0 , af[1]  );
    EXPECT_NEAR( 1.0e-10 , af[2] , 1.0e-10 );

}

TEST( TESTNAME , adjusted_fitness_copy )
{
    std::vector< double > sf = { 2.0 , 0.0 , 1.0e10 };
    std::vector< double > af = gpcxx::adjusted_fitness_copy( sf );
    EXPECT_EQ( af.size() , size_t( 3 ) );
    EXPECT_NEAR( 0.3333333333 , af[0] , 1.0e-10 );
    EXPECT_DOUBLE_EQ( 1.0 , af[1]  );
    EXPECT_NEAR( 1.0e-10 , af[2] , 1.0e-10 );
}

TEST( TESTNAME , adjusted_fitness_inplace )
{
    std::vector< double > sf = { 2.0 , 0.0 , 1.0e10 };
    gpcxx::adjusted_fitness_inplace( sf );
    EXPECT_EQ( sf.size() , size_t( 3 ) );
    EXPECT_NEAR( 0.3333333333 , sf[0] , 1.0e-10 );
    EXPECT_DOUBLE_EQ( 1.0 , sf[1]  );
    EXPECT_NEAR( 1.0e-10 , sf[2] , 1.0e-10 );
}

TEST( TESTNAME , adjusted_fitness_copy_as_functor )
{
    std::vector < std::vector< double > > sf = {
        { 2.0 , 0.0 , 1.0e10 } ,
        { 3.0 , 1.0 , 0.0 } ,
        { 5.0 , 0.1 , 0.1 , 0.5 } };
    std::vector< std::vector< double > > af( sf.size() );
    std::transform( sf.begin() , sf.end() , af.begin() , gpcxx::adjusted_fitness_copy );
    EXPECT_EQ( af[0].size() , size_t( 3 ) );
    EXPECT_EQ( af[1].size() , size_t( 3 ) );
    EXPECT_EQ( af[2].size() , size_t( 4 ) );
}