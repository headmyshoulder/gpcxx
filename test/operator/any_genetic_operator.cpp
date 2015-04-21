/*
 * test/operator/any_genetic_operator.cpp
 * Date: 2015-04-06
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */


// uncomment the following line to see the debug output
#define GPCXX_ANY_GENETIC_OPERATOR_DEBUG 1

#include <gpcxx/operator/any_genetic_operator.hpp>
#include <gpcxx/tree/basic_tree.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#define TESTNAME any_genetic_operator_tests

using population_type = std::vector< gpcxx::basic_tree< std::string > >;
using fitness_type = std::vector< double > ;
using selection_type = std::vector< population_type::const_iterator >;
using any_genetic_operator_type = gpcxx::any_genetic_operator< population_type , fitness_type >;

struct mocker
{
    MOCK_METHOD2( op , population_type( population_type const& , fitness_type const& ) );
    MOCK_METHOD2( selection , selection_type( population_type const& , fitness_type const& ) );
    MOCK_METHOD1( operation , population_type( selection_type const& ) );
    MOCK_METHOD0( copy , void( void ) );
    MOCK_METHOD0( move , void( void ) );
    MOCK_METHOD0( copy_assign , void( void ) );
    MOCK_METHOD0( move_assign , void( void ) );
};

struct mock_functor
{
    static const size_t arity = 1;
    
    mocker &m_mocker;
    mock_functor( mocker &m ) : m_mocker( m ) { }
    mock_functor( mock_functor const& m ) : m_mocker( m.m_mocker ) { m_mocker.copy(); }
    mock_functor( mock_functor&& m ) : m_mocker( m.m_mocker ) { m_mocker.move(); }
    mock_functor& operator=( mock_functor const& m ) { m_mocker.copy_assign(); return *this; }
    mock_functor& operator=( mock_functor&& m ) { m_mocker.move_assign(); return *this; }

    population_type operator()( population_type const& pop , fitness_type const& fitness )
    {
        return m_mocker.op( pop , fitness );
    }
    selection_type selection( population_type const& pop , fitness_type const& fitness )
    {
        return m_mocker.selection( pop , fitness );
    }
    population_type operation( selection_type const& selection )
    {
        return m_mocker.operation( selection );
    }
};

using namespace ::testing;

TEST( TESTNAME , default_construction )
{
    any_genetic_operator_type op;
    EXPECT_FALSE( op );
}

TEST( TESTNAME , construct_from_value_copy )
{
    mocker m;
    EXPECT_CALL( m , copy() )
        .Times( 1 );
    mock_functor f( m );
    any_genetic_operator_type op( f );
    EXPECT_TRUE( op );
    EXPECT_EQ( size_t( 1 ) , op.arity() );
}

TEST( TESTNAME , construct_from_value_move )
{
    mocker m;
    EXPECT_CALL( m , move() )
        .Times( 1 );
    mock_functor f( m );
    any_genetic_operator_type op( std::move( f ) );
    EXPECT_TRUE( op );
}

TEST( TESTNAME , copy_construct )
{
    mocker m;
    EXPECT_CALL( m , copy() )
        .Times( 2 );
    mock_functor f( m );
    any_genetic_operator_type op( f );
    any_genetic_operator_type op2( op );
    EXPECT_TRUE( op );
    EXPECT_TRUE( op2 );
}

TEST( TESTNAME , copy_construct2 )
{
    mocker m;
    EXPECT_CALL( m , copy() )
        .Times( 2 );
    mock_functor f( m );
    any_genetic_operator_type op( f );
    any_genetic_operator_type const& opref = op;
    any_genetic_operator_type op2( opref );
    EXPECT_TRUE( op );
    EXPECT_TRUE( op2 );
}

TEST( TESTNAME , copy_construct_from_empty )
{
    mocker m;
    mock_functor f( m );
    any_genetic_operator_type op;
    any_genetic_operator_type op2( op );
    EXPECT_FALSE( op );
    EXPECT_FALSE( op2 );
}

