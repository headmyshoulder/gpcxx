/*
 * parallel_algorithm.cpp
 * Date: 2013-12-10
 * Author: Gerard Choinka (gerard.choinka@ambrosys.de)
 */

#include <gpcxx/util/parallel_algorithm.hpp>
#include <boost/range.hpp>
#include <boost/range/irange.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>
#include <boost/range/algorithm.hpp>
#include <algorithm>
#include <cstdint>
#include <mutex>

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
            gpcxx::par::transform2( rng1, rng2, boost::begin( out_n_thread_gpcxx_par ), fun, nthreads );
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
            vector<size_t> rng; boost::generate( rng, [&](){ return k++; } );
            
           
            auto const fun = [](size_t a) { return a * a; };
            std::vector<int> out_singel_thread_boost ( boost::size( rng ), 0 );
            std::vector<int> out_n_thread_gpcxx_par ( boost::size( rng ), 1 );
            std::vector<int> out_singel_thread_std ( boost::size( rng ), 3 );
            
            boost::transform( rng, boost::begin( out_singel_thread_boost ), fun );
            gpcxx::par::transform( rng, boost::begin( out_n_thread_gpcxx_par ), fun, nthreads );
            std::transform( boost::begin( rng ), boost::end( rng ), boost::begin( out_singel_thread_std ), fun );
            EXPECT_EQ( out_singel_thread_boost , out_n_thread_gpcxx_par );
            EXPECT_EQ( out_singel_thread_std   , out_n_thread_gpcxx_par );            
        }
    }
}

TEST( TESTNAME , for_each )
{
    size_t thread_count[] = { 2, 3, 4, 5, 7, 8, 9, 10 };
    auto rng_size_min = 1, rng_size_max = 21, rng_size_step=1;
    
    for( auto const nthreads : thread_count )
    {
        for( auto i = rng_size_min; i < rng_size_max; i += rng_size_step)
        {
            int k = 0;
            vector<size_t> rng; 
            boost::generate( rng, [&](){ return k++; } );
                    
            size_t sum_singel_thread_boost = 0;
            size_t sum_n_thread_gpcxx_par = 0;
            size_t sum_singel_thread_std = 0;
            std::mutex mu;
            
            auto const fun = make_tuple(
                [&](size_t a) { sum_singel_thread_boost += a; },
                [&](size_t a) { std::lock_guard<std::mutex> lg(mu); sum_n_thread_gpcxx_par += a; },
                [&](size_t a) { sum_singel_thread_std += a; }
                             );
            
            
            
            boost::for_each( rng, std::get<0>(fun) );
            gpcxx::par::for_each( rng, std::get<1>(fun), nthreads );
            std::for_each( boost::begin( rng ), boost::end( rng ), std::get<2>(fun) );
            EXPECT_EQ( sum_singel_thread_boost , sum_n_thread_gpcxx_par );
            EXPECT_EQ( sum_singel_thread_std   , sum_n_thread_gpcxx_par );  
        }
    }
}
