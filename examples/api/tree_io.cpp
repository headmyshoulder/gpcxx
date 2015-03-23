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
    /* auto r = */ tree.insert_below( root , "x" );
    /* auto y1 = */ tree.insert_below( l , "y" );
    /* auto y2 = */ tree.insert_below( l , "5" );
    return tree;
}

//[ example_tree_mapper
std::string mapper( std::string const& in )
{
    if( in == "+" ) return "plus";
    if( in == "-" ) return "minus";
    return in;
}
//]


void tree_simple_io( void )
{
    cout << "Simple examples:" << endl;
    
    //[ tree_simple_io
    auto tree = create_tree();
    
    // will write ( y - 5 ) + x
    cout << simple( tree ) << endl;
    
    // will write +( -( y , 5 ) , x )
    cout << simple( tree , false ) << endl;
    
    // will write ( y minus 5 ) plus x
    cout << simple( tree , true , mapper ) << endl;
    
    // will write ( y - 5 ) + x
    write_simple( cout , tree , true , identity() );
    cout << endl;
    
    // will write y - 5
    write_simple_cursor( cout , tree.root().children(0) , true , identity() );
    cout << endl;

    // will write ( y - 5 ) + x
    std::string s = simple_string( tree , true , identity() );
    cout << s << endl;
    //]
    
    cout << endl << endl;
}

void tree_simple_io_1( void )
{
    //[ tree_simple_io_1
    auto tree = create_tree();
    
    // will write ( y - 5 ) + x
    cout << simple( tree ) << endl;
    //]
}    


void tree_polish_io( void )
{
    cout << "Polish examples:" << endl;
    //[ tree_polish_io
    auto tree = create_tree();
    
    // will write +|-|y|5|x
    cout << polish( tree ) << endl;
    
    // will write [+|[-|y|5]|x]
    cout << polish( tree , "|" , "[" , "]" ) << endl;
    
    // will write plus|minus|y|5|x    
    cout << polish( tree , "|" , "" , "" , mapper ) << endl;

    // will write +|-|y|5|x
    write_polish( cout , tree , "|" , "" , "" , identity() );
    cout << endl;
    
    // will write -|y|5
    write_polish_cursor( cout , tree.root().children(0) , "|" , "" , "" , identity() );
    cout << endl;

    // will write +|-|y|5|x
    std::string s = polish_string( tree , "|" , "" , "" , identity() );
    cout << s << endl;

    //]
    cout << endl << endl;
}

void tree_polish_io_2( void )
{
    //[ tree_polish_io_2
    auto tree = create_tree();
    //]
}

void tree_polish_io_3( void )
{
    //[ tree_polish_io_3
    auto tree = create_tree();
    cout << polish( tree  ) << endl;
    // will write +|-|y|5|x
    //]
}


int main( int argc , char **argv )
{
    tree_simple_io();
    tree_simple_io_1();
    tree_polish_io();
    

    return 0;
}