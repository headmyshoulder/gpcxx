/*
  gpcxx/util/parallel_algorithm.hpp

  Copyright 2013 Gerard Choinka

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef GPCXX_PARALLEL_ALGORITHM_HPP_DEFINED
#define GPCXX_PARALLEL_ALGORITHM_HPP_DEFINED

#include <vector>
#include <cmath>
#include <tuple>
#include <cassert>

#include <boost/range/algorithm.hpp>
#include <boost/range/adaptor/sliced.hpp>
#include <boost/range/adaptor/strided.hpp>

#if GPCXX_USE_THREAD_IMPLEMENTATION == GPCXX_STD_THREAD
    #include <thread>
    typedef std::thread gpcxx_thread_default_impl; 
    namespace gpcxx { namespace par { auto const hardware_concurrency = std::thread::hardware_concurrency; } }
#elif GPCXX_USE_THREAD_IMPLEMENTATION == GPCXX_BOOST_THREAD
    #include <boost/thread.hpp>
    typedef boost::thread gpcxx_thread_default_impl;
    namespace gpcxx { namespace par { auto const hardware_concurrency = boost::thread::hardware_concurrency; } }
#elif GPCXX_USE_THREAD_IMPLEMENTATION == GPCXX_DUMMY_THREAD
#   error "GPCXX_DUMMY_THREAD is not implemented"
#endif

    
    
namespace gpcxx {
namespace par {

std::tuple< size_t, size_t > partition( size_t n_partition,
                                        size_t partition_number,
                                        size_t rng_size );    
    
template < typename InputIterator, typename Distance >
InputIterator advance_with_copy ( InputIterator it, Distance n )
{
    std::advance( it, n );
    return it;
}



template<
    typename single_pass_range1,
    typename output_iterator,
    typename unary_operation,
    typename thread_type = gpcxx_thread_default_impl
>
output_iterator transform( single_pass_range1 const & rng,
                           output_iterator out,
                           unary_operation fun,
                           size_t number_of_threads = gpcxx::par::hardware_concurrency()
                        )
{    
    if ( boost::empty( rng ) )
        return out;

    if ( number_of_threads == 1 )
        return boost::transform( rng, out, fun ); 
   
    size_t rng_size = boost::size( rng );
    
    if( rng_size <= number_of_threads )
        number_of_threads = rng_size;
        
    std::vector< thread_type >  worker{}; worker.reserve(number_of_threads);
    
    for (size_t i = 0; i < number_of_threads; ++i)
    {
        auto parti =  partition(number_of_threads, i, rng_size );     
        auto sub_rng = rng | boost::adaptors::sliced( std::get<0>( parti ), std::get<1>( parti ) );

        std::advance( out, std::get<1>(parti) - std::get<0>(parti) );
        
        worker.emplace_back( [=](){ boost::transform( sub_rng, out, fun ); } );
    }
    for ( auto& w: worker ) w.join();
    
    return out;
}




std::tuple< size_t, size_t > partition( size_t n_partition,
                                        size_t partition_number,
                                        size_t rng_size )
{
    assert( n_partition > 0 );
    assert( n_partition > partition_number );
    if( n_partition == 1 )
        return std::tuple<size_t, size_t>{ 0, rng_size };
            

    int remainder = rng_size % n_partition;
    int partition_size = rng_size / n_partition;
    
    if( remainder == 0 )
        return std::tuple< size_t, size_t >{ partition_size * partition_number, partition_size * (partition_number + 1) };

    if ( n_partition  == ( partition_number + 1 ))
        return std::tuple< size_t, size_t >{ partition_size * partition_number, rng_size };
    
    return std::tuple< size_t, size_t >{ partition_size * partition_number, partition_size * (partition_number + 1)  };
}


template<
    typename single_pass_range1,
    typename single_pass_range2,
    typename output_iterator,
    typename binary_operation,
    typename thread_type = gpcxx_thread_default_impl
>
output_iterator transform( single_pass_range1 const & rng1,
                           single_pass_range2 const & rng2,
                           output_iterator out,
                           binary_operation fun,
                           size_t number_of_threads = gpcxx::par::hardware_concurrency()
                        )
{
    assert( boost::size(rng1) == boost::size(rng2) );
    
    if ( boost::empty( rng1 ) )
        return out;

    if ( number_of_threads == 1 )
        return boost::transform( rng1, rng2, out, fun ); 
   
    size_t rng1_size = boost::size( rng1 );
    
    if( rng1_size <= number_of_threads )
        number_of_threads = rng1_size;
        
    std::vector< thread_type >  worker{}; worker.reserve( number_of_threads );
    
    for (size_t i = 0; i < number_of_threads; ++i)
    {
        auto parti =  partition( number_of_threads, i, rng1_size );     
        auto sub_rng1 = rng1 | boost::adaptors::sliced( std::get< 0 >(parti), std::get< 1 >(parti) );
        auto sub_rng2 = rng2 | boost::adaptors::sliced( std::get< 0 >(parti), std::get< 1 >(parti) );
        
        std::advance( out, std::get< 1 >( parti ) - std::get< 0 >( parti ) );
        
        worker.emplace_back( [=](){ boost::transform( sub_rng1, sub_rng2, out, fun ); } );
    }
    for ( auto& w: worker ) w.join();
    
    return out;
}


template<
    typename single_pass_range,
    typename unary_function,
    typename thread_type = gpcxx_thread_default_impl
>
unary_function for_each( single_pass_range & rng, 
                         unary_function fun, 
                         size_t number_of_threads = gpcxx::par::hardware_concurrency()
                       )
{
    if ( boost::empty( rng ) )
        return fun;
    
    if ( number_of_threads == 1 )
        return boost::for_each( rng, fun );
       
    size_t rng_size = boost::size( rng );
    
    if( rng_size <= number_of_threads )
        number_of_threads = rng_size;
    
    std::vector< thread_type > worker{}; worker.reserve( number_of_threads );
    for (size_t i = 0; i < number_of_threads; ++i)
    {            
        auto parti =  partition( number_of_threads, i, rng_size );     
        auto sub_rng = rng | boost::adaptors::sliced( std::get< 0 >( parti ), std::get< 1 >( parti ) );

        worker.emplace_back( [=](){ boost::for_each( sub_rng, fun ); } );
    }
    for ( auto& w: worker ) w.join();
    
    return fun;
}


} // namespace par
} // namespace gpcxx


#endif // GPCXX_PARALLEL_ALGORITHM_HPP_DEFINED
class name;