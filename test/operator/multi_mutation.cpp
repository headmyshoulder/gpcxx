/*
 * test/operator/multi_mutation.cpp
 * Date: 2016-02-02
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/operator/multi_mutation.hpp>
#include <gpcxx/operator/simple_mutation_strategy.hpp>
#include <gpcxx/operator/random_selector.hpp>

#include "../common/test_template.hpp"

#include <gtest/gtest.h>

template <class T>
struct multi_mutation_tests : public test_template< T > { };

using testing::Types;

typedef Types< basic_tree_tag , intrusive_tree_tag > Implementations;

TYPED_TEST_CASE( multi_mutation_tests , Implementations );

TYPED_TEST( multi_mutation_tests , instanciation )
{
    std::vector< std::array< typename TestFixture::tree_type , 3 > > pop( 10 );
    std::vector< double > fitness( 10 );
    auto m = gpcxx::make_multi_mutation(
        this->m_gen.rng ,
        gpcxx::make_simple_mutation_strategy( this->m_gen.rng , this->m_gen.node_generator ) ,
        gpcxx::make_random_selector( this->m_gen.rng ) );
    auto mutated_nodes = m( pop , fitness );
    EXPECT_EQ( mutated_nodes.size() , size_t( 1 ) );
}
