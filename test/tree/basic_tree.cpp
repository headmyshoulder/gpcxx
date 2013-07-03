/*
 test/tree/basic_tree.cpp

 Copyright 2013 Karsten Ahnert

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gp/tree/basic_tree.hpp>
#include <gp/io/simple.hpp>

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
    basic_tree< std::string > tree;
    auto root = tree.root();
    auto n1 = tree.insert_below( root , std::string( "+" ) );
    
    cerr << *n1 << endl;
    cerr << n1.size() << endl;
    cerr << simple( tree ) << endl;

    tree.insert_below( n1 , std::string( "11" ) );
    
    cerr << simple( tree ) << endl;
    tree.insert_below( n1 , std::string( "12" ) );
    cerr << simple( tree ) << endl;
}

TEST( tree_tests , basic_tree_insert2 )
{
    basic_tree< std::string > tree;
    tree.insert_below( tree.root() , std::string( "+" ) );
    cerr << simple( tree ) << endl;
    tree.insert_below( tree.root() , std::string( "11" ) );
    cerr << simple( tree ) << endl;
    tree.insert_below( tree.root() , std::string( "12" ) );
    cerr << simple( tree ) << endl;
}


