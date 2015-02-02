/*
 * preorder_iterator.cpp
 * Date: 2015-01-31
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/tree/iterator/preorder_iterator.hpp>
#include <gpcxx/tree/basic_tree.hpp>
#include <gpcxx/generate/uniform_symbol.hpp>
#include <gpcxx/generate/node_generator.hpp>
#include <gpcxx/generate/ramp.hpp>
#include <gpcxx/io/polish.hpp>
#include <gpcxx/app/timer.hpp>

#include <iostream>
#include <sstream>
#include <random>

template< typename Tree >
void write_tree_iterator( std::ostream& out , Tree& t , std::string const& sep = "|" )
{
    auto first = gpcxx::begin_preorder( t.root() );
    auto last = gpcxx::end_preorder( t.root() );
    
    if( first != last )
    {
        out << *first++;
        for( ; first != last ; )
        {
            out << sep << *first++;
        }
    }
}

template< typename Tree >
void write_tree_stack( std::ostream& out , Tree &t , std::string const& sep = "|" )
{
    if( t.empty() ) return;

    using cursor = typename Tree::cursor;
    
    out << *( t.root() );
    std::stack< cursor > s;
    for( int i=t.root().size() - 1 ; i>=0 ; --i )
        s.push( t.root().children(i) );
    while( ! s.empty() )
    {
        cursor current = s.top();
        out << sep << *current;
        s.pop();
        for( int i=current.size() - 1 ; i >= 0 ; --i )
            s.push( current.children(i) );
    }
}

int main( int argc , char **argv )
{
    using value_type = std::string;
    using tree_type = gpcxx::basic_tree< value_type >;
    using rng_type = std::mt19937;
    using population_type = std::vector< tree_type >;
    
    auto terminals = gpcxx::uniform_symbol< value_type >{ { "x" , "y" , "z" } };
    auto unaries = gpcxx::uniform_symbol< value_type >{ { "sin" , "cos" , "log" , "exp" } };
    auto binaries = gpcxx::uniform_symbol< value_type >{ { "+" , "-" , "*" , "/" } };
    
    auto rng = rng_type{};
    auto node_generator = gpcxx::node_generator< value_type , rng_type , 3 >{
        { 2.0 * double( terminals.num_symbols() ) , 0 , terminals } ,
        { double( unaries.num_symbols() ) , 1 , unaries } ,
        { double( binaries.num_symbols() ) , 2 , binaries } };

    // size_t max_tree_height = 15;
    // size_t population_size = 512 * 256;

    size_t max_tree_height = 15;
    size_t population_size = 512 * 256;
    
    auto tree_generator = gpcxx::make_ramp( rng , node_generator , max_tree_height , max_tree_height , 0.5 );
    auto population = population_type( population_size );
    
    for( size_t i=0 ; i<population.size() ; ++i )
    {
        tree_generator( population[i] );
    }
    
    std::cout << "Starting iterator based polish output." << std::endl;
    auto timer = gpcxx::timer{};
    std::ostringstream stream1;
    for( auto& tree : population )
    {
        write_tree_iterator( stream1 , tree , "|" );
        stream1 << std::endl;
    }
    std::cout << "Finished iterator based polish output in " << timer.seconds() << " seconds. Wrote " << stream1.str().size() << " bytes." << std::endl;
    
    std::cout << "Starting cursor based polish output." << std::endl;
    timer.restart();
    std::ostringstream stream2;
    for( auto& tree : population )
    {
        stream2 << gpcxx::polish( tree , "|" ) << std::endl;
    }
    std::cout << "Finished cursor based polish output in " << timer.seconds() << " seconds. Wrote " << stream2.str().size() << " bytes." << std::endl;


    std::cout << "Starting cursor and stack based polish output." << std::endl;
    timer.restart();
    std::ostringstream stream3;
    for( auto& tree : population )
    {
        write_tree_stack( stream3 , tree , "|" );
        stream3 << std::endl;
    }
    std::cout << "Finished cursor and stack based polish output in " << timer.seconds() << " seconds. Wrote " << stream2.str().size() << " bytes." << std::endl;

    bool equal = ( ( stream1.str() == stream2.str() ) && ( stream2.str() == stream3.str() ) );
    std::cout << "Output of all version is equal: " << ( equal ? "yes" : "no" ) << std::endl;

    


    return 0;
}