TEST( TESTNAME , move_construct )
{
    mocker m;
    EXPECT_CALL( m , copy() )
        .Times( 1 );
    mock_functor f( m );
    any_genetic_operator_type op( f );
    any_genetic_operator_type op2( std::move( op ) );
    EXPECT_FALSE( op );
    EXPECT_TRUE( op2 );
}

TEST( TESTNAME , move_construct_from_empty )
{
    mocker m;
    mock_functor f( m );
    any_genetic_operator_type op;
    any_genetic_operator_type op2( std::move( op ) );
    EXPECT_FALSE( op );
    EXPECT_FALSE( op2 );
}


TEST( TESTNAME , copy_assign )
{
    mocker m;
    EXPECT_CALL( m , copy() )
        .Times( 2 );
    mock_functor f( m );
    any_genetic_operator_type op( f );
    any_genetic_operator_type op2;
    op2 = op;
    EXPECT_TRUE( op );
    EXPECT_TRUE( op2 );
}

TEST( TESTNAME , copy_assign2 )
{
    mocker m;
    EXPECT_CALL( m , copy() )
        .Times( 2 );
    mock_functor f( m );
    any_genetic_operator_type op( f );
    any_genetic_operator_type const& opref = op;
    any_genetic_operator_type op2;
    op2 = opref;
    EXPECT_TRUE( op );
    EXPECT_TRUE( op2 );
}

TEST( TESTNAME , copy_assign_from_empty )
{
    mocker m;
    mock_functor f( m );
    any_genetic_operator_type op;
    any_genetic_operator_type op2;
    op2 = op;
    EXPECT_FALSE( op );
    EXPECT_FALSE( op2 );
}

TEST( TESTNAME , move_assign )
{
    mocker m;
    EXPECT_CALL( m , copy() )
        .Times( 1 );
    mock_functor f( m );
    any_genetic_operator_type op( f );
    any_genetic_operator_type op2;
    op2 = std::move( op );
    EXPECT_FALSE( op );
    EXPECT_TRUE( op2 );
}

TEST( TESTNAME , move_assign_from_empty )
{
    mocker m;
    mock_functor f( m );
    any_genetic_operator_type op;
    any_genetic_operator_type op2;
    op2 = std::move( op );
    EXPECT_FALSE( op );
    EXPECT_FALSE( op2 );
}


TEST( TESTNAME , copy_value_assign )
{
    mocker m;
    mock_functor f( m );
    EXPECT_CALL( m , copy() )
        .Times( 1 );
    any_genetic_operator_type op;
    op = f;
    EXPECT_TRUE( op );
}

TEST( TESTNAME , move_value_assign )
{
    mocker m;
    mock_functor f( m );
    EXPECT_CALL( m , move() )
        .Times( 1 );
    any_genetic_operator_type op;
    op = std::move( f );
    EXPECT_TRUE( op );
}

TEST( TESTNAME , op )
{
    mocker m;
    EXPECT_CALL( m , copy() )
        .Times( 1 );
    EXPECT_CALL( m , op( testing::_ , testing::_ ) )
        .Times( 1 )
        .WillOnce( Return( population_type{} ) );
    mock_functor f( m );
    any_genetic_operator_type op( f );
    EXPECT_TRUE( op );
    op( population_type {} , fitness_type {} );
}

TEST( TESTNAME , selection )
{
    mocker m;
    EXPECT_CALL( m , copy() )
        .Times( 1 );
    EXPECT_CALL( m , selection( testing::_ , testing::_ ) )
        .Times( 1 )
        .WillOnce( Return( selection_type{} ) );
    mock_functor f( m );
    any_genetic_operator_type op( f );
    EXPECT_TRUE( op );
    op.selection( population_type {} , fitness_type {} );
}

TEST( TESTNAME , operation )
{
    mocker m;
    EXPECT_CALL( m , copy() )
        .Times( 1 );
    EXPECT_CALL( m , operation( testing::_ ) )
        .Times( 1 )
        .WillOnce( Return( population_type{} ) );
    mock_functor f( m );
    any_genetic_operator_type op( f );
    EXPECT_TRUE( op );
    op.operation( selection_type {} );
}

