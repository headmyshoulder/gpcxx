/*
 * one_point_crossover_strategy.cpp
 * Date: 2013-02-20
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include <gp/operator/one_point_crossover_strategy.hpp>
#include "../common/test_tree.hpp"
#include "../common/test_generator.hpp"

#include <gtest/gtest.h>

using namespace std;

TEST( operator_tests , one_point_crossover_strategy_instanciation )
{
    test_tree tree;
    test_generator gen;
    gp::crossover()( tree.data , tree.data2 , gen.rng , 10 );

}
