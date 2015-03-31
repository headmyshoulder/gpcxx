/*
 * gpcxx/eval/normalized_fitness.hpp
 * Date: 2015-03-26
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_EVAL_NORMALIZED_FITNESS_HPP_INCLUDED
#define GPCXX_EVAL_NORMALIZED_FITNESS_HPP_INCLUDED

#include <boost/range/adaptor/transformed.hpp>

#include <algorithm>
#include <numeric>
#include <iterator>
#include <vector>

struct A;

namespace gpcxx {
    
namespace detail {
    
    
    struct normalized_fitness_transform_fn
    {
        template< typename AdjustedFitness , typename NormalizedFitness >
        void operator()( AdjustedFitness const& af , NormalizedFitness& nf ) const
        {
            nf.resize( af.size() );
            auto sum = std::accumulate( std::begin( af ) , std::end( af ) , static_cast< typename AdjustedFitness::value_type >( 0.0 ) );
            std::transform( std::begin( af ) , std::end( af ) , std::begin( nf ) , [sum]( auto& x ) { return x / sum; } );
        }
    };
    
    struct normalized_fitness_copy_fn
    {
        template< typename AdjustedFitness >
        std::vector< typename AdjustedFitness::value_type > operator()( AdjustedFitness const& af ) const
        {
            std::vector< typename AdjustedFitness::value_type > nf;
            normalized_fitness_transform_fn {} ( af , nf );
            return nf;
        }
    };
    
    struct normalized_fitness_inplace_fn
    {
        template< typename AdjustedFitness >
        void operator()( AdjustedFitness& af ) const
        {
            auto sum = std::accumulate( std::begin( af ) , std::end( af ) , static_cast< typename AdjustedFitness::value_type >( 0.0 ) );
            std::for_each( std::begin( af ) ,std::end( af ) , [sum]( auto& x ) { x /= sum; } );
        }
    };
    
    struct normalized_fitness_view_fn
    {
        template< typename AdjustedFitness >
        auto operator()( AdjustedFitness const& af ) const
        {
            auto sum = std::accumulate( std::begin( af ) , std::end( af ) , static_cast< typename AdjustedFitness::value_type >( 0.0 ) );
            return boost::adaptors::transform( af , [sum]( auto x ) { return x / sum; } );
        }
    };
    
} // namespace detail


static constexpr detail::normalized_fitness_transform_fn normalized_fitness = detail::normalized_fitness_transform_fn {};
static constexpr detail::normalized_fitness_copy_fn normalized_fitness_copy = detail::normalized_fitness_copy_fn {};
static constexpr detail::normalized_fitness_inplace_fn normalized_fitness_inplace = detail::normalized_fitness_inplace_fn {};
static constexpr detail::normalized_fitness_view_fn normalized_fitness_view = detail::normalized_fitness_view_fn {};

} // namespace gpcxx


#endif // GPCXX_EVAL_NORMALIZED_FITNESS_HPP_INCLUDED
