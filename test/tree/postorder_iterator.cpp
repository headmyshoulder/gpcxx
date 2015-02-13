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
    test_value( *first , "z" );
    ++first;
    EXPECT_NE( first , last );
    test_value( *first , "minus" );
    ++first;
    EXPECT_NE( first , last );
    test_value( *first , "plus" );
    ++first;
    EXPECT_EQ( first , last );
}

TEST( TESTNAME , TestCase )
{
    test_tree< basic_tree_tag > trees;
    
    auto first = begin_postorder( trees.data.root() );
    auto last = end_postorder( trees.data.root() );
    
    check_cursors_tree1( first , last );
}
