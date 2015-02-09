/*
 * gpcxx/examples/artificial_ant/ant_simulation_nodes.hpp
 * Date: 2014-08-18
 * Author: Gerard Choinka (gerard.choinka+gpcxx@gmail.com)
 * Copyright: Gerard Choinka
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_EXAMPLES_ARTIFICIAL_ANT_ANT_SIMULATION_NODES_HPP_INCLUDED
#define GPCXX_EXAMPLES_ARTIFICIAL_ANT_ANT_SIMULATION_NODES_HPP_INCLUDED

#include "simulation.hpp"

#include <gpcxx/tree.hpp>

#include <vector>




namespace ant_example {
    
//[node_types_delerations  
size_t const max_children = 3;
using context_type = ant_simulation;
using node_return_type = void;
using node_type = gpcxx::intrusive_named_func_node< node_return_type , context_type, max_children > ;
using tree_type = gpcxx::intrusive_tree< node_type >;
using population_type = std::vector< tree_type >;
using fitness_type = std::vector< int >;
//]

//[evaluator_delerations  
struct evaluator
{
    int operator()( tree_type const& t , context_type c ) const
    {
        while( !c.is_finsh() ) 
        {
            t.root()->eval( c );
        }
        return c.score();
    }
};
//]

//[branch_nodes
struct prog2                                                                                           
{                                                                                                     
    void operator()( context_type& ant_sim , node_type const& node ) const         
    {
        node.child( 0 ).eval( ant_sim );
        node.child( 1 ).eval( ant_sim );             
    }                                                                                                 
};

struct prog3                                                                                           
{                                                                                                     
    void operator()( context_type& ant_sim , node_type const& node ) const         
    {
        node.child( 0 ).eval( ant_sim );
        node.child( 1 ).eval( ant_sim );
        node.child( 2 ).eval( ant_sim );            
    }                                                                                                 
};

struct if_food_ahead                                                                                           
{                                                                                                     
    void operator()( context_type& ant_sim , node_type const& node ) const         
    {
        if(ant_sim.food_in_front())
        {
            node.child( 0 ).eval( ant_sim );
        }
        else
        {
            node.child( 1 ).eval( ant_sim );
        }                     
    }                                                                                                 
};
//]


//[action_nodes 
struct ant_move_task_terminal
{
    void operator()( context_type & ant_sim , node_type const& node ) const
    {
        ant_sim.move();
    }
};

struct ant_turn_left_task_terminal
{
    void operator()( context_type & ant_sim , node_type const& node ) const
    {
        ant_sim.turn_left();
    }
};

struct ant_turn_right_task_terminal
{
    void operator()( context_type & ant_sim , node_type const& node ) const
    {
        ant_sim.turn_right();
    }
}; 
//]
    
} // namespace ant_example


#endif // GPCXX_EXAMPLES_ARTIFICIAL_ANT_ANT_SIMULATION_NODES_HPP_INCLUDED