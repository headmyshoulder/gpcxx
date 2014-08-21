/*
 * test/generate/node_generator_generate_strategy.cpp
 * Date: 2014-08-11
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/generate/node_generator_generate_strategy.hpp>

#include "../common/test_template.hpp"

#include <gtest/gtest.h>

using namespace std;
using testing::Types;

template< typename T >
class node_generator_generate_strategy_tests : public test_template< T  >
{
};



typedef Types< basic_tree_tag , intrusive_tree_tag > Implementations;

TYPED_TEST_CASE( node_generator_generate_strategy_tests , Implementations );


TYPED_TEST( node_generator_generate_strategy_tests , test_instanciation )
{
    for( size_t i=0 ; i<1000 ; ++i )
    {
        typename TestFixture::tree_type tree;
        auto generator = make_node_generator_generate_strategy( this->m_gen.rng , this->m_gen.node_generator , 2 , 4 );
        generator( tree );
        EXPECT_TRUE( tree.root().height() >= 2 );
        EXPECT_TRUE( tree.root().height() <= 4 );
    }
}
