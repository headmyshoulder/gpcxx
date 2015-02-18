/*
 * tree_io.cpp
 * Date: 2015-02-18
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/tree.hpp>
#include <gpcxx/io.hpp>

#include <iostream>

using namespace std;
using namespace gpcxx;

auto create_tree( void )
{
    basic_tree< string > tree;
    auto root = tree.insert_below( tree.root() , "+" );
    auto l = tree.insert_below( root , "-" );
    auto r = tree.insert_below( root , "x" );
    auto y1 = tree.insert_below( l , "y" );
    auto y2 = tree.insert_below( l , "5" );
    return tree;
}

int main( int argc , char **argv )
{
    auto tree = create_tree();
    cout << simple( tree ) << endl;
    
    
    return 0;
}