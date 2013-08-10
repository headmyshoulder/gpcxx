/*
 test/operator/reproduce.cpp

 Copyright 2013 Karsten Ahnert

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/operator/reproduce.hpp>
#include <gpcxx/operator/random_selector.hpp>
#include "../common/test_tree.hpp"
#include "../common/test_generator.hpp"

#include <gtest/gtest.h>

#define TESTNAME reproduce_tests

using namespace std;

TEST( TESTNAME , instanciation )
{
    test_generator gen;
    std::vector< test_tree< basic_tree_tag >::tree_type > pop( 10 , test_tree< basic_tree_tag >::tree_type() );
    std::vector< double > fitness( 10 );
    auto selector = gpcxx::make_random_selector( gen.rng );
    auto node = selector( pop , fitness );
}

