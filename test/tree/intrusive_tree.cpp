/*
 * test/tree/intrusive_tree.cpp
 * Date: 2013-10-07
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/tree/intrusive_tree.hpp>

#include <gtest/gtest.h>

#include <sstream>
#include <array>

#define TESTNAME intrusive_tree_tests

using namespace std;
using namespace gpcxx;


/// user stuff
using context_type = std::array< double , 3 >;

struct my_node : public intrusive_node< my_node >
{
    typedef std::function< double( context_type const& , my_node const& ) > func_type;
    
    my_node( func_type f , std::string name )
    : m_func( std::move( f ) ) , m_name( std::move( name ) ) { }
    
    double eval( context_type const & context ) const
    {
        return m_func( context , *this );
    }
    
    func_type m_func;
    std::string m_name;
};

#define UNARY_FUNC( NAME , FUNC )                                                                     \
struct NAME : public intrusive_node< NAME >                                                           \
{                                                                                                     \
    double operator()( context_type const& c , my_node const& node ) const                            \
    {                                                                                                 \
        return FUNC( node.children( 0 )->eval( c ) );                                                 \
    }                                                                                                 \
}

#define BINARY_FUNC( NAME , FUNC )                                                                    \
struct NAME : public intrusive_node< NAME >                                                           \
{                                                                                                     \
    double operator()( context_type const& c , my_node const& node ) const                            \
    {                                                                                                 \
        return node.children( 0 )->eval( c ) FUNC + node.children( 1 )->eval( c );                    \
    }                                                                                                 \
}


UNARY_FUNC( sin_func , sin );
UNARY_FUNC( cos_func , cos );
UNARY_FUNC( exp_func , exp );
UNARY_FUNC( log_func , log );

BINARY_FUNC( plus_func , + );
BINARY_FUNC( minus_func , - );
BINARY_FUNC( multiplies_func , * );
BINARY_FUNC( divides_func , / );


template< size_t I >
struct terminal
{
    double operator()( context_type const& c , my_node const& node ) const
    {
        return c[I];
    }
};

using terminal_x = terminal< 0 >;
using terminal_y = terminal< 1 >;
using terminal_z = terminal< 2 >;

using tree_type = intrusive_tree< my_node >;

template< typename Cursor , typename Value >
void test_cursor( Cursor n , Value value , size_t artiy , size_t height , size_t level )
{
    EXPECT_EQ( n->m_name , value );
    EXPECT_EQ( n.size() , artiy );
    EXPECT_EQ( n.height() , height );
    EXPECT_EQ( (n).level() , level );
}

TEST( TESTNAME , default_construct )
{
    tree_type tree;
    EXPECT_EQ( tree.size() , 0 );
    EXPECT_TRUE( tree.empty() );
}


TEST( TESTNAME , insert_value )
{
    tree_type tree;
    auto root = tree.root();
    auto n1 = tree.insert_below( root , my_node( plus_func() , "+" ) );
    
    EXPECT_EQ( tree.size() , 1 );
    EXPECT_FALSE( tree.empty() );
    test_cursor( n1 , "+" , 0 , 1 , 0 );
    test_cursor( tree.root() , "+" , 0 , 1 , 0 );

    auto n2 = tree.insert_below( n1 , my_node( terminal_x() , "l1" ) );
    auto n3 = tree.insert_below( n1 , my_node( terminal_y() , "l2" ) );
    EXPECT_FALSE( tree.empty() );
    EXPECT_EQ( tree.size() , 3 );
    test_cursor( tree.root() , "+" , 2 , 2 , 0 );
    test_cursor( tree.root().children(0) , "l1" , 0 , 1 , 1 );
    test_cursor( tree.root().children(1) , "l2" , 0 , 1 , 1 );
    test_cursor( n2 , "l1" , 0 , 1 , 1 );
    test_cursor( n3 , "l2" , 0 , 1 , 1 );
}

TEST( TESTNAME , insert_and_erase )
{
    tree_type tree;
    tree.insert_below( tree.root() , my_node( plus_func() , "+" ) );
    auto n1 = tree.insert_below( tree.root() , my_node( minus_func() , "-" ) );
    auto n2 = tree.insert_below( tree.root() , my_node( multiplies_func() , "*" ) );
    auto n3 = tree.insert_below( n1 , my_node( terminal_x() , "13" ) );
    auto n4 = tree.insert_below( n1 , my_node( terminal_y() , "14" ) );
    auto n5 = tree.insert_below( n2 , my_node( terminal_z() , "15" ) );
    auto n6 = tree.insert_below( n2 , my_node( terminal_x() , "16" ) );
    
    EXPECT_EQ( tree.size() , 7 );
    EXPECT_FALSE( tree.empty() );
    test_cursor( tree.root() , "+" , 2 , 3 , 0 );
    test_cursor( tree.root().children(0) , "-" , 2 , 2 , 1 );
    test_cursor( tree.root().children(0).children(0) , "13" , 0 , 1 , 2 );
    test_cursor( tree.root().children(0).children(1) , "14" , 0 , 1 , 2 );
    test_cursor( tree.root().children(1) , "*" , 2 , 2 , 1 );
    test_cursor( tree.root().children(1).children(0) , "15" , 0 , 1 , 2 );
    test_cursor( tree.root().children(1).children(1) , "16" , 0 , 1 , 2 );
    
    tree.erase( n2 );
    
    EXPECT_EQ( tree.size() , 4 );
    EXPECT_FALSE( tree.empty() );
    test_cursor( tree.root() , "+" , 1 , 3 , 0 );
    test_cursor( tree.root().children(0) , "-" , 2 , 2 , 1 );
    test_cursor( tree.root().children(0).children(0) , "13" , 0 , 1 , 2 );
    test_cursor( tree.root().children(0).children(1) , "14" , 0 , 1 , 2 );
}
