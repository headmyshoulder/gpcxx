/*
 * test/app/benchmark_problems.cpp
 * Date: 2015-06-24
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/app/benchmark_problems/keijzer.hpp>
#include <gpcxx/app/benchmark_problems/koza.hpp>
#include <gpcxx/app/benchmark_problems/korns.hpp>
#include <gpcxx/app/benchmark_problems/pagie.hpp>
#include <gpcxx/app/benchmark_problems/vladislavleva.hpp>
#include <gpcxx/util/macros.hpp>

#include <gtest/gtest.h>
#include <random>

#define TESTNAME benchmark_problems_tests

using namespace std;
using namespace gpcxx;

#define CREATE_INSTANTIATION_TEST( func )                                  \
TEST( TESTNAME , func )                                                    \
{                                                                          \
    auto f = func ();                                                      \
    GPCXX_UNUSED_VAR( f );                                                 \
}

#define CREATE_INSTANTIATION_TEST_RNG( func )                              \
TEST( TESTNAME , func )                                                    \
{                                                                          \
    std::mt19937 rng;                                                      \
    auto f = func ( rng );                                                 \
    GPCXX_UNUSED_VAR( f );                                                 \
}


CREATE_INSTANTIATION_TEST_RNG( generate_koza1 )
CREATE_INSTANTIATION_TEST_RNG( generate_koza2 )
CREATE_INSTANTIATION_TEST_RNG( generate_koza3 )

CREATE_INSTANTIATION_TEST( generate_pagie1 )
CREATE_INSTANTIATION_TEST( generate_pagie2 )

CREATE_INSTANTIATION_TEST( generate_keijzer1 )
CREATE_INSTANTIATION_TEST( generate_keijzer2 )
CREATE_INSTANTIATION_TEST( generate_keijzer3 )
CREATE_INSTANTIATION_TEST( generate_keijzer4 )
CREATE_INSTANTIATION_TEST_RNG( generate_keijzer5 )
CREATE_INSTANTIATION_TEST( generate_keijzer6 )
CREATE_INSTANTIATION_TEST( generate_keijzer7 )
CREATE_INSTANTIATION_TEST( generate_keijzer8 )
CREATE_INSTANTIATION_TEST( generate_keijzer9 )
CREATE_INSTANTIATION_TEST_RNG( generate_keijzer10 )
CREATE_INSTANTIATION_TEST_RNG( generate_keijzer11 )
CREATE_INSTANTIATION_TEST_RNG( generate_keijzer12 )
CREATE_INSTANTIATION_TEST_RNG( generate_keijzer13 )
CREATE_INSTANTIATION_TEST_RNG( generate_keijzer14 )
CREATE_INSTANTIATION_TEST_RNG( generate_keijzer15 )

CREATE_INSTANTIATION_TEST_RNG( generate_korns1 )
CREATE_INSTANTIATION_TEST_RNG( generate_korns2 )
CREATE_INSTANTIATION_TEST_RNG( generate_korns3 )
CREATE_INSTANTIATION_TEST_RNG( generate_korns4 )
CREATE_INSTANTIATION_TEST_RNG( generate_korns5 )
CREATE_INSTANTIATION_TEST_RNG( generate_korns6 )
CREATE_INSTANTIATION_TEST_RNG( generate_korns7 )
CREATE_INSTANTIATION_TEST_RNG( generate_korns8 )
CREATE_INSTANTIATION_TEST_RNG( generate_korns9 )
CREATE_INSTANTIATION_TEST_RNG( generate_korns10 )
CREATE_INSTANTIATION_TEST_RNG( generate_korns11 )
CREATE_INSTANTIATION_TEST_RNG( generate_korns12 )
CREATE_INSTANTIATION_TEST_RNG( generate_korns13 )
CREATE_INSTANTIATION_TEST_RNG( generate_korns14 )
CREATE_INSTANTIATION_TEST_RNG( generate_korns15 )

CREATE_INSTANTIATION_TEST_RNG( generate_vladislavleva1 )
CREATE_INSTANTIATION_TEST( generate_vladislavleva2 )
CREATE_INSTANTIATION_TEST( generate_vladislavleva3 )
CREATE_INSTANTIATION_TEST_RNG( generate_vladislavleva4 )
CREATE_INSTANTIATION_TEST_RNG( generate_vladislavleva5 )
CREATE_INSTANTIATION_TEST_RNG( generate_vladislavleva6 )
CREATE_INSTANTIATION_TEST_RNG( generate_vladislavleva7 )
CREATE_INSTANTIATION_TEST_RNG( generate_vladislavleva8 )
