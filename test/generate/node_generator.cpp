/*
 * test/generate/node_generator.cpp
 * Date: 2014-08-09
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/generate/node_generator.hpp>

#include "../common/test_generator.hpp"

#include <gtest/gtest.h>

#define TESTNAME node_generator_tests


using namespace std;
using namespace gpcxx;

struct TESTNAME : public ::testing::Test
{
    size_t m_num_trials = 1000000;
    size_t m_tolerance = 1000;
    
    test_generator m_gen;
    std::function< bool( std::string const& ) > m_is_terminal_symbol;
    std::function< bool( std::string const& ) > m_is_unary_symbol;
    std::function< bool( std::string const& ) > m_is_binary_symbol;
    
    TESTNAME( void )
    {
        m_is_terminal_symbol = [this]( std::string const& s ) -> bool {
            return std::find( m_gen.term_symbols.begin() , m_gen.term_symbols.end() , s ) != m_gen.term_symbols.end(); };
        m_is_unary_symbol = [this]( std::string const& s ) -> bool {
            return std::find( m_gen.unary_symbols.begin() , m_gen.unary_symbols.end() , s ) != m_gen.unary_symbols.end(); };
        m_is_binary_symbol = [this]( std::string const& s ) -> bool {
        return std::find( m_gen.binary_symbols.begin() , m_gen.binary_symbols.end() , s ) != m_gen.binary_symbols.end(); };
    }
    
    
    template< typename Gen >
    void check_generator( Gen const& generator , size_t index , size_t arity , double weight )
    {
        EXPECT_EQ( arity , generator.arity( index ) );
        EXPECT_DOUBLE_EQ( weight , generator.weight( index ) );
    }
    
    
    void check_occurency( size_t occ , size_t expected )
    {
        EXPECT_GE( occ , expected - m_tolerance );
        EXPECT_LT( occ , expected + m_tolerance );
    }
};



TEST_F( TESTNAME , test_construct_from_generator1 )
{
    node_generator< std::string , std::mt19937 , 1 > generator { m_gen.gen0 };
    check_generator( generator , 0 , 0 , 1.0 );
}

TEST_F( TESTNAME , test_construct_from_weighted_generator1 )
{
    node_generator< std::string , std::mt19937 , 1 > generator { { 2.0 , 0 , m_gen.gen0 } };
    check_generator( generator , 0 , 0 , 2.0 );
}

TEST_F( TESTNAME , test_construct_from_generator2 )
{
    node_generator< std::string , std::mt19937 , 2 > generator { m_gen.gen0 , m_gen.gen1 };
    check_generator( generator , 0 , 0 , 1.0 );
    check_generator( generator , 1 , 1 , 1.0 );
}

TEST_F( TESTNAME , test_construct_from_weighted_generator2 )
{
    node_generator< std::string , std::mt19937 , 2 > generator { { 2.0 , 1 , m_gen.gen1 } ,{ 3.0 , 0 , m_gen.gen0 } };
    check_generator( generator , 0 , 1 , 2.0 );
    check_generator( generator , 1 , 0 , 3.0 );
}

TEST_F( TESTNAME , test_construct_from_generator3 )
{
    node_generator< std::string , std::mt19937 , 3 > generator { m_gen.gen0 , m_gen.gen1 , m_gen.gen2 };
    check_generator( generator , 0 , 0 , 1.0 );
    check_generator( generator , 1 , 1 , 1.0 );
    check_generator( generator , 2 , 2 , 1.0 );
}

TEST_F( TESTNAME , test_construct_from_weighted_generator3 )
{
    node_generator< std::string , std::mt19937 , 3 > generator { { 2.0 , 1 , m_gen.gen1 } ,{ 3.0 , 0 , m_gen.gen0 } , { 0.5 , 2 , m_gen.gen0 } };
    check_generator( generator , 0 , 1 , 2.0 );
    check_generator( generator , 1 , 0 , 3.0 );
    check_generator( generator , 2 , 2 , 0.5 );
}

TEST_F( TESTNAME , test_getter_setter_weight )
{
    node_generator< std::string , std::mt19937 , 2 > generator { m_gen.gen0 , m_gen.gen1 };
    generator.set_weight( 0 , 2.0 );
    generator.set_weight( 1 , 0.5 );
    EXPECT_DOUBLE_EQ( 2.0 , generator.weight( 0 ) );
    EXPECT_DOUBLE_EQ( 0.5 , generator.weight( 1 ) );
}

TEST_F( TESTNAME , test_get_non_terminal_node )
{
    node_generator< std::string , std::mt19937 , 3 > generator( m_gen.gen0 , m_gen.gen1 , m_gen.gen2 );
    std::vector< std::string > nodes;
    for( size_t i=0 ; i<m_num_trials ; ++i )
        nodes.push_back( generator.get_non_terminal_node( m_gen.rng ).first );
    EXPECT_TRUE( std::all_of( nodes.begin() , nodes.end() , std::not1( m_is_terminal_symbol ) ) );
    EXPECT_FALSE( std::any_of( nodes.begin() , nodes.end() , m_is_terminal_symbol ) );
}

TEST_F( TESTNAME , test_get_node )
{
    node_generator< std::string , std::mt19937 , 3 > generator( m_gen.gen0 , m_gen.gen1 , m_gen.gen2 );
    std::vector< std::string > nodes;
    for( size_t i=0 ; i<m_num_trials ; ++i )
        nodes.push_back( generator.get_node( m_gen.rng ).first );
    
    EXPECT_TRUE( std::any_of( nodes.begin() , nodes.end() , std::not1( m_is_terminal_symbol ) ) );
    EXPECT_TRUE( std::any_of( nodes.begin() , nodes.end() , m_is_terminal_symbol ) );
}


TEST_F( TESTNAME , test_prob_non_terminal_node1 )
{
    node_generator< std::string , std::mt19937 , 3 > generator( m_gen.gen0 , m_gen.gen1 , m_gen.gen2 );
    std::vector< std::string > nodes;
    
    for( size_t i=0 ; i<m_num_trials ; ++i )
        nodes.push_back( generator.get_non_terminal_node( m_gen.rng ).first );
    

    size_t num_terminals = std::count_if( nodes.begin() , nodes.end() , m_is_terminal_symbol );
    size_t num_unaries = std::count_if( nodes.begin() , nodes.end() , m_is_unary_symbol );
    size_t num_binaries = std::count_if( nodes.begin() , nodes.end() , m_is_binary_symbol );
    
    EXPECT_EQ( num_terminals , size_t( 0 ) );
    check_occurency( num_unaries , m_num_trials / 2 );
    check_occurency( num_binaries , m_num_trials / 2 );
}

TEST_F( TESTNAME , test_prob_non_terminal_node2 )
{
    node_generator< std::string , std::mt19937 , 3 > generator( m_gen.gen0 , m_gen.gen1 , m_gen.gen2 );
    generator.set_weight( 0 , 5.0 );
    generator.set_weight( 1 , 2.0 );
    generator.set_weight( 2 , 1.0 );
    
    std::vector< std::string > nodes;
    for( size_t i=0 ; i<m_num_trials ; ++i )
        nodes.push_back( generator.get_non_terminal_node( m_gen.rng ).first );
    
    size_t num_terminals = std::count_if( nodes.begin() , nodes.end() , m_is_terminal_symbol );
    size_t num_unaries = std::count_if( nodes.begin() , nodes.end() , m_is_unary_symbol );
    size_t num_binaries = std::count_if( nodes.begin() , nodes.end() , m_is_binary_symbol );
    
    EXPECT_EQ( num_terminals , size_t( 0 ) );
    check_occurency( num_unaries , 2 * m_num_trials / 3 );
    check_occurency( num_binaries , m_num_trials / 3 );
}



TEST_F( TESTNAME , test_prob_node1 )
{
    node_generator< std::string , std::mt19937 , 3 > generator( m_gen.gen0 , m_gen.gen1 , m_gen.gen2 );
    std::vector< std::string > nodes;
    
    for( size_t i=0 ; i<m_num_trials ; ++i )
        nodes.push_back( generator.get_node( m_gen.rng ).first );
    
    size_t num_terminals = std::count_if( nodes.begin() , nodes.end() , m_is_terminal_symbol );
    size_t num_unaries = std::count_if( nodes.begin() , nodes.end() , m_is_unary_symbol );
    size_t num_binaries = std::count_if( nodes.begin() , nodes.end() , m_is_binary_symbol );
    
    check_occurency( num_terminals , m_num_trials / 3 );
    check_occurency( num_unaries , m_num_trials / 3 );
    check_occurency( num_binaries , m_num_trials / 3 );
}

TEST_F( TESTNAME , test_prob_node2 )
{
    node_generator< std::string , std::mt19937 , 3 > generator( m_gen.gen0 , m_gen.gen1 , m_gen.gen2 );
    generator.set_weight( 0 , 5.0 );
    generator.set_weight( 1 , 2.0 );
    generator.set_weight( 2 , 1.0 );
    
    std::vector< std::string > nodes;
    for( size_t i=0 ; i<m_num_trials ; ++i )
        nodes.push_back( generator.get_node( m_gen.rng ).first );
    
    size_t num_terminals = std::count_if( nodes.begin() , nodes.end() , m_is_terminal_symbol );
    size_t num_unaries = std::count_if( nodes.begin() , nodes.end() , m_is_unary_symbol );
    size_t num_binaries = std::count_if( nodes.begin() , nodes.end() , m_is_binary_symbol );
    
    check_occurency( num_terminals , 5 * m_num_trials / 8 );
    check_occurency( num_unaries , m_num_trials / 4 );
    check_occurency( num_binaries , m_num_trials / 8 );
}

TEST_F( TESTNAME , test_get_terminal )
{
    node_generator< std::string , std::mt19937 , 3 > generator( m_gen.gen0 , m_gen.gen1 , m_gen.gen2 );
    std::vector< std::string > nodes;
    
    for( size_t i=0 ; i<m_num_trials ; ++i )
        nodes.push_back( generator.get_terminal( m_gen.rng ).first );
    
    size_t num_terminals = std::count_if( nodes.begin() , nodes.end() , m_is_terminal_symbol );
    size_t num_unaries = std::count_if( nodes.begin() , nodes.end() , m_is_unary_symbol );
    size_t num_binaries = std::count_if( nodes.begin() , nodes.end() , m_is_binary_symbol );
    
    EXPECT_EQ( m_num_trials , num_terminals);
    EXPECT_EQ( size_t( 0 ) , num_unaries );
    EXPECT_EQ( size_t( 0 ) , num_binaries );

}


