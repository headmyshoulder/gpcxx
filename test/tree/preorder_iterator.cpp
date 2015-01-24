/*
 * test/tree/preorder_iterator.cpp
 * Date: 2015-01-22
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "../common/test_tree.hpp"

#include <gpcxx/tree/iterator/preorder_iterator.hpp>

#include <sstream>
#include <gtest/gtest.h>

#define TESTNAME preorder_iterator_tests

using namespace std;

TEST( TESTNAME , TestCase )
{
    test_tree< basic_tree_tag > trees;
    
    auto first = begin_preorder( trees.data.root() );
    auto last = end_preorder( trees.data.root() );
    cout << *first << endl;
    ++first;
    cout << *first << endl;
    
    EXPECT_EQ( true , true );
}
