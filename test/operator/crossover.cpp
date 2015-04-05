/*
 test/operator/crossover.cpp

 Copyright 2013 Karsten Ahnert

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/operator/crossover.hpp>
#include <gpcxx/operator/one_point_crossover_strategy.hpp>
#include <gpcxx/operator/random_selector.hpp>

#include "../common/test_template.hpp"

#include <gtest/gtest.h>

template <class T>
struct crossover_tests : public test_template< T > { };

using testing::Types;

typedef Types< basic_tree_tag , intrusive_tree_tag > Implementations;

TYPED_TEST_CASE( crossover_tests , Implementations );

TYPED_TEST( crossover_tests , instanciation )
{
    std::vector< typename TestFixture::tree_type > pop( 10 , typename TestFixture::tree_type() );
    std::vector< double > fitness( 10 );
    auto c = gpcxx::make_crossover(
        gpcxx::make_one_point_crossover_strategy( this->m_gen.rng , 10 ) ,
        gpcxx::make_random_selector( this->m_gen.rng ) );
    auto crossover_nodes = c( pop , fitness );
    EXPECT_EQ( crossover_nodes.size() , size_t( 2 ) );
}

