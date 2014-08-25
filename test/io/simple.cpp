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

TEST( TESTNAME , simple3 )
{
    test_tree< basic_tree_tag > tree;
    ostringstream str;
    str << simple( tree.data3 );
    EXPECT_EQ( str.str() , "plus3( sin( x ) , y minus 2 , cos( y ) minus x )" );
}


TEST( TESTNAME , simple1_no_infix )
{
    test_tree< basic_tree_tag > tree;
    ostringstream str;
    str << simple( tree.data , false );
    EXPECT_EQ( str.str() , "plus( sin( x ) , minus( y , 2 ) )" );
}


TEST( TESTNAME , simple2_no_infix )
{
    test_tree< basic_tree_tag > tree;
    ostringstream str;
    str << simple( tree.data2 , false );
    EXPECT_EQ( str.str() , "minus( cos( y ) , x )" );
}

TEST( TESTNAME , simple3_no_infix )
{
    test_tree< basic_tree_tag > tree;
    ostringstream str;
    str << simple( tree.data3 , false );
    EXPECT_EQ( str.str() , "plus3( sin( x ) , minus( y , 2 ) , minus( cos( y ) , x ) )" );
}


TEST( TESTNAME , simple3_intrusive )
{
    test_tree< intrusive_tree_tag > tree;
    ostringstream str;
    str << simple( tree.data3 );
    EXPECT_EQ( str.str() , "plus3( sin( x ) , y minus 2 , cos( y ) minus x )" );
}


TEST( TESTNAME , simple3_no_infix_intrusive )
{
    test_tree< intrusive_tree_tag > tree;
    ostringstream str;
    str << simple( tree.data3 , false );
    EXPECT_EQ( str.str() , "plus3( sin( x ) , minus( y , 2 ) , minus( cos( y ) , x ) )" );
}



