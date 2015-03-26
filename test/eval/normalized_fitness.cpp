/*
 * test/eval/normalized_fitness.cpp
 * Date: 2015-03-26
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/eval/normalized_fitness.hpp>

#include <gtest/gtest.h>

#include <vector>

#define TESTNAME normalized_fitness_tests

using namespace std;


TEST( TESTNAME , normalized_fitness )
{
    vector< double > adjusted_fitness = { 0.0 , 0.5 , 0.3 , 0.2 , 1.0 };
    gpcxx::normalize_fitness( adjusted_fitness );
    EXPECT_EQ( adjusted_fitness.size() , size_t( 5 ) );
    EXPECT_DOUBLE_EQ( 0.0 , adjusted_fitness[0] );
    EXPECT_DOUBLE_EQ( 0.25 , adjusted_fitness[1] );
    EXPECT_DOUBLE_EQ( 0.15 , adjusted_fitness[2] );
    EXPECT_DOUBLE_EQ( 0.1 , adjusted_fitness[3] );
    EXPECT_DOUBLE_EQ( 0.5 , adjusted_fitness[4] );
}
