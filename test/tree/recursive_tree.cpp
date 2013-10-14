/*
 * test/tree/recursive_tree.cpp
 * Date: 2013-09-14
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/tree/recursive_tree.hpp>

#include <gtest/gtest.h>

#include <sstream>

#define TESTNAME recursive_tree_tests

using namespace std;
using namespace gpcxx;

TEST( TESTNAME , tree_node_size )
{
    std::cout << sizeof( detail::nil ) << endl;
    std::cout << sizeof( detail::recursive_node_base< char > ) << endl;
    std::cout << sizeof( detail::recursive_node_base< short > ) << endl;
    std::cout << sizeof( detail::recursive_node_base< int > ) << endl;
    std::cout << sizeof( detail::recursive_node< char > ) << endl;
    std::cout << sizeof( detail::recursive_node< short > ) << endl;
    std::cout << sizeof( detail::recursive_node< int > ) << endl;
}
