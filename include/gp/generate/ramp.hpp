/*
  gp/generate/ramp.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GP_GENERATE_RAMP_HPP_DEFINED
#define GP_GENERATE_RAMP_HPP_DEFINED

#include <gp/generate/basic_generate_strategy.hpp>

#include <random>

namespace gp {


template< typename Rng , typename TerminalGen , typename UnaryGen , typename BinaryGen >
class ramp
{
public:
    
    ramp( Rng &rng , TerminalGen &gen0 , UnaryGen &gen1 , BinaryGen &gen2 ,
          size_t min_height , size_t max_height , double grow_prob , std::array< int , 3 > const& gen_weights )
    : m_rng( rng ) , m_terminal_gen( gen0 ) , m_unary_gen( gen1 ) , m_binary_gen( gen2 )
    , m_min_height( min_height ) , m_max_height( max_height ) , m_grow_prob( grow_prob ) , m_gen_weights( gen_weights )
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
            auto generate = make_basic_generate_strategy( m_rng , m_terminal_gen , m_unary_gen , m_binary_gen , 1 , height , m_gen_weights );
            generate( tree );
        }
        else // full method
        {
            auto generate = make_basic_generate_strategy( m_rng , m_terminal_gen , m_unary_gen , m_binary_gen , height - 1 , height , m_gen_weights );
            generate( tree );
        }
        
    }
    
private:
 
    Rng &m_rng;
    TerminalGen &m_terminal_gen;
    UnaryGen &m_unary_gen;
    BinaryGen &m_binary_gen;
    size_t m_min_height;
    size_t m_max_height;
    double m_grow_prob; // full prob is 1 - m_grow__prob
    std::array< int , 3 > m_gen_weights;
};


template< typename Rng , typename TerminalGen , typename UnaryGen , typename BinaryGen >
ramp< Rng , TerminalGen , UnaryGen , BinaryGen >
make_ramp( Rng &rng , TerminalGen &gen0 , UnaryGen &gen1 , BinaryGen &gen2 ,
           size_t min_height , size_t max_height , double grow_prob , std::array< int , 3 > const& gen_weights )
{
    return ramp< Rng , TerminalGen , UnaryGen , BinaryGen >( rng , gen0 , gen1 , gen2 , min_height , max_height , grow_prob , gen_weights );
}

} // namespace gp


#endif // GP_GENERATE_RAMP_HPP_DEFINED
