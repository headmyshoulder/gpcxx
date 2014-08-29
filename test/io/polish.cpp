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

class TESTNAME : public ::testing::Test
{
protected:
    
    typedef pair< size_t , std::function< std::string() > > basic_mapped_func;
    typedef unordered_map< string , basic_mapped_func > basic_mapper_map_type;
    typedef std::function< basic_mapped_func( string const& ) > basic_mapper_type;
    
    typedef get_tree_type< intrusive_tree_tag >::type intrusive_tree_type;
    typedef typename intrusive_tree_type::node_type intrusive_node_type;
    typedef pair< size_t , std::function< intrusive_node_type() > > intrusive_mapped_func;
    typedef unordered_map< string , intrusive_mapped_func > intrusive_mapper_map_type;
    typedef std::function< intrusive_mapped_func( string const& ) > intrusive_mapper_type;
    
    basic_mapper_map_type m_basic_mapper_map;
    basic_mapper_type m_basic_mapper;
    
    intrusive_mapper_map_type m_intrusive_mapper_map;
    intrusive_mapper_type m_intrusive_mapper;

    
    void SetUp( void ) override
    {
        m_basic_mapper_map = basic_mapper_map_type {
            make_pair( "plus" , make_pair( 2 , []() { return string( "plus" ); } ) ) ,
            make_pair( "+" , make_pair( 2 , []() { return string( "plus" ); } ) ) ,
            make_pair( "minus" , make_pair( 2 , []() { return string( "minus" ); } ) ) ,
            make_pair( "-" , make_pair( 2 , []() { return string( "minus" ); } ) ) ,
            make_pair( "multiplies" , make_pair( 2 , []() { return string( "multiplies" ); } ) ) ,
            make_pair( "*" , make_pair( 2 , []() { return string( "multiplies" ); } ) ) ,
            make_pair( "divides" , make_pair( 2 , []() { return string( "divides" ); } ) ) ,
            make_pair( "/" , make_pair( 2 , []() { return string( "divides" ); } ) ) ,
            make_pair( "%" , make_pair( 2 , []() { return string( "divides" ); } ) ) ,
            make_pair( "cos" , make_pair( 1 , []() { return string( "cos" ); } ) ) ,
            make_pair( "sin" , make_pair( 1 , []() { return string( "sin" ); } ) ) ,
            make_pair( "exp" , make_pair( 1 , []() { return string( "exp" ); } ) ) ,
            make_pair( "x" , make_pair( 0 , []() { return string( "x" ); } ) ) ,
            make_pair( "y" , make_pair( 0 , []() { return string( "y" ); } ) ) ,
            make_pair( "z" , make_pair( 0 , []() { return string( "z" ); } ) ) ,
            make_pair( "2" , make_pair( 0 , []() { return string( "2" ); } ) )
        };
        
        m_basic_mapper = [this] ( std::string const& str ) -> basic_mapped_func {
            auto iter = m_basic_mapper_map.find( str );
            if( iter != m_basic_mapper_map.end() ) return iter->second;
            else return make_pair( size_t( 0 ) , [str]() { return str; } );
        };
            
        
        m_intrusive_mapper_map = intrusive_mapper_map_type {
            make_pair( "plus" , make_pair( 2 , []() { return intrusive_node_type( plus_func {} , "plus" ); } ) ) ,
            make_pair( "minus" , make_pair( 2 , []() { return intrusive_node_type( minus_func {} , "minus" ); } ) ) ,
            make_pair( "multiplies" , make_pair( 2 , []() { return intrusive_node_type( multiplies_func {} , "multiplies" ); } ) ) ,
            make_pair( "divides" , make_pair( 2 , []() { return intrusive_node_type( divides_func {} , "divides" ); } ) ) ,
            make_pair( "cos" , make_pair( 1 , []() { return intrusive_node_type( cos_func {} , "cos" ); } ) ) ,
            make_pair( "sin" , make_pair( 1 , []() { return intrusive_node_type( sin_func {} , "sin" ); } ) ) ,
            make_pair( "x" , make_pair( 0 , []() { return intrusive_node_type( array_terminal<0> {} , "x" ); } ) ) ,
            make_pair( "y" , make_pair( 0 , []() { return intrusive_node_type( array_terminal<1> {} , "y" ); } ) ) ,
            make_pair( "2" , make_pair( 0 , []() { return intrusive_node_type( constant_terminal<2> { } , "2" ); } ) ) 
        };
        
        m_intrusive_mapper = [this]( std::string const& str ) -> intrusive_mapped_func {
            auto iter = m_intrusive_mapper_map.find( str );
            if( iter != m_intrusive_mapper_map.end() ) return iter->second;
            else return make_pair( size_t( 0 ) , [str]() { return intrusive_node_type( constant_terminal<0> { } , "str" ); } );
        };
    }
    
