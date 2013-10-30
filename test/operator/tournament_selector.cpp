/*
 test/operator/tournament_selector.cpp

 Copyright 2013 Karsten Ahnert

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/operator/tournament_selector.hpp>
#include "../common/test_template.hpp"

#include <gtest/gtest.h>

template <class T>
struct tournament_selector_tests : public test_template< T > { };

using testing::Types;

typedef Types< basic_tree_tag , intrusive_tree_tag > Implementations;

TYPED_TEST_CASE( tournament_selector_tests , Implementations );

TYPED_TEST( tournament_selector_tests , instanciation )
{
    std::vector< typename TestFixture::tree_type > pop( 10 , typename TestFixture::tree_type() );
    std::vector< double > fitness( 10 );
    auto selector = gpcxx::make_tournament_selector( this->m_gen.rng , 7 );
    auto node = selector( pop , fitness );
}

