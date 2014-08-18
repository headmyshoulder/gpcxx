/*
 * gpcxx/examples/santa_fe_ant_trail/ant_simulation_nodes.hpp
 * Date: 2014-08-18
 * Author: Gerard Choinka (gerard.choinka+gpcxx@gmail.com)
 * Copyright: Gerard Choinka
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_EXAMPLES_SANTA_FE_ANT_TRAIL_ANT_SIMULATION_NODES_HPP_INCLUDED
#define GPCXX_EXAMPLES_SANTA_FE_ANT_TRAIL_ANT_SIMULATION_NODES_HPP_INCLUDED

#include <vector>

#include <gpcxx/tree/basic_named_intrusive_node.hpp>
#include <gpcxx/tree/intrusive_tree.hpp>

#include "ant_simulation.hpp"

namespace ant_example {
    
    
using eval_context_type = ant_simulation;
using result_type = void ;
using node_type = gpcxx::basic_named_intrusive_node< result_type , eval_context_type, 2 > ;
using tree_type = gpcxx::intrusive_tree< node_type >;
using population_type = std::vector< tree_type >;
using fitness_type = std::vector< int >;
    
struct evaluator
{
    typedef eval_context_type context_type;

    int operator()( tree_type const& t , context_type c ) const
    {
        while( !c.is_finsh() ) 
        {
            t.root()->eval( c );
        }
        return c.score();
    }
};

struct prog2                                                                                           
{                                                                                                     
    template< typename Context , typename Node >                                                      
    inline typename Node::result_type operator()( Context& ant_sim , Node const& node ) const         
    {
        node.children( 0 )->eval( ant_sim );
        node.children( 1 )->eval( ant_sim );             
    }                                                                                                 
};

struct prog3                                                                                           
{                                                                                                     
    template< typename Context , typename Node >                                                      
    inline typename Node::result_type operator()( Context& ant_sim , Node const& node ) const         
    {
        node.children( 0 )->eval( ant_sim );
        node.children( 1 )->eval( ant_sim );
        node.children( 2 )->eval( ant_sim );            
    }                                                                                                 
};

struct if_food_ahead                                                                                           
{                                                                                                     
    template< typename Context , typename Node >                                                      
    inline typename Node::result_type operator()( Context& ant_sim , Node const& node ) const         
    {
        if(ant_sim.food_in_front())
        {
            node.children( 0 )->eval( ant_sim );
        }
        else
        {
            node.children( 1 )->eval( ant_sim );
        }                     
    }                                                                                                 
};

struct do_nothing                                                                                           
{                                                                                                     
    template< typename Context , typename Node >                                                      
    inline typename Node::result_type operator()( Context& ant_sim , Node const& node ) const         
    {
        node.children( 0 )->eval( ant_sim );                    
    }                                                                                                 
};

struct ant_move_task_terminal
{
    template< typename Context , typename Node >
    typename Node::result_type operator()( Context & ant_sim , Node const& node ) const
    {
        ant_sim.move();
    }
};

struct ant_turn_left_task_terminal
{
    template< typename Context , typename Node >
    typename Node::result_type operator()( Context & ant_sim , Node const& node ) const
    {
        ant_sim.turn_left();
    }
};

struct ant_turn_right_task_terminal
{
    template< typename Context , typename Node >
    typename Node::result_type operator()( Context & ant_sim , Node const& node ) const
    {
        ant_sim.turn_right();
    }
}; 

    
} // namespace ant_example


#endif // GPCXX_EXAMPLES_SANTA_FE_ANT_TRAIL_ANT_SIMULATION_NODES_HPP_INCLUDED