    // void TearDown( void ) override  { }
};


TEST_F( TESTNAME , polish_tree1_withseparator )
{
    test_tree< basic_tree_tag > tree;
    ostringstream str;
    str << polish( tree.data );
    EXPECT_EQ( str.str() , "plus|sin|x|minus|y|2" );
}

TEST_F( TESTNAME , polish_tree1_withoutseparator )
{
    test_tree< basic_tree_tag > tree;
    ostringstream str;
    str << polish( tree.data , "" );
    EXPECT_EQ( str.str() , "plussinxminusy2" );
}

TEST_F( TESTNAME , polish_tree1_withseparator_and_mapper )
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

TEST_F( TESTNAME , polish_tree1_withseparator_and_brackets )
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



TEST_F( TESTNAME , polish_tree2_withseparator )
{
    test_tree< basic_tree_tag > tree;
    ostringstream str;
    str << polish( tree.data2 );
    EXPECT_EQ( str.str() , "minus|cos|y|x" );
}


TEST_F( TESTNAME , read_basic_tree1 )
{
    SCOPED_TRACE( "read_basic_tree1" );
    
    auto tree = get_tree_type< basic_tree_tag >::type {};
    string str( "plus|sin|x|minus|y|2" );
    read_polish( str , tree , m_basic_mapper , "|" );
    
    EXPECT_EQ( tree.size() , 6 );
    test_cursor( tree.root() , "plus" , 2 , 3 , 0 );
    test_cursor( tree.root().children(0) , "sin" , 1 , 2 , 1 );
    test_cursor( tree.root().children(0).children(0) , "x" , 0 , 1 , 2 );
    test_cursor( tree.root().children(1) , "minus" , 2 , 2 , 1 );
    test_cursor( tree.root().children(1).children(0) , "y" , 0 , 1 , 2 );
    test_cursor( tree.root().children(1).children(1) , "2" , 0 , 1 , 2 );
    
    test_tree< basic_tree_tag > trees;
    EXPECT_TRUE( tree == trees.data );

}

TEST_F( TESTNAME , read_basic_tree2 )
{
    SCOPED_TRACE( "read_basic_tree2" );
    
    auto tree = get_tree_type< basic_tree_tag >::type {};
    string str( "minus|cos|y|x" );
    read_polish( str , tree , m_basic_mapper , "|" );
    
    EXPECT_EQ( tree.size() , 4 );
    test_cursor( tree.root() , "minus" , 2 , 3 , 0 );
    test_cursor( tree.root().children(0) , "cos" , 1 , 2 , 1 );
    test_cursor( tree.root().children(0).children(0) , "y" , 0 , 1 , 2 );
    test_cursor( tree.root().children(1) , "x" , 0 , 1 , 1 );
    
    test_tree< basic_tree_tag > trees;
    EXPECT_TRUE( tree == trees.data2 );
}



