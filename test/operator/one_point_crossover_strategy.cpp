/*
 * one_point_crossover_strategy.cpp
 * Date: 2013-02-20
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include <gpcxx/operator/one_point_crossover_strategy.hpp>
#include "../common/test_template.hpp"

#include <gtest/gtest.h>

template <class T>
struct one_point_crossover_strategy_tests : public test_template< T > { };

using testing::Types;

typedef Types< basic_tree_tag , intrusive_tree_tag > Implementations;

TYPED_TEST_CASE( one_point_crossover_strategy_tests , Implementations );

TYPED_TEST( one_point_crossover_strategy_tests , instanciation )
{
    auto c = gpcxx::make_one_point_crossover_strategy( this->m_gen.rng , 10 );
    c( this->m_test_trees.data , this->m_test_trees.data2 );
    EXPECT_NE( this->m_test_trees.data , this->m_test_trees.data2 );
}
