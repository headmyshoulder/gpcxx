/*
 * simple.cpp
 * Date: 2013-02-23
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include <gpcxx/io/simple.hpp>

#include "../common/test_tree.hpp"

#include <gtest/gtest.h>

#include <sstream>

#define TESTNAME simple_io_tests

using namespace std;

TEST( TESTNAME , simple1 )
{
    test_tree< basic_tree_tag > tree;
    ostringstream str;
    str << simple( tree.data );
    EXPECT_EQ( str.str() , "sin( x ) plus ( y minus 2 )" );
}

TEST( TESTNAME , simple2 )
{
    test_tree< basic_tree_tag > tree;
    ostringstream str;
    str << simple( tree.data2 );
    EXPECT_EQ( str.str() , "cos( y ) minus x" );
}

