/*
 * test/tree/transform_tree.cpp
 * Date: 2015-08-12
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "../common/test_tree.hpp"
#include <gpcxx/tree/transform_tree.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>



#define TESTNAME transform_tree_tests

using namespace std;
using namespace ::testing;

using tree_type = get_tree_type< basic_nary_tree_tag >::type;
using cursor = tree_type::cursor;
using rule_type = std::function< gpcxx::rule_result( tree_type& , cursor ) >;

struct rule_mocker
{
    MOCK_METHOD2( operation , gpcxx::rule_result( tree_type& , cursor ) );
};

struct mock_rule
{
    rule_mocker& m_mocker;
    mock_rule( rule_mocker& mocker ) : m_mocker( mocker ) {}
    gpcxx::rule_result operator()( tree_type& tree , cursor c ) const
    {
        return m_mocker.operation( tree , c );
    }
};

TEST( TESTNAME , simple_test )
{
    test_tree< basic_nary_tree_tag > trees;
    
    rule_mocker mocker;
    EXPECT_CALL( mocker , operation(_,_) )
        .WillRepeatedly( Return( descent ) );
    
    std::vector< rule_type > rules = { mock_rule { mocker } };
    gpcxx::transform_tree( rules , trees.data );
}

TEST( TESTNAME , test_2 )
{
    test_tree< basic_nary_tree_tag > trees;
    
    rule_mocker mocker;
    EXPECT_CALL( mocker , operation(_,_) )
        .WillRepeatedly( Return( ascent ) );
    
    std::vector< rule_type > rules = { mock_rule { mocker } };
    EXPECT_THROW( { gpcxx::transform_tree( rules , trees.data ); } , gpcxx::gpcxx_exception );
}
