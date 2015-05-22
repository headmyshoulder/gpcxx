/*
 * test/io/json.cpp
 * Date: 2015-05-21
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/io/json.hpp>
#include <gpcxx/tree/basic_tree.hpp>

#include "../common/test_tree.hpp"

#include <gtest/gtest.h>

#include <sstream>

#define TESTNAME json_io_tests


using namespace std;
using namespace gpcxx;

TEST( TESTNAME , empty_tree )
{
    basic_tree< std::string > t;
    ostringstream str;
    str << json( t );
    EXPECT_EQ( str.str() , "{}" );
}

TEST( TESTNAME , json1 )
{
    test_tree< basic_tree_tag > tree;
    ostringstream str;
    str << json( tree.data );
    EXPECT_EQ( str.str() , "{\n  \"value\" : \"plus\" ,\n  \"children\" : [\n    {\n      \"value\" : \"sin\" ,\n      \"children\" : [\n        {\n          \"value\" : \"x\" ,\n          \"children\" : []\n        }\n      ]\n    } ,\n    {\n      \"value\" : \"minus\" ,\n      \"children\" : [\n        {\n          \"value\" : \"y\" ,\n          \"children\" : []\n        } ,\n        {\n          \"value\" : \"2\" ,\n          \"children\" : []\n        }\n      ]\n    }\n  ]\n}" );
   
}

TEST( TESTNAME , json_string )
{
    test_tree< basic_tree_tag > tree;
    auto str = json_string( tree.data );
    EXPECT_EQ( str , "{\n  \"value\" : \"plus\" ,\n  \"children\" : [\n    {\n      \"value\" : \"sin\" ,\n      \"children\" : [\n        {\n          \"value\" : \"x\" ,\n          \"children\" : []\n        }\n      ]\n    } ,\n    {\n      \"value\" : \"minus\" ,\n      \"children\" : [\n        {\n          \"value\" : \"y\" ,\n          \"children\" : []\n        } ,\n        {\n          \"value\" : \"2\" ,\n          \"children\" : []\n        }\n      ]\n    }\n  ]\n}" );
}

TEST( TESTNAME , json_cursor )
{
    test_tree< basic_tree_tag > tree;
    ostringstream str;
    write_json_cursor( str , tree.data.root() , "\n" , "  " , 0 , gpcxx::identity {} );
    EXPECT_EQ( str.str() , "{\n  \"value\" : \"plus\" ,\n  \"children\" : [\n    {\n      \"value\" : \"sin\" ,\n      \"children\" : [\n        {\n          \"value\" : \"x\" ,\n          \"children\" : []\n        }\n      ]\n    } ,\n    {\n      \"value\" : \"minus\" ,\n      \"children\" : [\n        {\n          \"value\" : \"y\" ,\n          \"children\" : []\n        } ,\n        {\n          \"value\" : \"2\" ,\n          \"children\" : []\n        }\n      ]\n    }\n  ]\n}" );
}

TEST( TESTNAME , json_parameters )
{
    test_tree< basic_tree_tag > tree;
    ostringstream str;
    str << json( tree.data , "" , "" );
    EXPECT_EQ( str.str() , "{\"value\" : \"plus\" ,\"children\" : [{\"value\" : \"sin\" ,\"children\" : [{\"value\" : \"x\" ,\"children\" : []}]} ,{\"value\" : \"minus\" ,\"children\" : [{\"value\" : \"y\" ,\"children\" : []} ,{\"value\" : \"2\" ,\"children\" : []}]}]}" );

}