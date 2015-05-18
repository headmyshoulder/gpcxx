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

#include <gpcxx/tree/basic_nary_tree.hpp>
#include <gpcxx/io/graphviz.hpp>

#include <string>
#include <fstream>
#include <iostream>

using namespace std;
using namespace gpcxx;

void insert_below_examples( void )
{
    //[ insert_below_1
    basic_nary_tree< std::string > tree;
    tree.insert_below( tree.root() , "A" );
    //]
    
    generate_graphviz_png( "insert_below_1.png" , tree );
    
    //[ insert_below_2
    tree.insert_below( tree.root() , "B" );
    //]
    
    generate_graphviz_png( "insert_below_2.png" , tree );
    
    //[ insert_below_3
    tree.insert_below( tree.root() , "C" );
    //]
    
    generate_graphviz_png( "insert_below_3.png" , tree );
    
    //[ insert_below_4
    basic_nary_tree< std::string > tree2;
    tree2.insert_below( tree2.root() , "X" );
    tree2.insert_below( tree2.root() , tree.root() );
    //]
    
    generate_graphviz_png( "insert_below_4.png" , tree2 );
}


void insert_examples( void )
{
    //[ insert_1
    basic_nary_tree< std::string , 3 > tree;
    tree.insert( tree.root() , "A" );
    auto c1 = tree.insert_below( tree.root() , "B" );
    //]
    
    generate_graphviz_png( "insert_1.png" , tree );
    
    //[ insert_2
    auto c2 = tree.insert( c1 , "C" );
    //]
    
    generate_graphviz_png( "insert_2.png" , tree );
    
    //[ insert_3
    tree.insert( c2 , "D" );
    //]
    
    generate_graphviz_png( "insert_3.png" , tree );
}

void insert_above_examples( void )
{
    //[ insert_above_1
    basic_nary_tree< std::string > tree;
    tree.insert( tree.root() , "A" );
    tree.insert_below( tree.root() , "B" );
    auto c1 = tree.insert_below( tree.root() , "C" );
    //]
    
    generate_graphviz_png( "insert_above_1.png" , tree );
    
    //[ insert_above_2
    auto c2 = tree.insert_above( c1 , "D" );
    //]
    
    generate_graphviz_png( "insert_above_2.png" , tree );
    
    //[ insert_above_3
    tree.insert_above( c2 , "E" );
    //]
    
    generate_graphviz_png( "insert_above_3.png" , tree );
}

void emplace_below_examples( void )
{
    //[ emplace_below_1
    basic_nary_tree< std::string > tree;
    auto c1 = tree.emplace_below( tree.root() , "A" );
    tree.emplace_below( c1 , "B" );
    tree.emplace_below( c1 , "C" );
    //]
}

void emplace_examples( void )
{
    //[ emplace_1
    basic_nary_tree< std::string , 3 > tree;
    tree.emplace( tree.root() , "A" );
    auto c1 = tree.emplace_below( tree.root() , "B" );
    auto c2 = tree.emplace( c1 , "C" );
    tree.emplace( c2 , "C" );
    //]
}

void erase_examples( void )
{
    //[ erase_1
    basic_nary_tree< std::string > tree;
    tree.insert_below( tree.root() , "A" );
    auto c1 = tree.insert_below( tree.root() , "B" );
    auto c2 = tree.insert_below( tree.root() , "C" );
    tree.insert_below( c1 , "D" );
    tree.insert_below( c1 , "E" );
    tree.insert_below( c2 , "F" );
    //]
    
    generate_graphviz_png( "erase_1.png" , tree );
    
    
    //[ erase_2
    tree.erase( c1 );
    //]
    
    generate_graphviz_png( "erase_2.png" , tree );
}

void querying_examples( void )
{
    //[ querying_1
    basic_nary_tree< std::string > tree;
    tree.insert_below( tree.root() , "A" );
    tree.insert_below( tree.root() , "B" );
    tree.insert_below( tree.root() , "C" );
    
    cout << tree.size() << endl;        // will print 3
    cout << tree.empty()<< endl;        // will print 0 for false
    cout << tree.max_size() << endl;    // will print a very large number, for example 18446744073709551615
    //]
}

int main( int argc , char **argv )
{
    insert_below_examples();
    insert_examples();
    insert_above_examples();
    emplace_below_examples();
    emplace_examples();
    erase_examples();
    querying_examples();
    
    return 0;
}