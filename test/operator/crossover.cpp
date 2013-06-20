/*
 test/operator/crossover.cpp

 Copyright 2013 Karsten Ahnert

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gp/operator/crossover.hpp>
#include <gp/operator/one_point_crossover_strategy.hpp>
#include <gp/operator/random_selector.hpp>
#include "../common/test_tree.hpp"
#include "../common/test_generator.hpp"

#include <gtest/gtest.h>

using namespace std;

TEST( operator_tests , crossover_instanciation )
{
    test_tree tree;
    test_generator gen;
    std::vector< test_tree::node_type > pop( 10 , test_tree::node_type() );
    std::vector< double > fitness( 10 );
    auto c = gp::make_crossover( gp::make_one_point_crossover_strategy( gen.rng , 10 ) , gp::make_random_selector( gen.rng ) );
    c( pop , fitness );
}

