/*
 test/util/iterate_until.cpp

 Copyright 2013 Karsten Ahnert

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/util/iterate_until.hpp>

#include <boost/fusion/include/make_vector.hpp>


#include <sstream>

#include <gtest/gtest.h>

#define TESTNAME iterate_until_tests

using namespace std;
namespace fusion = boost::fusion;

template< typename TypeToFind >
struct test_visitor
{
    TypeToFind m_to_find;
    
    test_visitor( TypeToFind const& to_find ) : m_to_find( to_find ) { }
    
    template< typename T >
    bool operator()( T const& t ) const
    {
        return false;
    }
    
    bool operator()( TypeToFind const &t ) const
    {
        return ( t == m_to_find );
    }
};

template< typename T >
test_visitor< T > make_test_visitor( T const& t )
{
    return test_visitor< T >( t );
}


struct test_struct { };

TEST( TESTNAME , test1 )
{
    auto vec = fusion::make_vector( std::string( "b" ) , std::string( "a" ) , std::string( "d" ) , 10 , 20 );
    EXPECT_TRUE( gpcxx::iterate_until( vec , make_test_visitor( 10 ) ) );
    EXPECT_TRUE( gpcxx::iterate_until( vec , make_test_visitor( 20 ) ) );
    EXPECT_FALSE( gpcxx::iterate_until( vec , make_test_visitor( 11 ) ) );
    EXPECT_TRUE( gpcxx::iterate_until( vec , make_test_visitor( std::string( "a" ) ) ) );
    EXPECT_TRUE( gpcxx::iterate_until( vec , make_test_visitor( std::string( "b" ) ) ) );
    EXPECT_TRUE( gpcxx::iterate_until( vec , make_test_visitor( std::string( "d" ) ) ) );
    EXPECT_FALSE( gpcxx::iterate_until( vec , make_test_visitor( std::string( "x" ) ) ) );
    EXPECT_FALSE( gpcxx::iterate_until( vec , make_test_visitor( test_struct() ) ) );
}

