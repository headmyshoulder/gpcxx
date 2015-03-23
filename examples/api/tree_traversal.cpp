/*
 * tree_traversal.cpp
 * Date: 2015-03-23
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/tree/basic_tree.hpp>
#include <gpcxx/tree/iterator/preorder_iterator.hpp>
#include <gpcxx/tree/iterator/postorder_iterator.hpp>

#include <string>
#include <iostream>
#include <algorithm>

using namespace std;
using namespace gpcxx;

void iteration( void )
{
    //[ preorder_iteration_1
    basic_tree< std::string > tree;
    tree.insert_below( tree.root() , "A" );
    auto c1 = tree.insert_below( tree.root() , "B" );
    auto c2 = tree.insert_below( tree.root() , "C" );
    tree.insert_below( c1 , "D" );
    tree.insert_below( c1 , "E" );
    tree.insert_below( c2 , "F" );
    
    auto first = begin_preorder( tree.root() );
    auto last = end_preorder( tree.root() );
    // will print A B D E C F
    std::for_each( first , last , []( auto x ) { cout << x << " "; } );
    cout << endl;
    //]
    
    {
        //[ postorder_iteration_1
        auto first = begin_postorder( tree.root() );
        auto last = end_postorder( tree.root() );
        // will print D E B F C A
        std::for_each( first , last , []( auto x ) { cout << x << " "; } );
        cout << endl;
        //]
    }
}

void cursors( void )
{
    //[ cursor_1
    basic_tree< std::string > tree;
    tree.insert_below( tree.root() , "A" );
    auto c1 = tree.insert_below( tree.root() , "B" );
    auto c2 = tree.insert_below( tree.root() , "C" );
    tree.insert_below( c1 , "D" );
    tree.insert_below( c1 , "E" );
    tree.insert_below( c2 , "F" );
    //]
    
    {
        //[ cursor_2
        auto root = tree.root();
        cout << *root << endl;              // dereference, will print A
        auto c1 = root.children(0);         // get the first child of A
        cout << *c1 << endl;                // will print B
        cout << root.size() << endl;        // get the number of child nodes of A
        auto parent = c1.parent();          // get the parent of c1
        cout << *parent << endl;            // will print A
        //]
    }
    
    //[ cursor_3
    {
        auto root = tree.root();
        for( auto c : root )
            cout << c << endl;
    }
    //]
    
    //[ cursor_4
    {
        auto root = tree.root();
        cout << root.height() << endl;                    // will print 3
        cout << root.level() << endl;                     // will print 0
        cout << root.children(0).height() << endl;        // will print 2
        cout << root.children(0).level() << endl;         // will print 1
    }
    //]
    
}


int main( int argc , char **argv )
{
    iteration();
    cursors();
    
    return 0;
}