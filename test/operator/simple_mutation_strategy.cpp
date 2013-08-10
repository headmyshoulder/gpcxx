/*
 test/operator/simple_mutation_strategy.cpp

 Copyright 2013 Karsten Ahnert

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/operator/simple_mutation_strategy.hpp>
#include "../common/test_tree.hpp"
#include "../common/test_generator.hpp"

#include <sstream>

#include <gtest/gtest.h>

#define TESTNAME simple_mutation_strategy_tests

using namespace std;
using namespace gpcxx;

TEST( TESTNAME , instanciaion )
{
    test_tree< basic_tree_tag > tree;
    test_generator gen;
    auto strategy = make_simple_mutation_strategy( gen.rng , gen.gen0 , gen.gen1 , gen.gen2 );
    strategy( tree.data );
}

TEST( TESTNAME , mutation1 )
{
// for( size_t i=0 ; i<100 ; ++i )
//     {
//         gpcxx::mutation::mutation_impl( root , 0 , gen.gen0 , gen.gen1 , gen.gen2 );
//         if( root.value() != "plus" ) not_equal_plus = true;
//         EXPECT_TRUE( contains( gen.binary_symbols , root.value() ) );
//     }
// 
//     bool not_equal_sin = false;
//     auto &c1 = root.children( 0 );
//     for( size_t i=0 ; i<100 ; ++i )
//     {
//         gpcxx::mutation::mutation_impl( root , 1 , gen.gen0 , gen.gen1 , gen.gen2 );
//         if( c1.value() != "sin" ) not_equal_sin = true;
//         EXPECT_TRUE( contains( gen.unary_symbols , c1.value() ) );
//     }
// 
//     bool not_equal_x = false;
//     auto &c2 = c1.children( 0 );
//     for( size_t i=0 ; i<100 ; ++i )
//     {
//         gpcxx::mutation::mutation_impl( tree.data , 2 , gen.gen0 , gen.gen1 , gen.gen2 );
//         if( c2.value() != "x" ) not_equal_x = true;
//         EXPECT_TRUE( contains( gen.term_symbols , c2.value() ) );
//     }
//     EXPECT_TRUE( not_equal_x );
}