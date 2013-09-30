/*
 * test/stat/node_statistics.cpp
 * Date: 2013-09-30
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/stat/node_statistics.hpp>

#include "../common/test_tree.hpp"

#include <gtest/gtest.h>

#include <sstream>

#define TESTNAME node_statistics_tests

using namespace std;
using namespace gpcxx;

TEST( TESTNAME , test1 )
{
    test_tree< basic_tree_tag > trees;
    node_statistics stat = calc_node_statistics_tree( trees.data );
    
    EXPECT_EQ( stat.num_nodes , 6 );
    EXPECT_EQ( stat.num_terminals , 3 );
    EXPECT_EQ( stat.num_unaries , 1 );
    EXPECT_EQ( stat.num_binaries , 2 );
}

TEST( TESTNAME , test2 )
{
    test_tree< basic_tree_tag > trees;
    node_statistics stat = calc_node_statistics_tree( trees.data2 );

    EXPECT_EQ( stat.num_nodes , 4 );
    EXPECT_EQ( stat.num_terminals , 2 );
    EXPECT_EQ( stat.num_unaries , 1 );
    EXPECT_EQ( stat.num_binaries , 1 );
}
