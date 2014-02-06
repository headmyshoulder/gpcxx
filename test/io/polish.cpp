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
#include "../common/test_functions.hpp"

#include <gtest/gtest.h>

#include <sstream>
#include <map>
#include <unordered_map>


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
    str << polish( tree.data , "|" , "" , "" , mapper );
    EXPECT_EQ( str.str() , "+|sin|x|-|y|2" );
}

TEST( TESTNAME , polish_tree1_withseparator_and_brackets )
{
    std::map< std::string , std::string > mapping = { { "plus" , "+" } , { "minus" , "-" } };
    
    auto mapper = [mapping]( std::string const& s ) -> std::string {
        auto iter = mapping.find( s );
        if( iter != mapping.end() ) return iter->second;
        else return s; };
        
   test_tree< basic_tree_tag > tree;
    ostringstream str;
    str << polish( tree.data , " " , "[" , "]" , mapper );
    EXPECT_EQ( str.str() , "[+ [sin x] [- y 2]]" );
}



TEST( TESTNAME , polish_tree2_withseparator )
{
    test_tree< basic_tree_tag > tree;
    ostringstream str;
    str << polish( tree.data2 );
    EXPECT_EQ( str.str() , "minus|cos|y|x" );
}




TEST( TESTNAME , read_basic_tree1 )
{
    SCOPED_TRACE( "read_basic_tree1" );
    
    auto tree = get_tree_type< basic_tree_tag >::type();
    string str( "minus|cos|y|x" );
    unordered_map< string , pair< size_t , std::function< std::string() > > > mapper = {
        make_pair( "plus" , make_pair( 2 , [] { return string( "+" ); } ) ) ,
        make_pair( "minus" , make_pair( 2 , [] { return string( "-" ); } ) ) ,
        make_pair( "multiplies" , make_pair( 2 , [] { return string( "*" ); } ) ) ,
        make_pair( "divides" , make_pair( 2 , [] { return string( "/" ); } ) ) ,
        make_pair( "cos" , make_pair( 1 , [] { return string( "cos" ); } ) ) ,
        make_pair( "sin" , make_pair( 1 , [] { return string( "sin" ); } ) ) ,
        make_pair( "x" , make_pair( 0 , [] { return string( "x" ); } ) ) ,
        make_pair( "y" , make_pair( 0 , [] { return string( "y" ); } ) ) 
    };
    read_polish( str , tree , mapper , "|" );
    
    EXPECT_EQ( tree.size() , 4 );
    test_cursor( tree.root() , "-" , 2 , 3 , 0 );
    test_cursor( tree.root().children(0) , "cos" , 1 , 2 , 1 );
    test_cursor( tree.root().children(0).children(0) , "y" , 0 , 1 , 2 );
    test_cursor( tree.root().children(1) , "x" , 0 , 1 , 1 );
}

TEST( TESTNAME , read_basic_tree2 )
{
}

TEST( TESTNAME , read_intrusive_tree1 )
{
}

TEST( TESTNAME , read_intrusive_tree2 )
{
}

TEST( TESTNAME , read_basic_tree1_with_brackets )
{
}

TEST( TESTNAME , read_basic_tree1_with_custom_separator )
{
}


