/*
 * generate_random_linked_tree.cpp
 * Date: 2013-02-21
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include <gp/tree/linked_node.hpp>
#include <gp/tree/generate_random_linked_tree.hpp>

#include "../common/test_generator.hpp"

#include <random>

#include <gtest/gtest.h>


TEST( tree_tests , generate_radnom_linked_tree_test1 )
{
    test_generator gen;

    std::mt19937 rng;

    for( size_t i=0 ; i<1000 ; ++i )
    {
        gp::linked_node< std::string > tree;
        gp::generate_random_linked_tree tree_gen;
        tree_gen( tree , gen.gen0 , gen.gen1 , gen.gen2 , rng , 2 , 4 );
        EXPECT_TRUE( tree.height() >= 2 );
        EXPECT_TRUE( tree.height() <= 4 );
    }
}

