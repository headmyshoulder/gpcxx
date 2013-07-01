/*
 test/tree/basic_tree.cpp

 Copyright 2013 Karsten Ahnert

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gp/tree/basic_tree.hpp>

#include <gtest/gtest.h>

#include <sstream>

using namespace std;
using namespace gp;

TEST( tree_tests , basic_tree_default_construct )
{
    basic_tree< int > tree;
    auto root = tree.root();
}

TEST( tree_tests , basic_tree_insert )
{
    basic_tree< int > tree;
    auto root = tree.root();
    tree.insert_below( root , 10 );
}

