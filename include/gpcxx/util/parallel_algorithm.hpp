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
#include <thread>

#include <boost/range/algorithm.hpp>
#include <boost/range/adaptor/sliced.hpp>
#include <boost/range/adaptor/strided.hpp>



    
    
namespace gpcxx {
namespace par {
 
namespace adaptors {
    namespace detail {
        
        inline std::tuple< size_t, size_t > partition_bounds( size_t n_partition,
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

        inline size_t partition_upper_bound( size_t n_partition, size_t partition_number, size_t rng_size )
        {
            return std::get<1>( partition_bounds( n_partition, partition_number, rng_size) );
        }

        inline size_t partition_lower_bound( size_t n_partition, size_t partition_number, size_t rng_size )
        {
            return std::get<0>( partition_bounds( n_partition, partition_number, rng_size) );
        }
    }//namespace detail

    template<typename RandomAccessRange>
    auto partition(RandomAccessRange & rng, size_t n_partition, size_t partition_number, size_t rng_size) 
    -> decltype( boost::adaptors::slice( 
        rng,  
        detail::partition_lower_bound( n_partition, partition_number, boost::size( rng ) ), 
        detail::partition_upper_bound( n_partition, partition_number, boost::size( rng ) )
                                    )
    )
    {
        return boost::adaptors::slice( 
            rng,  
            detail::partition_lower_bound( n_partition, partition_number, boost::size( rng ) ), 
            detail::partition_upper_bound( n_partition, partition_number, boost::size( rng ) )
        );
    }


    struct partitioned
    {
        partitioned(size_t n_partition, size_t partition_number, size_t rng_size)
            :n_partition(n_partition), partition_number(partition_number), rng_size(rng_size) { }
        size_t n_partition;
        size_t partition_number;
        size_t rng_size;
    };
        
    template< typename random_access_range >
    inline auto
    operator|( random_access_range& r, const partitioned& f ) 
        -> decltype(
            partition( r, f.n_partition, f.partition_number, f.rng_size )
        )
    {
        return partition(r, f.n_partition, f.partition_number, f.rng_size );
    }

} //namespace adaptors

template < typename input_iterator, typename distance >
input_iterator advance_with_copy ( input_iterator it, distance n )
{
    std::advance( it, n );
    return it;
}





template<
    typename single_pass_range1,
    typename output_iterator,
    typename unary_operation,
    typename thread_type = std::thread
>
output_iterator transform( single_pass_range1 const & rng,
                           output_iterator out,
                           unary_operation fun,
                           size_t number_of_threads = std::thread::hardware_concurrency()
                        )
{    
    using namespace gpcxx::par::adaptors;
    using namespace gpcxx::par::adaptors::detail;
    
    if ( boost::empty( rng ) )
        return out;

    if ( number_of_threads == 1 )
        return boost::transform( rng, out, fun ); 
   
    size_t rng_size = boost::size( rng );
    
    if( rng_size < number_of_threads )
        number_of_threads = rng_size;
        
    std::vector< thread_type >  worker{}; worker.reserve(number_of_threads);
    
    for (size_t i = 0; i < number_of_threads; ++i)
    {
        auto sub_rng = rng | partitioned( number_of_threads, i , rng_size );

        auto out_iter_for_sub_range = advance_with_copy( 
            out, 
            partition_lower_bound( number_of_threads, i , rng_size ) );
        
        worker.emplace_back( [&](){ boost::transform( sub_rng, out_iter_for_sub_range, fun ); } );
    }
    for ( auto& w: worker ) w.join();
    
    return out;
}






template<
    typename single_pass_range1,
    typename single_pass_range2,
    typename output_iterator,
    typename binary_operation,
    typename thread_type = std::thread
>
output_iterator transform( single_pass_range1 const & rng1,
                           single_pass_range2 const & rng2,
                           output_iterator out,
                           binary_operation fun,
                           size_t number_of_threads = std::thread::hardware_concurrency()
                        )
{
    assert( boost::size(rng1) == boost::size(rng2) );
    
    using namespace gpcxx::par::adaptors;
    using namespace gpcxx::par::adaptors::detail;
    
    if ( boost::empty( rng1 ) )
        return out;

    if ( number_of_threads == 1 )
        return boost::transform( rng1, rng2, out, fun ); 
   
    size_t rng1_size = boost::size( rng1 );
    
    if( rng1_size < number_of_threads )
        number_of_threads = rng1_size;
        
    std::vector< thread_type >  worker{}; worker.reserve( number_of_threads );
    
    for (size_t i = 0; i < number_of_threads; ++i)
    {    
        auto sub_rng1 = rng1 | partitioned( number_of_threads, i , rng1_size );
        auto sub_rng2 = rng2 | partitioned( number_of_threads, i , rng1_size );
        
        auto out_iter_for_sub_range = advance_with_copy( 
            out, 
            partition_lower_bound( number_of_threads, i , rng1_size ) );
        
        worker.emplace_back( [&](){boost::transform( sub_rng1, sub_rng2, out_iter_for_sub_range, fun ); } );
    }
    for ( auto& w: worker ) w.join();
    
    return out;
}


template<
    typename single_pass_range,
    typename unary_function,
    typename thread_type = std::thread
>
unary_function for_each( single_pass_range & rng, 
                         unary_function fun, 
                         size_t number_of_threads = std::thread::hardware_concurrency()
                       )
{
    using namespace gpcxx::par::adaptors;

    if ( boost::empty( rng ) )
        return fun;
    
    if ( number_of_threads == 1 )
        return boost::for_each( rng, fun );
       
    size_t rng_size = boost::size( rng );
    
    if( rng_size < number_of_threads )
        number_of_threads = rng_size;
    
    std::vector< thread_type > worker{}; worker.reserve( number_of_threads );
    for ( size_t i = 0; i < number_of_threads; ++i )
    {            
        auto sub_rng =  rng | partitioned( number_of_threads, i , rng_size );

        worker.emplace_back( [&](){ boost::for_each( sub_rng, fun ); } );
    }
    for ( auto& w: worker ) w.join();
    
    return fun;
}


} // namespace par
} // namespace gpcxx


#endif // GPCXX_PARALLEL_ALGORITHM_HPP_DEFINED
class name;