/*
 * gpcxx/generate/node_generator.hpp
 * Date: 2014-07-08
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_GENERATE_NODE_GENERATOR_HPP_INCLUDED
#define GPCXX_GENERATE_NODE_GENERATOR_HPP_INCLUDED

#include <utility>


namespace gpcxx {


template< typename Value , typename Node , typename Rng , size_t Dim >
class node_generator
{
public:

    // typedefs
    static const size_t dim = Dim;
    using value_type = Value;
    using rng_type = Rng;
    using node_type = Node;
    using generator_type = std::function< node_type( rng_type& ) >;
    using weighted_generator_type = std::pair< value_type , generator_type >;
    using generator_container = std::array< weighted_generator_type >;
    
    // construct
    
    /// Constructs a node generator, the weight is one
    node_generator( generator_type terminal )
    : m_generators()
    {
        static_assert( dim == 1 , "Dimension must be one." );
        m_generators[0].first = 1.0;
        m_generators[0].second = std::move( terminal );
    }
    
    /// Constructs a node generator, the weights are one
    node_generator( generator_type terminal , generator_type unary )
    : m_generators()
    {
        static_assert( dim == 2 , "Dimension must be one." );
        m_generators[0].first = 1.0;
        m_generators[0].second = std::move( terminal );
        m_generators[1].first = 1.0;
        m_generators[1].second = std::move( unary );
    }
    
    /// Constructs a node generator, the weights are one
    node_generator( generator_type terminal , generator_type unary , generator_type binary )
    : m_generators()
    {
        static_assert( dim == 3 , "Dimension must be one." );
        m_generators[0].first = 1.0;
        m_generators[0].second = std::move( terminal );
        m_generators[1].first = 1.0;
        m_generators[1].second = std::move( unary );
        m_generators[2].first = 1.0;
        m_generators[2].second = std::move( binary );
    }

    
    
    // accessors
    value_type weight( size_t i ) const
    {
        return m_generators[i].first;
    }
    
    value_type& weight( size_t i )
    {
        return m_generators[i].first;
    }
    
    generator_type const& generator( size_t i ) const
    {
        return m_generators[i].second;
    }
    
    generator_type& generator( size_t i )
    {
        return m_generators[i].second;
    }

    
private:
    
    generator_container m_generators;

};

template< typename Value , typename Node , typename Rng >
node_generator< Value , Node , Rng , 1 > make_node_generator( 



} // namespace gpcxx


#endif // GPCXX_GENERATE_NODE_GENERATOR_HPP_INCLUDED
