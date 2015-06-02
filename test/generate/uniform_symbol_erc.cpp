/*
 test/generate/random_symbol_erc_generator.cpp

 Copyright 2013 Karsten Ahnert

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/generate/uniform_symbol_erc.hpp>

#include "../common/test_generator.hpp"

#include <sstream>

#include <gtest/gtest.h>

#define TESTNAME random_symbol_erc_generator_tests

using namespace std;

TEST( TESTNAME , TestCase )
{
    std::mt19937 rng;
    std::vector< std::string > symbols( { "x" , "y" } );
    auto gen = gpcxx::make_uniform_symbol_erc< boost::variant< std::string , double > >( symbols , 1.0 , std::normal_distribution<>( 0.0 , 1.0 ) );
    typedef decltype( gen ) generator_type;
    
    bool double_found = false;
    bool symbol_found = false;
    for( size_t i=0 ; i<100 ; ++i )
    {
        generator_type::result_type res = gen( rng );
        if( res.which() == 0 )
        {
            EXPECT_TRUE( contains( symbols , boost::get< std::string >( res ) ) );
            symbol_found = true;
        }
        if( res.which() == 1 ) double_found = true;
    }
    EXPECT_TRUE( symbol_found );
    EXPECT_TRUE( double_found );
}

