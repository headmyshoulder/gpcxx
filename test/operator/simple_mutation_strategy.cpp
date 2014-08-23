/*
 test/operator/simple_mutation_strategy.cpp

 Copyright 2013 Karsten Ahnert

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/operator/simple_mutation_strategy.hpp>
#include "../common/test_template.hpp"

#include <gtest/gtest.h>

template <class T>
struct simple_mutation_strategy_tests : public test_template< T > { };

using testing::Types;

typedef Types< basic_tree_tag , intrusive_tree_tag > Implementations;

TYPED_TEST_CASE( simple_mutation_strategy_tests , Implementations );

TYPED_TEST( simple_mutation_strategy_tests , instanciation )
{
    auto strategy = gpcxx::make_simple_mutation_strategy( this->m_gen.rng , this->m_gen.node_generator );
    strategy( this->m_test_trees.data );
}
