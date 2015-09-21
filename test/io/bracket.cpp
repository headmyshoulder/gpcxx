/*
 * test/io/bracket.cpp
 * Date: 2015-08-18
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/io/bracket.hpp>

#include "../common/test_tree.hpp"
#include "../common/test_functions.hpp"

#include <gtest/gtest.h>
#include <sstream>
#include <map>


#define TESTNAME bracket_io_tests

using namespace std;

TEST( TESTNAME , empty_tree )
{
    basic_tree< std::string > t;
    ostringstream str;
    str << gpcxx::bracket( t );
    EXPECT_EQ( str.str() , "" );
    EXPECT_EQ( true , true );
}

TEST( TESTNAME , bracket1 )
{
    test_tree< basic_tree_tag > tree;
    ostringstream str;
    str << gpcxx::bracket( tree.data );
    EXPECT_EQ( str.str() , "{plus{sin{x}}{minus{y}{2}}}" );
}

TEST( TESTNAME , bracket1_custom_mapper )
{
    std::map< std::string , std::string > mapping = { { "plus" , "+" } , { "minus" , "-" } };
    
    auto mapper = [mapping]( std::string const& s ) -> std::string {
        auto iter = mapping.find( s );
        if( iter != mapping.end() ) return iter->second;
        else return s; };
        
    test_tree< basic_tree_tag > tree;
    ostringstream str;
    str << gpcxx::bracket( tree.data , "(" , ")" , mapper );
    EXPECT_EQ( str.str() , "(+(sin(x))(-(y)(2)))" );
}

TEST( TESTNAME , read_empty_tree )
{
    basic_tree< std::string > t;
    std::string str {};
    gpcxx::read_bracket( str , t );
    EXPECT_TRUE( t.empty() );
}

TEST( TESTNAME , read_tree1 )
{
    basic_tree< std::string > t;
    std::string str { "{plus{sin{x}}{minus{y}{2}}}" };
    gpcxx::read_bracket( str , t );
    ASSERT_FALSE( t.empty() );
    test_cursor( t.root() , "plus" , 2 , 3 , 0 );
    test_cursor( t.root().children(0) , "sin" , 1 , 2 , 1 );
    test_cursor( t.root().children(0).children(0) , "x" , 0 , 1 , 2 );
    test_cursor( t.root().children(1) , "minus" , 2 , 2 , 1 );
    test_cursor( t.root().children(1).children(0) , "y" , 0 , 1 , 2 );
    test_cursor( t.root().children(1).children(1) , "2" , 0 , 1 , 2 );
}

TEST( TESTNAME , read_tree2 )
{
    std::map< std::string , std::string > mapping = { { "+" , "plus" } , { "-" , "minus" } };
    
    auto mapper = [mapping]( std::string const& s ) -> std::string {
        auto iter = mapping.find( s );
        if( iter != mapping.end() ) return iter->second;
        else return s; };
        
    basic_tree< std::string > t;
    std::string str { "(+(sin(x))(-(y)(2)))" };
    gpcxx::read_bracket( str , t , mapper , "(" , ")" );
    ASSERT_FALSE( t.empty() );
    test_cursor( t.root() , "plus" , 2 , 3 , 0 );
    test_cursor( t.root().children(0) , "sin" , 1 , 2 , 1 );
    test_cursor( t.root().children(0).children(0) , "x" , 0 , 1 , 2 );
    test_cursor( t.root().children(1) , "minus" , 2 , 2 , 1 );
    test_cursor( t.root().children(1).children(0) , "y" , 0 , 1 , 2 );
    test_cursor( t.root().children(1).children(1) , "2" , 0 , 1 , 2 );
}

TEST( TESTNAME , read_tree3 )
{
    basic_tree< std::string > t;
    std::string str { "{a}" };
    gpcxx::read_bracket( str , t );
    EXPECT_EQ( t.size() , size_t { 1 } );
}

TEST( TESTNAME , read_broken1 )
{
    basic_tree< std::string > t;
    std::string str { "{" };
    EXPECT_THROW( { gpcxx::read_bracket( str , t ); } , gpcxx::gpcxx_exception );
}

TEST( TESTNAME , read_broken2 )
{
    basic_tree< std::string > t;
    std::string str { "}" };
    EXPECT_THROW( { gpcxx::read_bracket( str , t ); } , gpcxx::gpcxx_exception );
}

TEST( TESTNAME , read_broken3 )
{
    basic_tree< std::string > t;
    std::string str { "a" };
    EXPECT_THROW( { gpcxx::read_bracket( str , t ); } , gpcxx::gpcxx_exception );
}

TEST( TESTNAME , read_unicode )
{
    basic_tree< std::string > t;
    std::string str = "{τ{σ}}";
    gpcxx::read_bracket( str , t );
    ASSERT_FALSE( t.empty() );
    test_cursor( t.root() , "τ" , 1 , 2 , 0 );
    test_cursor( t.root().children(0) , "σ" , 0 , 1 , 1 );
}