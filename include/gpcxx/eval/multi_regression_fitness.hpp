/*
 * gpcxx/eval/multi_regression_fitness.hpp
 * Date: 2016-01-26
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_EVAL_MULTI_REGRESSION_FITNESS_HPP_INCLUDED
#define GPCXX_EVAL_MULTI_REGRESSION_FITNESS_HPP_INCLUDED

#include <cassert>
#include <utility>
#include <cmath>
#include <cstddef>


namespace gpcxx {


template< typename Eval , typename Distance >
struct multi_regression_fitness
{
    using eval_type = Eval ;
    using distance_type = Distance;
    
    eval_type m_eval;
    distance_type m_dist;
    
    multi_regression_fitness( eval_type eval , distance_type dist )
    : m_eval( std::move( eval ) ) , m_dist( std::move( dist ) ) { }
    
    template< typename Individual , typename Independent , typename Dependent >
    auto get_chi2( Individual const &individual , Independent const& x , Dependent const& y ) const
    {
        using value_type = decltype( m_dist( y[0] , y[0] ) );
        
        assert( x.size() == y.size() );
        value_type chi2 = 0.0;
        for( size_t i=0 ; i<x.size() ; ++i )
        {
            auto yy = m_eval( individual , x[i] );
            auto d = m_dist( y[i] , yy );
            chi2 += d;
        }
        return chi2 / value_type( x[0].size() );
    }

    template< typename Individual , typename Independent , typename Dependent >
    auto operator()( Individual const &individual , Independent const& x , Dependent const& y ) const
    {
        auto chi2 = get_chi2( individual , x , y );
        return ( std::isnan( chi2 ) ? 1.0 : 1.0 - 1.0 / ( 1.0 + chi2 ) );
    }
};

template< typename Eval , typename Distance >
multi_regression_fitness< Eval , Distance > make_multi_regression_fitness( Eval eval , Distance distance )
{
    return multi_regression_fitness< Eval , Distance >( std::move( eval ) , std::move( distance ) );
}



} // namespace gpcxx


#endif // GPCXX_EVAL_MULTI_REGRESSION_FITNESS_HPP_INCLUDED
