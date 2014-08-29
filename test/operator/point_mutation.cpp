/*
 * test/operator/point_mutation.cpp
 * Date: 2013-09-13
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/operator/point_mutation.hpp>
#include <gpcxx/generate/basic_generate_strategy.hpp>
#include <gpcxx/io/simple.hpp>

#include "../common/test_template.hpp"

#include <gtest/gtest.h>

template <class T>
struct point_mutation_tests : public test_template< T > { };

using testing::Types;
using namespace gpcxx;

typedef Types< basic_tree_tag , intrusive_tree_tag > Implementations;

TYPED_TEST_CASE( point_mutation_tests , Implementations );

TYPED_TEST( point_mutation_tests , instanciation )
{
    auto generator = make_basic_generate_strategy( this->m_gen.rng , this->m_gen.node_generator , 1 , 5 );
    auto strategy = make_point_mutation( this->m_gen.rng , generator , 5 , 128 );

    for( size_t i=0 ; i<1000 ; ++i )
    {
        typename TestFixture::tree_type tree;
        generator( tree );
        strategy( tree );
        EXPECT_LE( tree.root().height() , 5 );
    }
}
