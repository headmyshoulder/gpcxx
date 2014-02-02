/*
 * test/io/graphviz.cpp
 * Date: 2014-01-18
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/io/graphviz.hpp>
#include "../common/test_tree.hpp"

#include <gtest/gtest.h>
#include <map>
#include <sstream>


#define TESTNAME graphviz_io_tests

using namespace std;

TEST( TESTNAME , graphviz1 )
{
    test_tree< basic_tree_tag > tree;
    ostringstream str;
    str << graphviz( tree.data , true );
    std::string expected =
        "digraph G\n"
        "{\n"
        "NODE0 [ label = \"plus(3 0)\" ]\n"
        "NODE0 -> NODE1\n"
        "NODE1 [ label = \"sin(2 1)\" ]\n"
        "NODE1 -> NODE2\n"
        "NODE2 [ label = \"x(1 2)\" ]\n"
        "NODE0 -> NODE3\n"
        "NODE3 [ label = \"minus(2 1)\" ]\n"
        "NODE3 -> NODE4\n"
        "NODE4 [ label = \"y(1 2)\" ]\n"
        "NODE3 -> NODE5\n"
        "NODE5 [ label = \"2(1 2)\" ]\n"
        "}\n";
    EXPECT_EQ( str.str() , expected );
}

TEST( TESTNAME , graphviz2 )
{
    test_tree< basic_tree_tag > tree;
    ostringstream str;
    str << graphviz( tree.data );
    std::string expected =
        "digraph G\n"
        "{\n"
        "NODE0 [ label = \"plus\" ]\n"
        "NODE0 -> NODE1\n"
        "NODE1 [ label = \"sin\" ]\n"
        "NODE1 -> NODE2\n"
        "NODE2 [ label = \"x\" ]\n"
        "NODE0 -> NODE3\n"
        "NODE3 [ label = \"minus\" ]\n"
        "NODE3 -> NODE4\n"
        "NODE4 [ label = \"y\" ]\n"
        "NODE3 -> NODE5\n"
        "NODE5 [ label = \"2\" ]\n"
        "}\n";
    EXPECT_EQ( str.str() , expected );
}

TEST( TESTNAME , graphviz3 )
{
    std::map< std::string , std::string > mapping = { { "plus" , "+" } , { "minus" , "-" } };
    
    auto mapper = [mapping]( std::string const& s ) -> std::string {
        auto iter = mapping.find( s );
        if( iter != mapping.end() ) return iter->second;
        else return s; };
    
        
    test_tree< basic_tree_tag > tree;
    ostringstream str;
    str << graphviz( tree.data , false , mapper );
    std::string expected =
        "digraph G\n"
        "{\n"
        "NODE0 [ label = \"+\" ]\n"
        "NODE0 -> NODE1\n"
        "NODE1 [ label = \"sin\" ]\n"
        "NODE1 -> NODE2\n"
        "NODE2 [ label = \"x\" ]\n"
        "NODE0 -> NODE3\n"
        "NODE3 [ label = \"-\" ]\n"
        "NODE3 -> NODE4\n"
        "NODE4 [ label = \"y\" ]\n"
        "NODE3 -> NODE5\n"
        "NODE5 [ label = \"2\" ]\n"
        "}\n";
    EXPECT_EQ( str.str() , expected );
}