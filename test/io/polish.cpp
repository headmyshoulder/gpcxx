/*
 * test/io/polish.cpp
 * Date: 2014-02-02
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/io/polish.hpp>

#include "../common/test_tree.hpp"

#include <gtest/gtest.h>

#include <sstream>
#include <map>


#define TESTNAME polish_tests

using namespace std;

TEST( TESTNAME , polish_tree1_withseparator )
{
    test_tree< basic_tree_tag > tree;
    ostringstream str;
    str << polish( tree.data );
    EXPECT_EQ( str.str() , "plus|sin|x|minus|y|2" );
}

TEST( TESTNAME , polish_tree1_withoutseparator )
{
    test_tree< basic_tree_tag > tree;
    ostringstream str;
    str << polish( tree.data , "" );
    EXPECT_EQ( str.str() , "plussinxminusy2" );
}

TEST( TESTNAME , polish_tree1_withseparator_and_mapper )
{
    std::map< std::string , std::string > mapping = { { "plus" , "+" } , { "minus" , "-" } };
    
    auto mapper = [mapping]( std::string const& s ) -> std::string {
        auto iter = mapping.find( s );
        if( iter != mapping.end() ) return iter->second;
        else return s; };
        
   test_tree< basic_tree_tag > tree;
    ostringstream str;
    str << polish( tree.data , "|" , mapper );
    EXPECT_EQ( str.str() , "+|sin|x|-|y|2" );
}


TEST( TESTNAME , polish_tree2_withseparator )
{
    test_tree< basic_tree_tag > tree;
    ostringstream str;
    str << polish( tree.data2 );
    EXPECT_EQ( str.str() , "minus|cos|y|x" );
}