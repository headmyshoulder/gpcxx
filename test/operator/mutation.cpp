/*
 * mutation.cpp
 * Date: 2013-02-20
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include <gp/operator/mutation.hpp>
#include <gp/operator/simple_mutation_strategy.hpp>
#include <gp/operator/random_selector.hpp>
#include "../common/test_tree.hpp"
#include "../common/test_generator.hpp"

#include <gtest/gtest.h>

#include <iostream>

#define TESTNAME mutation_tests

using namespace std;

TEST( TESTNAME , instanciation )
{
    test_tree< basic_tree_tag > tree;
    test_generator gen;
    std::vector< test_tree< basic_tree_tag >::tree_type > pop( 10 , test_tree< basic_tree_tag >::tree_type() );
    std::vector< double > fitness( 10 );
    auto m = gp::make_mutation( gp::make_simple_mutation_strategy( gen.rng , gen.gen0 , gen.gen1 , gen.gen2 ) , gp::make_random_selector( gen.rng ) );
    m( pop , fitness );
}
