/*
 test/util/version.cpp

 Copyright 2013 Karsten Ahnert

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gp/util/version.hpp>

#include <sstream>

#include <gtest/gtest.h>

#define TESTNAME version_tests

using namespace std;

TEST( TESTNAME , instanciation )
{
    std::string version = gp::get_version_string();
    int major = gp::get_version_major();
    int minor = gp::get_version_minor();
    int patch = gp::get_version_patch();
    std::string vsha1 = gp::get_version_sha1();
    
    EXPECT_EQ( major , GP_VERSION_MAJOR );
    EXPECT_EQ( minor , GP_VERSION_MINOR );
    EXPECT_EQ( patch , GP_VERSION_PATCH );
    EXPECT_EQ( vsha1 , "GP_VERSION_SHA1" );
}

