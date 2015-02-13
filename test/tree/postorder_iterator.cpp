/*
 * test/tree/postorder_iterator.cpp
 * Date: 2015-01-22
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "../common/test_tree.hpp"
#include "../common/test_functions.hpp"

#include <gpcxx/tree/iterator/postorder_iterator.hpp>

#include <iterator>
#include <sstream>
#include <gtest/gtest.h>

#define TESTNAME postorder_iterator_tests

using namespace std;

template< typename Cursor >
inline void check_cursors_tree1( Cursor first , Cursor last )
{
    EXPECT_NE( first , last );
    test_value( *first , "x" );
    ++first;
    EXPECT_NE( first , last );
    test_value( *first , "sin" );
    ++first;
    EXPECT_NE( first , last );
    test_value( *first , "y" );
    ++first;
    EXPECT_NE( first , last );
    test_value( *first , "2" );
    ++first;
    EXPECT_NE( first , last );
    test_value( *first , "minus" );
    ++first;
    EXPECT_NE( first , last );
    test_value( *first , "plus" );
    ++first;
    EXPECT_EQ( first , last );
}

TEST( TESTNAME , iterate_forward )
{
    test_tree< basic_tree_tag > trees;
    
    auto first = begin_postorder( trees.data.root() );
    auto last = end_postorder( trees.data.root() );
    
    check_cursors_tree1( first , last );
}

TEST( TESTNAME , iterate_backward )
{
    test_tree< basic_tree_tag > trees;
    
    auto first = begin_postorder( trees.data.root() );
    auto last = end_postorder( trees.data.root() );
    
    EXPECT_NE( first , last );
    --last;
    EXPECT_EQ( *last , "plus" );
    EXPECT_NE( first , last );
    --last;
    EXPECT_EQ( *last , "minus" );    
    EXPECT_NE( first , last );
    --last;
    EXPECT_EQ( *last , "2" );    
    EXPECT_NE( first , last );
    --last;
    EXPECT_EQ( *last , "y" );    
    EXPECT_NE( first , last );
    --last;
    EXPECT_EQ( *last , "sin" );    
    EXPECT_NE( first , last );
    --last;
    EXPECT_EQ( *last , "x" );
    EXPECT_EQ( first , last );
}

TEST( TESTNAME , reverse_iterator )
{
    test_tree< basic_tree_tag > trees;
    
    using riterator = reverse_iterator< decltype( begin_postorder( trees.data.root() ) ) >;
    
    auto first = riterator( end_postorder( trees.data.root() ) );
    auto last = riterator( begin_postorder( trees.data.root() ) );
    
    EXPECT_NE( first , last );
    EXPECT_EQ( *first , "plus" );
    ++first;
    EXPECT_NE( first , last );
    EXPECT_EQ( *first , "minus" );
    ++first;
    EXPECT_NE( first , last );
    EXPECT_EQ( *first , "2" );
    ++first;
    EXPECT_NE( first , last );
    EXPECT_EQ( *first , "y" );
    ++first;
    EXPECT_NE( first , last );
    EXPECT_EQ( *first , "sin" );
    ++first;
    EXPECT_NE( first , last );
    EXPECT_EQ( *first , "x" );
    ++first;
    EXPECT_EQ( first , last );
}