/*
 * parallel_algorithm.cpp
 * Date: 2013-12-10
 * Author: Gerard Choinka (gerard.choinka@ambrosys.de)
 */

#include <gpcxx/util/parallel_algorithm.hpp>
#include <boost/range.hpp>
#include <boost/range/irange.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>
#include <algorithm>
#include <cstdint>

#include <gtest/gtest.h>

using namespace std;

#define TESTNAME parallel_algorithm

TEST( TESTNAME , transform_binary )
{
    size_t thread_count[] = { 2, 3, 4, 5, 7, 8, 9, 10 };
    auto rng_size_min = 1, rng_size_max = 21, rng_size_step=1;
    
    for( auto const nthreads : thread_count )
    {
        for( auto i = rng_size_min; i < rng_size_max; i += rng_size_step)
        {
            int k = 0;
            vector<size_t> rng1; boost::generate( rng1, [&](){ return k++; } );
            k=0;
            vector<size_t> rng2; boost::generate( rng2, [&](){ return k++; } );
            
           
            auto const fun = [](size_t a, size_t b) { return a * b; };
            std::vector<int> out_singel_thread_boost ( boost::size( rng1 ), 0 );
            std::vector<int> out_n_thread_gpcxx_par ( boost::size( rng1 ), 1 );
            std::vector<int> out_singel_thread_std ( boost::size( rng1 ), 3 );
            
            boost::transform( rng1, rng2, boost::begin( out_singel_thread_boost ), fun );
            gpcxx::par::transform( rng1, rng2, boost::begin( out_n_thread_gpcxx_par ), fun, nthreads );
            std::transform( boost::begin( rng1 ), boost::end( rng1 ), boost::begin( rng2 ), boost::begin( out_singel_thread_std ), fun );
            EXPECT_EQ( out_singel_thread_boost , out_n_thread_gpcxx_par );
            EXPECT_EQ( out_singel_thread_std   , out_n_thread_gpcxx_par );            
        }
    }
}

TEST( TESTNAME , transform_unary )
{
    size_t thread_count[] = { 2, 3, 4, 5, 7, 8, 9, 10 };
    auto rng_size_min = 1, rng_size_max = 21, rng_size_step=1;
    
    for( auto const nthreads : thread_count )
    {
        for( auto i = rng_size_min; i < rng_size_max; i += rng_size_step)
        {
            int k = 0;
            vector<size_t> rng1; boost::generate( rng1, [&](){ return k++; } );
            
           
            auto const fun = [](size_t a) { return a * a; };
            std::vector<int> out_singel_thread_boost ( boost::size( rng1 ), 0 );
            std::vector<int> out_n_thread_gpcxx_par ( boost::size( rng1 ), 1 );
            std::vector<int> out_singel_thread_std ( boost::size( rng1 ), 3 );
            
            boost::transform( rng1, boost::begin( out_singel_thread_boost ), fun );
            gpcxx::par::transform( rng1, boost::begin( out_n_thread_gpcxx_par ), fun, nthreads );
            std::transform( boost::begin( rng1 ), boost::end( rng1 ), boost::begin( out_singel_thread_std ), fun );
            EXPECT_EQ( out_singel_thread_boost , out_n_thread_gpcxx_par );
            EXPECT_EQ( out_singel_thread_std   , out_n_thread_gpcxx_par );            
        }
    }
}