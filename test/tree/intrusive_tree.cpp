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


#define TESTNAME intrusive_tree_tests

using namespace std;
using namespace gpcxx;

TEST( TESTNAME , TestCase )
{
    intrusive_tree< double > tree;
    EXPECT_EQ( true , true );
}
