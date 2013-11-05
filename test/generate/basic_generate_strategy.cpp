/*
 * basic_generate_strategy.cpp
 * Date: 2013-02-21
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include <gpcxx/generate/basic_generate_strategy.hpp>
#include <gpcxx/tree/basic_tree.hpp>

#include "../common/test_template.hpp"

#include <gtest/gtest.h>

template <class T>
struct crossover_tests : public test_template< T > { };

using testing::Types;

typedef Types< basic_tree_tag , intrusive_tree_tag > Implementations;

TYPED_TEST_CASE( basic_generate_strategy_tests , Implementations );

TYPED_TEST( basic_generate_strategy_tests , instanciation )
{
    std::array< double , 3 > weights{{ 1.0 , 1.0 , 1.0 }};

    for( size_t i=0 ; i<1000 ; ++i )
    {
        typename TestFixture::tree_type tree;
        
        auto generator = gpcxx::make_basic_generate_strategy( this->m_gen.rng , this->m_gen.gen0 , this->m_gen.gen1 , this->m_gen.gen2 , 2 , 4 , weights );
        generator( tree );
        EXPECT_TRUE( tree.root().height() >= 2 );
        EXPECT_TRUE( tree.root().height() <= 4 );
    }
}

TYPED_TEST( basic_generate_strategy_tests , generate_radnom_linked_tree_test2 )
{
    for( size_t i=0 ; i<1000 ; ++i )
    {
        typename TestFixture::tree_type tree;
        
        auto generator = gpcxx::make_basic_generate_strategy( this->m_gen.rng , this->m_gen.gen0 , this->m_gen.gen1 , this->m_gen.gen2 , 2 , 4 );
        generator( tree );
        EXPECT_TRUE( tree.root().height() >= 2 );
        EXPECT_TRUE( tree.root().height() <= 4 );
    }
}
