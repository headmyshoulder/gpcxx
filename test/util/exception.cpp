/*
 * test/util/exception.cpp
 * Date: 2015-02-13
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/util/exception.hpp>

#include <gtest/gtest.h>

#define TESTNAME exception_tests

void throw_gpcxx_exception( void )
{
    throw gpcxx::gpcxx_exception { "error" };
}

TEST( TESTNAME , construct_gpcxx_exception )
{
    gpcxx::gpcxx_exception e { "Hello exception!" };
    EXPECT_STREQ( e.what() , "Hello exception!" );
}

TEST( TESTNAME , throw_gpcxx_exception )
{
    EXPECT_THROW( throw_gpcxx_exception() , gpcxx::gpcxx_exception );
    EXPECT_THROW( throw_gpcxx_exception() , std::exception );
}
