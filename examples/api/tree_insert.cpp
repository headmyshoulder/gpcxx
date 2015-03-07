/*
 * tree_insert.cpp
 * Date: 2015-03-07
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/tree/basic_tree.hpp>
#include <gpcxx/io/graphviz.hpp>

#include <string>
#include <fstream>

using namespace std;
using namespace gpcxx;

void insert_below_examples( void )
{
    //[ insert_below_1]
    basic_tree< std::string > tree;
    tree.insert_below( tree.root() , "A" );
    //]
    
    generate_graphviz_png( "insert_below_1.png" , tree );
    
    //[ insert_below_2]
    tree.insert_below( tree.root() , "B" );
    //]
    
    generate_graphviz_png( "insert_below_2.png" , tree );
    
    //[ insert_below_3]
    tree.insert_below( tree.root() , "C" );
    //]
    
    generate_graphviz_png( "insert_below_3.png" , tree );
}

int main( int argc , char **argv )
{
    insert_below_examples();
    return 0;
}