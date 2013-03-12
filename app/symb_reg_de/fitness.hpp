/*
 * fitness.hpp
 * Date: 2013-02-22
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef FITNESS_HPP_INCLUDED
#define FITNESS_HPP_INCLUDED

/*
 * OK # include de package into genetic programming branch
 * # traverse tree and find all double terminals and pack them in a vector of node*
 * # de optimization
 */

#include "eval.hpp"

#include <gp/io/simple.hpp>
#include <gp/util/log.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "my_listener.hpp"
#include <differential_evolution.hpp>


#include <iostream>
#define tab "\t"
using namespace std;


struct fitness_function
{
    typedef std::vector< node_type* > node_vector;
    typedef std::vector< double > vector_type;
    struct context_type
    {
        vector_type x1 , x2 , x3 , y;
    };

    static double fitness( tree_type &t , const context_type &c )
    {
        double chi2 = 0.0;
        for( size_t i=0 ; i<c.x1.size() ; ++i )
        {
            tree_eval::context_type cc;
            cc.x = c.x1[i];
            cc.y = c.x2[i];
            cc.z = c.x3[i];
            double yy = tree_eval::eval( t.data() , cc );
            chi2 += ( yy - c.y[i] ) * ( yy - c.y[i] );
        }
        chi2 /= double( c.x1.size() );
        return - 1.0 / ( 1.0 + chi2 ); 
    }

    double operator()( tree_type &t , const context_type &c ) const
    {
        node_vector double_terminals;
        find_double_terminals( t , double_terminals );

        // GP_LOG_LEVEL_MODULE( gp::LogLevel::PROGRESS , gp::FITNESS )
        //     << "Calculating fitness for " << gp::simple( t )
        //     << ". Number of double terminals " << double_terminals.size();

        double f = 0.0;
        if( double_terminals.empty() )
        {
            f = fitness( t , c );
        }
        else
        {
            f = de_optimization( double_terminals , t , c );
        }

        // GP_LOG_LEVEL_MODULE( gp::LogLevel::PROGRESS , gp::FITNESS )
        //     << "Fitness for " << gp::simple( t )
        //     << " is " << f;

        return f;
    }


    static void find_double_terminals( tree_type &t , node_vector &nodes )
    {
        find_double_terminals_impl( t.data() , nodes );
    }


    static void find_double_terminals_impl( node_type *n , node_vector &nodes )
    {
        if( n->arity == 0 )
        {
            if( n->value.which() == 1 )
                nodes.push_back( n );
        }
        else if( n->arity == 1 )
        {
            find_double_terminals_impl( n->children[0] , nodes );
        }
        else if( n->arity == 2 )
        {
            find_double_terminals_impl( n->children[0] , nodes );
            find_double_terminals_impl( n->children[1] , nodes );
        }
        else
        {
            throw std::runtime_error( "find_double_terminals_impl() : more then two chilren are not allowed" );
        }
    }


    struct de_fitness_function 
    {
        const context_type &m_c;
        tree_type &m_tree;
        node_vector &m_nodes;

        de_fitness_function( const context_type &c , tree_type &tree , node_vector &nodes )
            : m_c( c ) , m_tree( tree ) , m_nodes( nodes ) { }

        double operator()( de::DVectorPtr args )
        {
            auto &constants = *args;
            for( size_t i=0 ; i<constants.size() ; ++i )
                m_nodes[i]->value = constants[i];
            return fitness( m_tree , m_c );
        }

        std::string name() const { return "Fitness function"; }
    };


    static double de_optimization( node_vector &double_terminals , tree_type &t , const context_type &c )
    {
        std::cout.precision( 14 );

        typedef de_fitness_function objective_function;
        typedef boost::shared_ptr< objective_function > objective_function_ptr;

        const size_t vars_count = double_terminals.size();
        const size_t population_size = 20;

        // GP_LOG_LEVEL_MODULE( gp::LogLevel::PROGRESS , gp::DE )
        //     << "Starting optimization for " << gp::simple( t )
        //     << ". Fitness befor de optimization " << fitness( t , c );

        de::constraints_ptr constraints( boost::make_shared< de::constraints >( vars_count , -1.0e6, 1.0e6 ) );
        for( size_t i=0 ; i<vars_count ; ++i )
            (*constraints)[ i ] = boost::make_shared< de::real_constraint >( -10.0 , 10.0 );

        objective_function_ptr of( boost::make_shared< de_fitness_function >( c , t , double_terminals ) );

        // de::listener_ptr listener( boost::make_shared< my_listener >( std::cout ) );
        de::listener_ptr listener( boost::make_shared< de::null_listener >() );
        de::processor_listener_ptr processor_listener( boost::make_shared< de::null_processor_listener >() );

        de::processors< objective_function_ptr >::processors_ptr processors(
            boost::make_shared< de::processors< objective_function_ptr > >( 1 , of, processor_listener ) );

        de::termination_strategy_ptr terminationStrategy( boost::make_shared< de::max_gen_termination_strategy >( 10 ) );
        de::selection_strategy_ptr selectionStrategy( boost::make_shared< de::best_parent_child_selection_strategy >() );

        de::mutation_strategy_arguments mutation_arguments( 0.5, 0.9 );
        de::mutation_strategy_ptr mutationStrategy( boost::make_shared< de::mutation_strategy_1 >( vars_count, mutation_arguments ) );
        de::differential_evolution< objective_function_ptr > de( vars_count, population_size, processors,
                                                             constraints, true, terminationStrategy, selectionStrategy, mutationStrategy, listener );

        de.run();

        de::individual_ptr best( de.best() );

        // we have to copy the result back into the tree, since the result is selected from more then one de generation
        const auto &res = *best->vars();
        for( size_t i=0 ; i<vars_count ; ++i )
            double_terminals[i]->value = res[i];

        // GP_LOG_LEVEL_MODULE( gp::LogLevel::PROGRESS , gp::DE )
        //     << "Finished optimization for " << gp::simple( t )
        //     << ". Fitness after de optimization " << fitness( t , c ) << " , " << best->cost();

        return best->cost();
    }

};



#endif // FITNESS_HPP_INCLUDED
