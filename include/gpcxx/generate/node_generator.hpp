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
#include <random>
#include <array>
#include <algorithm>
#include <cassert>

namespace gpcxx {

// implement new generate strategy
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
    using weighted_generator_type = struct {
        value_type weight;
        size_t arity;
        generator_type generator; };
    using generator_container = std::array< weighted_generator_type , Dim >;
    
    // construct
    /// Default constructor
    node_generator( void )
    { }
    
    /// Constructs a node generator.
    node_generator( weighted_generator_type gen1 )
    : m_generators { std::move( gen1 ) }
    {
        static_assert( dim == 1 , "Dimension must be one." );
        prepare();
    }
    
    /// Constructs a node generator
    node_generator( weighted_generator_type gen1 , weighted_generator_type gen2 )
    : m_generators { { std::move( gen1 ) , std::move( gen2 ) } }
    {
        static_assert( dim == 2 , "Dimension must be one." );
        prepare();
    }
    
    /// Constructs a node generator
    node_generator( weighted_generator_type gen1 , weighted_generator_type gen2 , weighted_generator_type gen3 )
    : m_generators { { std::move( gen1 ) , std::move( gen2 ) , std::move( gen3 ) } }
    {
        static_assert( dim == 3 , "Dimension must be one." );
        prepare();
    }
    
    /// Constructs a note generator, the weight is one
    node_generator( generator_type terminal )
    : m_generators { { 1.0 , 0 , std::move( terminal ) } }
    {
        static_assert( dim == 1 , "Dimension must be one." );
        prepare();
    }
    
    /// Constructs a node generator
    node_generator( generator_type terminal , generator_type unary )
    : m_generators { {
        { 1.0 , 0 , std::move( terminal ) } ,
        { 1.0 , 1 , std::move( unary ) }
        } }
    {
        static_assert( dim == 2 , "Dimension must be one." );
        prepare();
    }
    
    /// Constructs a node generator
    node_generator( generator_type terminal , generator_type unary , generator_type binary )
    : m_generators { {
        { 1.0 , 0 , std::move( terminal ) } ,
        { 1.0 , 1 , std::move( unary ) } ,
        { 1.0 , 2 , std::move( binary ) }
        } }
    {
        static_assert( dim == 3 , "Dimension must be one." );
        prepare();
    }

    
    
    // accessors
    value_type weight( size_t i ) const
    {
        return m_generators[i].weight;
    }
    
    void set_weight( size_t i , value_type w )
    {
        m_generators[i].weight = w;
        prepare();
    }
    
    generator_type const& generator( size_t i ) const
    {
        return m_generators[i].generator;
    }
    
    void set_generator( size_t i , generator_type gen )
    {
        m_generators[i].generator = std::move( gen );
        prepare();
    }
    
    size_t arity( size_t i ) const
    {
        return m_generators[i].arity;
    }
    
    void set_arity( size_t i , size_t a )
    {
        m_generators[i].arity = a;
        prepare();
    }
    
    
    
    // 
    node_type get_non_terminal_node( rng_type& rng )
    {
        return m_non_terminal_generators[ m_non_terminal_dist( rng ) ].generator( rng );
    }
    
    node_type get_node( rng_type& rng )
    {
        return m_generators[ m_dist( rng ) ].generator( rng );
    }


    
private:
    
    void prepare( void )
    {
        prepare_non_terminal_dist();
        prepare_dist();
    }
    
    void prepare_non_terminal_dist( void )
    {
        std::array< double , dim > weight;
        auto t_iter = std::copy_if( m_generators.begin() , m_generators.end() , m_non_terminal_generators.begin() ,
                      []( weighted_generator_type const& w ) -> bool { return w.arity != 0; } );
        std::ptrdiff_t len = std::distance( m_non_terminal_generators.begin() , t_iter );
        assert( ( len >= 0 ) && ( len <= dim ) );
        std::transform( m_non_terminal_generators.begin() , t_iter , weight.begin() ,
                        []( weighted_generator_type const& w ) { return w.weight; } );
        m_non_terminal_dist = std::discrete_distribution<>( weight.begin() , weight.begin() + len );
    }
    
    void prepare_dist( void )
    {
        std::array< double , dim > weight;
        std::transform( m_generators.begin() , m_generators.end() , weight.begin() ,
                        []( weighted_generator_type const& w ) { return w.weight; } );
        m_dist = std::discrete_distribution<>( weight.begin() , weight.end() );
    }
    
    generator_container m_generators;
    generator_container m_non_terminal_generators;
    std::discrete_distribution<> m_dist;
    std::discrete_distribution<> m_non_terminal_dist;
};




} // namespace gpcxx


#endif // GPCXX_GENERATE_NODE_GENERATOR_HPP_INCLUDED
