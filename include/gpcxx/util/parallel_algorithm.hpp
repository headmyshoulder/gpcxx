/*
  gpcxx/util/parallel_algorithm.hpp

  Copyright 2013 Gerard Choinka

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef GPCXX_PARALLEL_ALGORITHM_HPP_DEFINED
#define GPCXX_PARALLEL_ALGORITHM_HPP_DEFINED

#include <boost/range/algorithm.hpp>
#include <algorithm>
#include <vector>
#include <cassert>
#include <thread>

#include <gpcxx/util/equal_slice.hpp>
    
namespace gpcxx {


template<
    typename RandomAccessRange,
    typename OutputIterator,
    typename UnaryOperation
>
OutputIterator transform(  RandomAccessRange const & rng,
                           OutputIterator out,
                           UnaryOperation fun,
                           size_t number_of_threads = std::thread::hardware_concurrency()
                        )
{    
    using namespace gpcxx::detail;
    
    if ( number_of_threads == 1 || boost::empty( rng ) )
        return boost::transform( rng, out, fun ); 
   
    number_of_threads = std::min( boost::size( rng ), number_of_threads );
        
    std::vector< std::thread >  worker{};  
    for (size_t i = 0; i < number_of_threads ; ++i)
    {
        auto sub_rng = rng | equal_sliceed( i, number_of_threads );
        auto sub_out = out + equal_slice_lower_bound( i, number_of_threads,  boost::size( rng ) ) ;            
        worker.emplace_back( [=](){ boost::transform( sub_rng, sub_out, fun ); } );
    }
    for ( auto& w: worker ) w.join();
    
    return out;
}


template<
    typename RandomAccessRange1,
    typename RandomAccessRange2,
    typename OutputIterator,
    typename BinaryOperation
>
OutputIterator transform2( RandomAccessRange1 const & rng1,
                           RandomAccessRange2 const & rng2,
                           OutputIterator out,
                           BinaryOperation fun,
                           size_t number_of_threads = std::thread::hardware_concurrency() )
{
    using namespace gpcxx::detail;
    
    assert( boost::size(rng1) == boost::size(rng2) );

    if ( number_of_threads == 1 || boost::empty( rng1 ) )
        return boost::transform( rng1, rng2, out, fun ); 
       
    number_of_threads = std::min( boost::size( rng1 ), number_of_threads );
        
    std::vector< std::thread >  worker{};   
    for (size_t i = 0; i < number_of_threads ; ++i)
    {    
        auto sub_rng1 = rng1 | equal_sliceed( i, number_of_threads );
        auto sub_rng2 = rng2 | equal_sliceed( i, number_of_threads );
        
        auto sub_out = out + equal_slice_lower_bound( i, number_of_threads,  boost::size( rng1 ) ) ;
        
        worker.emplace_back( [=](){boost::transform( sub_rng1, sub_rng2, sub_out, fun ); } );
    }
    for ( auto& w: worker ) w.join();
    
    return out;
}


template<
    typename RandomAccessRange,
    typename UnaryFunction
>
UnaryFunction for_each(  RandomAccessRange & rng, 
                         UnaryFunction fun, 
                         size_t number_of_threads = std::thread::hardware_concurrency() )
{   
    if ( number_of_threads == 1  || boost::empty( rng ) )
        return boost::for_each( rng, fun );
       
    number_of_threads = std::min( boost::size( rng ) , number_of_threads );

    std::vector< std::thread > worker{};
    for ( size_t i = 0; i < number_of_threads ; ++i )
    {            
        auto sub_rng =  rng | equal_sliceed( i, number_of_threads );
        worker.emplace_back( [=](){ boost::for_each( sub_rng, fun ); } );
    }
    for ( auto& w: worker ) w.join();
    
    return fun;
}


} // namespace gpcxx


#endif // GPCXX_PARALLEL_ALGORITHM_HPP_DEFINED
