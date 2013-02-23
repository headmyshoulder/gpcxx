/*
 * simple.cpp
 * Date: 2013-02-23
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include <gp/io/simple.hpp>

#include "../common/test_tree.hpp"

#include <gtest/gtest.h>

#include <sstream>

using namespace std;

TEST( io_tests , simple1 )
{
    test_tree tree;
    ostringstream str;
    str << simple( tree.data );
    EXPECT_EQ( str.str() , "sin( x ) plus ( y minus 2 )" );
}

TEST( io_tests , simple2 )
{
    test_tree tree;
    ostringstream str;
    str << simple( tree.data2 );
    EXPECT_EQ( str.str() , "cos( y ) minus x" );
}

