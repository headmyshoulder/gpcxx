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
    MOCK_METHOD1( selection , population_type( selection_type const& ) );
    MOCK_METHOD0( copy , void( void ) );
    MOCK_METHOD0( move , void( void ) );
    MOCK_METHOD0( copy_assign , void( void ) );
    MOCK_METHOD0( move_assign , void( void ) );
};

struct mock_functor
{
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
        return m_mocker.selection( selection );
    }
};

TEST( TESTNAME , default_construction )
{
    any_genetic_operator_type op;
    EXPECT_FALSE( op );
}

TEST( TESTNAME , construct_from_value_copy )
{
    using namespace ::testing;
    mocker m;
    EXPECT_CALL( m , copy() )
        .Times( 1 );
    mock_functor f( m );
    any_genetic_operator_type op( f );
    EXPECT_TRUE( op );
}

TEST( TESTNAME , construct_from_value_move )
{
    using namespace ::testing;
    mocker m;
    EXPECT_CALL( m , move() )
        .Times( 1 );
    mock_functor f( m );
    any_genetic_operator_type op( std::move( f ) );
    EXPECT_TRUE( op );
}

TEST( TESTNAME , copy_construct )
{
    using namespace ::testing;
    mocker m;
    EXPECT_CALL( m , copy() )
        .Times( 2 );
    mock_functor f( m );
    any_genetic_operator_type op( f );
    any_genetic_operator_type op2( op );
    EXPECT_TRUE( op );
    EXPECT_TRUE( op2 );
}

TEST( TESTNAME , move_construct )
{
    using namespace ::testing;
    mocker m;
    EXPECT_CALL( m , copy() )
        .Times( 1 );
    mock_functor f( m );
    any_genetic_operator_type op( f );
    any_genetic_operator_type op2( std::move( op ) );
    EXPECT_FALSE( op );
    EXPECT_TRUE( op2 );
}

TEST( TESTNAME , copy_assign )
{
}

TEST( TESTNAME , move_assign )
{
}

TEST( TESTNAME , copy_value_assign )
{
}

TEST( TESTNAME( move_value_assign )
{
}

TEST( TESTNAME , op )
{
    using namespace ::testing;
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
}

TEST( TESTNAME , operation )
{
}

