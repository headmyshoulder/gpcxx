/*
 test/operator/reproduce.cpp

 Copyright 2013 Karsten Ahnert

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/operator/reproduce.hpp>
#include <gpcxx/operator/random_selector.hpp>

#include "../common/test_template.hpp"

#include <gtest/gtest.h>

template <class T>
struct reproduce_tests : public test_template< T > { };

using testing::Types;

typedef Types< basic_tree_tag , intrusive_tree_tag > Implementations;

TYPED_TEST_CASE( reproduce_tests , Implementations );

TYPED_TEST( reproduce_tests , instanciation )
{
    std::vector< typename TestFixture::tree_type > pop( 10 , typename TestFixture::tree_type() );
    std::vector< double > fitness( 10 );
    auto selector = gpcxx::make_random_selector( this->m_gen.rng );
    auto c = gpcxx::make_reproduce( selector );
    auto nodes = c( pop , fitness );
    EXPECT_EQ( nodes.size() , size_t( 1 ) );
}

