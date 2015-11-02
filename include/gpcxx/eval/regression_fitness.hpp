/*
  gpcxx/eval/regression_fitness.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GPCXX_EVAL_REGRESSION_FITNESS_HPP_DEFINED
#define GPCXX_EVAL_REGRESSION_FITNESS_HPP_DEFINED

#include <vector>
#include <cmath>
#include <cstddef>
#include <array>

namespace gpcxx {


template< typename Value , size_t Dim , typename SequenceType = std::vector< Value > >
struct regression_training_data
{
    static const size_t dim = Dim;
    SequenceType y;
    std::array< SequenceType , dim > x;
};

template< typename Value , size_t Dim >
using regression_context = std::array< Value , Dim >;


namespace detail {
    
    struct abs
    {
        template< typename T >
        T operator()( T t ) const { return std::abs( t ); }
    };
    
    struct square
    {
        template< typename T >
        T operator()( T t ) const { return t * t; }
    };
    
} // namespace detail


template< typename Eval , typename Norm = detail::abs >
struct regression_fitness
{
    typedef Eval eval_type;
    typedef typename eval_type::context_type context_type;
    typedef typename eval_type::value_type value_type;
    
    eval_type m_eval;
    
    regression_fitness( eval_type eval ) : m_eval( eval ) { }
    
    template< typename Tree , typename TrainingData >
    value_type get_chi2( Tree const &t , TrainingData const& c ) const
    {
        // static_assert( TrainingData::n == context_type::n , "dimension of trainingsdata must be equal to dimension of evaluation context" );
        value_type chi2 = 0.0;
        for( size_t i=0 ; i<c.x[0].size() ; ++i )
        {
            context_type cc;
            for( size_t j=0 ; j<TrainingData::dim ; ++j ) cc[j] = c.x[j][i];
            value_type yy = m_eval( t , cc );
            chi2 += Norm()( yy - c.y[i] );
        }
        return chi2 / value_type( c.x[0].size() );
    }

    template< typename Tree , typename TrainingData >
    value_type operator()( Tree const & t , TrainingData const& c ) const
    {
        value_type chi2 = get_chi2( t , c );
        return ( std::isnan( chi2 ) ? 1.0 : 1.0 - 1.0 / ( 1.0 + chi2 ) );
    }
};

template< typename Eval >
regression_fitness< Eval > make_regression_fitness( Eval eval )
{
    return regression_fitness< Eval >( eval );
}


} // namespace gpcxx


#endif // GPCXX_EVAL_REGRESSION_FITNESS_HPP_DEFINED
