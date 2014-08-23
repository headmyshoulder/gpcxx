/*
 * basic_generate_strategy.cpp
 * Date: 2013-02-21
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include <gpcxx/generate/basic_generate_strategy.hpp>

#include "../common/test_template.hpp"

#include <gtest/gtest.h>

template <class T>
struct basic_generate_strategy_tests : public test_template< T > { };

using testing::Types;

typedef Types< basic_tree_tag , intrusive_tree_tag > Implementations;

TYPED_TEST_CASE( basic_generate_strategy_tests , Implementations );

TYPED_TEST( basic_generate_strategy_tests , instanciation )
{
    for( size_t i=0 ; i<1000 ; ++i )
    {
        typename TestFixture::tree_type tree;
        
        auto generator = gpcxx::make_basic_generate_strategy( this->m_gen.rng , this->m_gen.node_generator , 2 , 4 );
        generator( tree );
        EXPECT_TRUE( tree.root().height() >= 2 );
        EXPECT_TRUE( tree.root().height() <= 4 );
    }
}
