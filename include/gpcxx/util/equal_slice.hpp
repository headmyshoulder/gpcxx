/*
  gpcxx/util/equal_slice.hpp

  Copyright 2013 Gerard Choinka

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef GPCXX_RATION_HPP_DEFINED
#define GPCXX_RATION_HPP_DEFINED

#include <boost/range/algorithm.hpp>
#include <boost/range/adaptor/sliced.hpp>

#include <tuple>
#include <cassert>


namespace gpcxx {

namespace detail {

    inline std::tuple< size_t, size_t >
    equal_slice_bounds( size_t equal_slice_number,
                size_t n_equal_slices,
                size_t rng_size )
    {
        assert( n_equal_slices > equal_slice_number );
                
        size_t equal_slice_size = rng_size / n_equal_slices ;
                    
        if ( n_equal_slices  == ( equal_slice_number + 1 ))
            return std::tuple< size_t, size_t >{ equal_slice_size * equal_slice_number, rng_size };
        
        return std::tuple< size_t, size_t >{ equal_slice_size * equal_slice_number, equal_slice_size * (equal_slice_number + 1)  };
    }


    inline size_t equal_slice_upper_bound( size_t equal_slice_number, size_t n_equal_slices, size_t rng_size )
    {
        return std::get<1>( equal_slice_bounds( equal_slice_number, n_equal_slices,  rng_size) );
    }


    inline size_t equal_slice_lower_bound( size_t equal_slice_number, size_t n_equal_slices, size_t rng_size )
    {
        return std::get<0>( equal_slice_bounds( equal_slice_number, n_equal_slices, rng_size) );
    }
  
}//namespace detail


template< typename RandomAccessRange >
auto equal_slice( RandomAccessRange & rng ,
                size_t equal_slice_number ,
                size_t n_equal_slices 
               )
-> decltype( boost::adaptors::slice( 
    rng,  
    detail::equal_slice_lower_bound( equal_slice_number, n_equal_slices, boost::size( rng ) ), 
    detail::equal_slice_upper_bound( equal_slice_number, n_equal_slices, boost::size( rng ) )
                ) )
{
    return boost::adaptors::slice( 
        rng,  
        detail::equal_slice_lower_bound( equal_slice_number, n_equal_slices, boost::size( rng ) ), 
        detail::equal_slice_upper_bound( equal_slice_number, n_equal_slices, boost::size( rng ) )
    );
}


struct equal_sliceed
{
    equal_sliceed( size_t equal_slice_number , size_t n_equal_slices  )
        : equal_slice_number( equal_slice_number ) , n_equal_slices( n_equal_slices ) { }
    size_t n_equal_slices;
    size_t equal_slice_number;
};
    
template< typename RandomAccessRange >
inline auto
operator | ( RandomAccessRange& rng,  equal_sliceed const & f ) 
    -> decltype( equal_slice( rng, f.equal_slice_number, f.n_equal_slices ) )
{
    return equal_slice( rng, f.equal_slice_number , f.n_equal_slices );
}

} // namespace gpcxx

#endif // GPCXX_RATION_HPP_DEFINED