/*
 * mutation.cpp
 * Date: 2013-02-20
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include <gpcxx/operator/mutation.hpp>
#include <gpcxx/operator/simple_mutation_strategy.hpp>
#include <gpcxx/operator/random_selector.hpp>
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
    auto m = gpcxx::make_mutation( gpcxx::make_simple_mutation_strategy( gen.rng , gen.gen0 , gen.gen1 , gen.gen2 ) , gpcxx::make_random_selector( gen.rng ) );
    m( pop , fitness );
}
