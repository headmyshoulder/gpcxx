/*
 * gpcxx/eval/adjusted_fitness.hpp
 * Date: 2015-03-25
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_EVAL_ADJUSTED_FITNESS_HPP_INCLUDED
#define GPCXX_EVAL_ADJUSTED_FITNESS_HPP_INCLUDED

#include <algorithm>
#include <vector>
#include <iterator>


namespace gpcxx {


namespace detail
{

    struct adjusted_fitness_single_fn
    {
        template< typename T >
        T operator()( T t ) const
        {
            const T one = static_cast< T >( 1.0 );
            return one / ( one + t );
        }
    };
    
    
    struct adjusted_fitness_transform_fn
    {
        template< typename StandardizedFitness , typename AdjustedFitness >
        void operator()( StandardizedFitness const& sf , AdjustedFitness& af ) const
        {
            af.resize( sf.size() );
            std::transform( sf.begin() , sf.end() , af.begin() , adjusted_fitness_single_fn {} );
        }
        
    };
    
    struct adjusted_fitness_copy_fn
    {
        template< typename StandardizedFitness >
        std::vector< typename StandardizedFitness::value_type > operator()( StandardizedFitness const& sf ) const
        {
            std::vector< typename StandardizedFitness::value_type > af;
            adjusted_fitness_transform_fn {} ( sf , af );
            return af;
        }
    };
    
    struct adjusted_fitness_fn
    {
        template< typename StandardizedFitness >
        void operator()( StandardizedFitness& f ) const
        {
            std::for_each( std::begin( f ) , std::end( f ) , []( auto& x ) { x = adjusted_fitness_single_fn {}( x ); } );
        }
    };
    

    
} // namespace detail

static constexpr detail::adjusted_fitness_single_fn adjusted_fitness_single = detail::adjusted_fitness_single_fn {};
static constexpr detail::adjusted_fitness_transform_fn adjusted_fitness = detail::adjusted_fitness_transform_fn {};
static constexpr detail::adjusted_fitness_copy_fn adjusted_fitness_copy = detail::adjusted_fitness_copy_fn {};
static constexpr detail::adjusted_fitness_fn adjusted_fitness_inplace = detail::adjusted_fitness_fn {};




/*
VERSIONS

view = adjusted_fitness_view( sf );  // view
adjust_fitness( sf );                // transform inplace
af = adjusted_fitness_copy( sf );         // return transformed fitness
adjust_fitness_transform( sf , af );           // transform out-of-place

*/




} // namespace gpcxx


#endif // GPCXX_EVAL_ADJUSTED_FITNESS_HPP_INCLUDED
