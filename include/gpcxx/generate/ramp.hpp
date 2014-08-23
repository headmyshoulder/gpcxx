/*
  gpcxx/generate/ramp.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GPCXX_GENERATE_RAMP_HPP_DEFINED
#define GPCXX_GENERATE_RAMP_HPP_DEFINED

#include <gpcxx/generate/basic_generate_strategy.hpp>

#include <random>

namespace gpcxx {

    
template< typename Rng , typename NodeGenerator >
class ramp
{
public:
    
    ramp( Rng &rng , NodeGenerator &gen ,
          size_t min_height , size_t max_height , double grow_prob )
    : m_rng( rng ) , m_gen( gen )
    , m_min_height( min_height ) , m_max_height( max_height ) , m_grow_prob( grow_prob )
    { }
    
    template< typename Tree >
    void operator()( Tree& tree ) const
    {
        std::uniform_int_distribution< size_t > height_dist( m_min_height , m_max_height );
        std::discrete_distribution< int > method_dist { m_grow_prob , 1.0 - m_grow_prob };
        
        size_t height = height_dist( m_rng );
        int method = method_dist( m_rng );
        
        if( method == 0 ) // grow method
        {
            auto generate = make_node_generator_generate_strategy( m_rng , m_gen , 1 , height );
            generate( tree );
        }
        else // full method
        {
            auto generate = make_node_generator_generate_strategy( m_rng , m_gen , height - 1 , height );
            generate( tree );
        }
        
    }
    
private:
 
    Rng &m_rng;
    NodeGenerator &m_gen;
    size_t m_min_height;
    size_t m_max_height;
    double m_grow_prob; // full prob is 1 - m_grow__prob
};


template< typename Rng , typename NodeGenerator >
ramp< Rng , NodeGenerator >
make_ramp( Rng &rng , NodeGenerator &gen ,
           size_t min_height , size_t max_height , double grow_prob )
{
    return ramp< Rng , NodeGenerator >( rng , gen , min_height , max_height , grow_prob );
}
} // namespace gpcxx


#endif // GPCXX_GENERATE_RAMP_HPP_DEFINED
