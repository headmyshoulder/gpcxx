/*
 * mutation.cpp
 * Date: 2013-02-20
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include <gpcxx/operator/mutation.hpp>
#include <gpcxx/operator/simple_mutation_strategy.hpp>
#include <gpcxx/operator/random_selector.hpp>

#include "../common/test_template.hpp"

#include <gtest/gtest.h>

template <class T>
struct mutation_tests : public test_template< T > { };

using testing::Types;

typedef Types< basic_tree_tag , intrusive_tree_tag > Implementations;

TYPED_TEST_CASE( mutation_tests , Implementations );

TYPED_TEST( mutation_tests , instanciation )
{
    std::vector< typename TestFixture::tree_type > pop( 10 , typename TestFixture::tree_type() );
    std::vector< double > fitness( 10 );
    auto m = gpcxx::make_mutation(
        gpcxx::make_simple_mutation_strategy( this->m_gen.rng , this->m_gen.gen0 , this->m_gen.gen1 , this->m_gen.gen2 ) ,
        gpcxx::make_random_selector( this->m_gen.rng ) );
    m( pop , fitness );
}
