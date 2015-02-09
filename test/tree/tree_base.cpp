/*
 * test/tree/tree_base.cpp
 * Date: 2015-02-09
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/tree/detail/tree_base.hpp>

#include <sstream>
#include <gtest/gtest.h>

#define TESTNAME tree_base_tests

using namespace std;
using namespace gpcxx;

TEST( TESTNAME , TestCase )
{
    tree_base< std::string > t;
}
