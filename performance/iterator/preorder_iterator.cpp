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

// #include <range/v3/range_facade.hpp>
// #include <range/v3/algorithm/for_each.hpp>
// #include <stack>

#include <iostream>
#include <sstream>
#include <random>
#include <fstream>


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


// template< typename Tree >
// class forward_range : public ranges::range_facade< forward_range< Tree > >
// {
//     friend ranges::range_access;
// 
// public:
// 
//     forward_range( Tree const& t  )
//     : m_tree( t ) , m_stack()
//     {
//         if( !m_tree.empty() )
//             m_stack.push( m_tree.root() );
//     }
// 
// 
// private:
// 
// 
//     
//     struct cursor
//     {
//         forward_range* m_rng;
//         
//         cursor( void ) = default;
//         
//         explicit cursor( forward_range& rng )
//         : m_rng( &rng )
//         {}
//         
//         // decltype( auto ) current( void ) const
//         typename Tree::const_reference current( void ) const
//         {
//             return * ( m_rng->m_stack.top() );
//         }
//     
//         bool done( void ) const
//         {
//             return m_rng->m_stack.empty();
//         }
//   
//         void next( void )
//         {
//             auto current = m_rng->m_stack.top();
//             m_rng->m_stack.pop();
//             for( int i=current.size() - 1 ; i >= 0 ; --i )
//                 m_rng->m_stack.push( current.children( i ) );
//         }
//     };
//     
//     cursor begin_cursor()
//     {
//         return cursor { *this };
//     }
// 
//     Tree const& m_tree;
//     std::stack< typename Tree::const_cursor > m_stack;
// };


// template< typename Tree >
// forward_range< Tree > make_forward_range( Tree& t )
// {
//     return forward_range< Tree >( t );
// }


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

    std::vector< size_t > tree_heights = { 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13 , 14 , 15 , 16 };
    size_t population_size = 16384 * 8;
    
    std::ofstream fout( "result.dat" );
    for( auto height : tree_heights )
    {
        std::cout << "Starting run with height " << height << " and population size " << population_size << "\n";
        
        auto tree_generator = gpcxx::make_ramp( rng , node_generator , height , height , 0.5 );
        auto population = population_type( population_size );
        
        std::cout << "\tGenerating data." << std::endl;
        for( size_t i=0 ; i<population.size() ; ++i )
        {
            tree_generator( population[i] );
        }
        std::cout << "\tFinished." << std::endl;
        
        std::cout << "\tStarting iterator based polish output." << std::endl;
        auto timer = gpcxx::timer{};
        std::ostringstream stream1;
        for( auto& tree : population )
        {
            write_tree_iterator( stream1 , tree , "|" );
            stream1 << std::endl;
        }
        double t1 = timer.seconds();
        std::cout << "\tFinished iterator based polish output in " << t1 << " seconds. Wrote " << stream1.str().size() << " bytes." << std::endl;
        
        std::cout << "\tStarting cursor based polish output." << std::endl;
        timer.restart();
        std::ostringstream stream2;
        for( auto& tree : population )
        {
            stream2 << gpcxx::polish( tree , "|" ) << std::endl;
        }
        double t2 = timer.seconds();
        std::cout << "\tFinished cursor based polish output in " << t2 << " seconds. Wrote " << stream2.str().size() << " bytes." << std::endl;


        std::cout << "\tStarting cursor and stack based polish output." << std::endl;
        timer.restart();
        std::ostringstream stream3;
        for( auto& tree : population )
        {
            write_tree_stack( stream3 , tree , "|" );
            stream3 << std::endl;
        }
        double t3 = timer.seconds();
        std::cout << "\tFinished cursor and stack based polish output in " << t3 << " seconds. Wrote " << stream2.str().size() << " bytes." << std::endl;


//         std::cout << "\tStarting stack based range polish output." << std::endl;
//         timer.restart();
//         std::ostringstream stream4;
//         for( auto& tree : population )
//         {
//             auto rng = make_forward_range( tree );
//             auto first = rng.begin();
//             auto last = rng.end();
//             if( first != last )
//                 stream4 << *first++;
//             for( ; first != last ; )
//             {
//                 stream4 << "|" << *first++;
//             }
//             stream4 << std::endl;
//         }
//         double t4 = timer.seconds();
//         std::cout << "\tFinished stack based range polish output in " << t4 << " seconds. Wrote " << stream4.str().size() << " bytes." << std::endl;


        bool equal = ( ( stream1.str() == stream2.str() ) && ( stream2.str() == stream3.str() ) /* && ( stream3.str() == stream4.str() ) */ );
        std::cout << "\tOutput of all versions is equal: " << ( equal ? "yes" : "no" ) << std::endl;
        
        fout << height << " " << population_size << " " << t1 << " " << t2 << " " << t3 << /* " " << t4 << */ std::endl;
    }

    return 0;
}