TEST_F( TESTNAME , read_intrusive_tree1 )
{
    SCOPED_TRACE( "read_intrusive_tree1" );
    
    auto tree = get_tree_type< intrusive_tree_tag >::type {};
    string str( "minus|cos|y|x" );
    read_polish( str , tree , m_intrusive_mapper , "|" );
    
    EXPECT_EQ( tree.size() , 4 );
    test_cursor( tree.root() , "minus" , 2 , 3 , 0 );
    test_cursor( tree.root().children(0) , "cos" , 1 , 2 , 1 );
    test_cursor( tree.root().children(0).children(0) , "y" , 0 , 1 , 2 );
    test_cursor( tree.root().children(1) , "x" , 0 , 1 , 1 );
    
    test_tree< intrusive_tree_tag > trees;
    EXPECT_TRUE( tree == trees.data2 );

}

TEST_F( TESTNAME , read_intrusive_tree2 )
{
    SCOPED_TRACE( "read_intrusive_tree2" );
    
    auto tree = get_tree_type< intrusive_tree_tag >::type {};
    string str( "plus|sin|x|minus|y|2" );
    read_polish( str , tree , m_intrusive_mapper , "|" );
    
    EXPECT_EQ( tree.size() , 6 );
    test_cursor( tree.root() , "plus" , 2 , 3 , 0 );
    test_cursor( tree.root().children(0) , "sin" , 1 , 2 , 1 );
    test_cursor( tree.root().children(0).children(0) , "x" , 0 , 1 , 2 );
    test_cursor( tree.root().children(1) , "minus" , 2 , 2 , 1 );
    test_cursor( tree.root().children(1).children(0) , "y" , 0 , 1 , 2 );
    test_cursor( tree.root().children(1).children(1) , "2" , 0 , 1 , 2 );
    
    test_tree< intrusive_tree_tag > trees;
    EXPECT_TRUE( tree == trees.data );

}

TEST_F( TESTNAME , read_basic_tree1_with_brackets )
{
    SCOPED_TRACE( "read_basic_tree1_with_brackets" );
    
    auto tree = get_tree_type< basic_tree_tag >::type {};
    string str( "[plus [sin x] [minus y 2]]" );
    read_polish( str , tree , m_basic_mapper , " " , "[" , "]" );
    
    EXPECT_EQ( tree.size() , 6 );
    test_cursor( tree.root() , "plus" , 2 , 3 , 0 );
    test_cursor( tree.root().children(0) , "sin" , 1 , 2 , 1 );
    test_cursor( tree.root().children(0).children(0) , "x" , 0 , 1 , 2 );
    test_cursor( tree.root().children(1) , "minus" , 2 , 2 , 1 );
    test_cursor( tree.root().children(1).children(0) , "y" , 0 , 1 , 2 );
    test_cursor( tree.root().children(1).children(1) , "2" , 0 , 1 , 2 );
    
    test_tree< basic_tree_tag > trees;
    EXPECT_TRUE( tree == trees.data );
}

TEST_F( TESTNAME , read_basic_tree_marc )
{
    SCOPED_TRACE( "read_basic_tree1_with_brackets" );
    
    auto tree = get_tree_type< basic_tree_tag >::type {};
    string str( "(% (- (exp (+ y z)) (* (% (+ x (* (+ x x) "
        "(- (- (% x z) z) -0.9783254))) 0.9326862) "
        "x)) (+ (+ (% x z) z) (+ (exp x) (* x (% (+ "
        "(+ (+ x x) (* (exp (* y (% x (- 0.76531005 "
        "(% x (exp y)))))) -0.10251226)) (* (+ (% "
        "(exp -0.43739066) x) (% (- (% x z) x) (- "
        "x (+ (- z (- x (+ (+ z (* -0.3348239 x)) "
        "(+ z z)))) 0.6675957)))) -0.8311438)) (% "
        "(- (* (exp (* x (- y (- z y)))) (% (* z y) "
        "(- z (- -0.5977164 x)))) -0.28946796) (+ "
        "y (+ -0.677738 (+ z y)))))))))" );
    read_polish( str , tree , m_basic_mapper , " " , "(" , ")" );
    EXPECT_EQ( 111 , tree.size() );
}
