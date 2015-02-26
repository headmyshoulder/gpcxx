/*
 * one_point_crossover_strategy.cpp
 * Date: 2013-02-20
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include <gpcxx/operator/one_point_crossover_strategy.hpp>
#include "../common/test_template.hpp"

#include <gtest/gtest.h>


// debugging
#include <gpcxx/io/simple.hpp>
#include <iostream>
using namespace std;


template <class T>
struct one_point_crossover_strategy_tests : public test_template< T > { };

using testing::Types;

// typedef Types< intrusive_tree_tag > Implementations;
typedef Types< basic_tree_tag , intrusive_tree_tag > Implementations;

TYPED_TEST_CASE( one_point_crossover_strategy_tests , Implementations );

TYPED_TEST( one_point_crossover_strategy_tests , instanciation )
{
    ASSERT_NO_THROW( 
    for( size_t i=0 ; i<10 ; ++i )
    {
        auto c = gpcxx::make_one_point_crossover_strategy( this->m_gen.rng , 10 );
        size_t l1 = this->m_test_trees.data.size() + this->m_test_trees.data2.size();
        cerr << this->m_test_trees.data.size() << " " << simple( this->m_test_trees.data ) << "\n";
        cerr << this->m_test_trees.data2.size() << " " << simple( this->m_test_trees.data2 ) << "\n\n";
        c( this->m_test_trees.data , this->m_test_trees.data2 );
        cerr << this->m_test_trees.data.size() << " " << simple( this->m_test_trees.data ) << "\n";
        cerr << this->m_test_trees.data2.size() << " " << simple( this->m_test_trees.data2 ) << "\n";
        cerr << endl;
        EXPECT_NE( this->m_test_trees.data , this->m_test_trees.data2 );
        EXPECT_EQ( l1 , this->m_test_trees.data.size() + this->m_test_trees.data2.size() );
    }
    );
}

TYPED_TEST( one_point_crossover_strategy_tests , heights )
{
    ASSERT_NO_THROW(
    for( size_t i=0 ; i<10 ; ++i )
    {
        auto c = gpcxx::make_one_point_crossover_strategy( this->m_gen.rng , 5 );
        cerr << this->m_test_trees.data.size() << " " << simple( this->m_test_trees.data ) << "\n";
        cerr << this->m_test_trees.data2.size() << " " << simple( this->m_test_trees.data2 ) << "\n\n";
        c( this->m_test_trees.data , this->m_test_trees.data2 );
        cerr << this->m_test_trees.data.size() << " " << simple( this->m_test_trees.data ) << "\n";
        cerr << this->m_test_trees.data2.size() << " " << simple( this->m_test_trees.data2 ) << "\n";
        cerr << endl;
        EXPECT_LE( this->m_test_trees.data.root().height() , 5 );
        EXPECT_LE( this->m_test_trees.data2.root().height() , 5 );
    }
    );
}

