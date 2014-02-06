/*
 * gpcxx/test/common/test_functions.hpp
 * Date: 2014-02-06
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_TEST_COMMON_TEST_FUNCTIONS_HPP_INCLUDED
#define GPCXX_TEST_COMMON_TEST_FUNCTIONS_HPP_INCLUDED

#include <gpcxx/tree/detail/intrusive_cursor.hpp>
#include <gtest/gtest.h>

template< typename Cursor >
void test_cursor( Cursor n , std::string const& value , size_t arity , size_t height , size_t level )
{
    EXPECT_EQ( *n , value );
    EXPECT_EQ( n.size() , arity );
    EXPECT_EQ( n.height() , height );
    EXPECT_EQ( n.level() , level );
}

template< typename T >
void test_cursor( gpcxx::detail::intrusive_cursor< T > n , std::string const& value , size_t arity , size_t height , size_t level )
{
    EXPECT_EQ( n.node()->name() , value );
    EXPECT_EQ( n.size() , arity );
    EXPECT_EQ( n.height() , height );
    EXPECT_EQ( n.level() , level );
}


#endif // GPCXX_TEST_COMMON_TEST_FUNCTIONS_HPP_INCLUDED
