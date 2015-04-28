/*
 * symb_reg_erc.cpp
 * Date: 2015-04-04
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/tree.hpp>
#include <gpcxx/generate.hpp>
#include <gpcxx/operator.hpp>
#include <gpcxx/eval.hpp>
#include <gpcxx/evolve.hpp>
#include <gpcxx/io.hpp>
#include <gpcxx/stat.hpp>
#include <gpcxx/app.hpp>
#include <gpcxx/util.hpp>

#include <iostream>
#include <random>
#include <vector>
#include <functional>
#include <ostream>
#include <fstream>
#include <tuple>


namespace detail {
    
    template< typename T >
    struct writer
    {
        static void write( std::ostream& out , T const& t )
        {
            out << t;
        }
    };
    
    template< typename T >
    void write( std::ostream& out , T const& t )
    {
        writer< T >::write( out , t );
    }
    
    template< typename T >
    struct writer< std::vector< T > >
    {
        static void write( std::ostream& out , std::vector< T > const& t )
        {
            out << "[ \n";
            if( ! t.empty() ) detail::write( out , t.front() );
            for( size_t i=1 ; i<t.size() ; ++i )
            {
                out << " , ";
                detail::write( out , t[i] );
            }
            out << " \n]";
        }
    };

} // namespace detail


struct evolution_part
{
    int op;
    size_t in;
    size_t out;
};



struct operator_observer
{
    using population_type = std::vector< std::tuple< double , std::string > >;
    using population_vector = std::vector< population_type >;
    
    using evolution_type = std::vector< evolution_part >;
    using evolution_vector = std::vector< evolution_type >;
    
    operator_observer( size_t population_size )
    : m_population_size( population_size ) , m_populations() , m_evolutions() , m_current( nullptr )
    {
    }
    
    template< typename Indices >
    void operator()( int choice , Indices const& in , Indices const& out )
    {
        for( auto i : in )
        {
            for( auto o : out )
            {
                m_current->push_back( evolution_part { choice , i , o } );
            }
        }
    }
    
    template< typename Pop , typename Fitness >
    void next_generation( Pop const& population , Fitness const& fitness )
    {
        assert( population.size() == fitness.size() );
        m_populations.push_back( population_type {} );
        for( size_t i=0 ; i<population.size() ; ++i )
        {
            m_populations.back().push_back( std::make_tuple( fitness[i] , gpcxx::simple_string( population[i] ) ) );
        }
        
        m_evolutions.push_back( evolution_type {} );
        m_current = & ( m_evolutions.back() );
    }
    
    void write_double( std::ostream& out , double x )
    {
        if( std::isnan( x ) ) out << "1.0";
        else out << x;
    }
    
    void write_nodes( std::ostream& out )
    {
        out << "[\n";
        bool first = true;
        for( size_t i=0 ; i<m_populations.size() ; ++i )
        {
            assert( m_populations[i].size() == m_population_size );
            for( size_t j=0 ; j<m_populations[i].size() ; ++j )
            {
                if( first )
                {
                    first = false;
                }
                else
                {
                    out << " , \n";
                }

                auto const& p = m_populations[i][j];
                out << "{ ";
                out << "\"generation\" : " << i << " , ";
                out << "\"name\" : \"" << std::to_string( j + 1 )<< "\" , ";
                out << "\"formula\" : \"" << std::get< 1 >( p ) << "\" , ";
                out << "\"value\" : "; write_double( out , std::get< 0 >( p ) ); out << " ";
                out << "}";
            }
        }
        out << "]\n";
    }
    
    void write_links( std::ostream& out )
    {
        out << "[\n";
        bool first = true;
        for( size_t i=0 ; i<m_evolutions.size() ; ++i )
        {
            for( size_t j=0 ; j<m_evolutions[i].size() ; ++j )
            {
                if( first )
                {
                    first = false;
                }
                else
                {
                    out << " , \n";
                }
                auto const& e = m_evolutions[i][j];
                out << "{";
                out << "\"source\" : " << i * m_population_size + e.in << " , ";
                out << "\"target\" : " << ( i + 1 ) * m_population_size + e.out << " , ";
                out << "\"op\" : " << e.op << " ";
                out << "}";
            }
        }
        out << "]\n";
    }

    
    void write( std::ostream& out )
    {
        out << "{\n";
        out << "  \"population_size\" : " << m_population_size << " , \n";
        out << "  \"nodes\" : "; write_nodes( out ); out << " , \n";
        out << "  \"links\" : "; write_links( out ); out << "\n";
        out << "}\n";
    }
    
    size_t m_population_size;
    population_vector m_populations;
    evolution_vector m_evolutions;
    evolution_type* m_current;
};



int main( int argc , char *argv[] )
{
    //[ create_training_data
    using rng_type = std::mt19937;
    rng_type rng;
    
    gpcxx::regression_training_data< double , 1 > c;
    gpcxx::generate_regression_test_data( c , 1024 , rng , []( double x )
            { return  x * x * x + 1.0 / 10.0 * x * x - 3.0 / 4.0 * x + 1.0 ; } );
    //]
    
    //[ define_tree_types
    using context_type = gpcxx::regression_context< double , 1 >;
    using node_type = gpcxx::intrusive_named_func_node< double , const context_type > ;
    using tree_type = gpcxx::intrusive_tree< node_type >;
    //]
    
    
    //[ define_terminal_set
    auto terminal_gen = gpcxx::make_uniform_symbol( std::vector< node_type >{
        node_type { []( context_type const& c , node_type const& n ) { return 1.0; } ,      "1" } ,
        node_type { []( context_type const& c , node_type const& n ) { return 2.0; } ,      "2" } ,
        node_type { []( context_type const& c , node_type const& n ) { return 3.0; } ,      "3" } ,
        node_type { []( context_type const& c , node_type const& n ) { return 4.0; } ,      "4" } ,
        node_type { []( context_type const& c , node_type const& n ) { return 5.0; } ,      "5" } ,
        node_type { []( context_type const& c , node_type const& n ) { return 6.0; } ,      "6" } ,
        node_type { []( context_type const& c , node_type const& n ) { return 7.0; } ,      "7" } ,
        node_type { []( context_type const& c , node_type const& n ) { return 8.0; } ,      "8" } ,
        node_type { []( context_type const& c , node_type const& n ) { return 9.0; } ,      "9" } ,
        node_type { gpcxx::array_terminal< 0 >{}                                     ,      "x" }
    } );
    //]

    //[ define_function_set
    auto unary_gen = gpcxx::make_uniform_symbol( std::vector< node_type > {
        node_type { gpcxx::sin_func {}                                               ,      "s" } ,
        node_type { gpcxx::cos_func {}                                               ,      "c" }
    } );

    auto binary_gen = gpcxx::make_uniform_symbol( std::vector< node_type > {
        node_type { gpcxx::plus_func {}                                              ,      "+" } ,
        node_type { gpcxx::minus_func {}                                             ,      "-" } ,
        node_type { gpcxx::multiplies_func {}                                        ,      "*" } ,
        node_type { gpcxx::divides_func {}                                           ,      "/" }
    } );
    //]

    //[ define_node_generator
    auto node_generator = gpcxx::node_generator< node_type , rng_type , 3 > {
        { 1.0 , 0 , terminal_gen } ,
        { 1.0 , 1 , unary_gen } ,
        { 1.0 , 2 , binary_gen } };
    //]

    //[ define_gp_parameters
    size_t population_size = 256;
    size_t generation_size = 15;
    size_t number_elite = 1;
    double mutation_rate = 0.3;
    double crossover_rate = 0.6;
    double reproduction_rate = 0.1;
    size_t min_tree_height = 2 , max_tree_height = 12;
    size_t tournament_size = 10;
    //]

        
    //[ define_population_and_fitness
    using population_type = std::vector< tree_type >;
    using fitness_type = std::vector< double >;
    
    fitness_type fitness( population_size , 0.0 );
    population_type population( population_size );
    //]
    
    //[ define_evolution
    using evolver_type = gpcxx::dynamic_pipeline< population_type , fitness_type , rng_type >;
    evolver_type evolver( rng , number_elite );
    //]

    //[define_evaluator
    using evaluator = struct {
        using context_type = gpcxx::regression_context< double , 1 >;
        using value_type = double;
        value_type operator()( tree_type const& t , context_type const& c ) const {
            return t.root()->eval( c );
        } };
    //]
        
    //[define_genetic_operators
    auto tree_generator = gpcxx::make_ramp( rng , node_generator , min_tree_height , max_tree_height , 0.5 );
    auto fitness_f = gpcxx::make_regression_fitness( evaluator {} );
    evolver.add_operator( gpcxx::make_mutation(
            gpcxx::make_point_mutation( rng , tree_generator , max_tree_height , 20 ) ,
            gpcxx::make_tournament_selector( rng , tournament_size ) )
        , mutation_rate );
    evolver.add_operator( gpcxx::make_crossover( 
            // gpcxx::make_one_point_crossover_strategy( rng , 10 , 100 , 0.01 ) ,
            gpcxx::make_one_point_crossover_strategy( rng , max_tree_height ) ,
            gpcxx::make_tournament_selector( rng , tournament_size ) )
        , crossover_rate );
    evolver.add_operator( gpcxx::make_reproduce(
            gpcxx::make_tournament_selector( rng , tournament_size ) )
        , reproduction_rate );
    //]


    //[init_population
    for( size_t i=0 ; i<population.size() ; ++i )
    {
        tree_generator( population[i] );
        fitness[i] = fitness_f( population[i] , c );
    }
    
    std::cout << "Best individuals" << std::endl << gpcxx::best_individuals( population , fitness ) << std::endl;
    std::cout << "Statistics : " << gpcxx::calc_population_statistics( population ) << std::endl;
    std::cout << std::endl << std::endl;
    //]
    
    operator_observer obs { population.size() };
    evolver.operator_observer() = std::ref( obs );
    obs.next_generation( population , fitness );
    
    //[main_loop
    for( size_t i=0 ; i<generation_size ; ++i )
    {
        evolver.next_generation( population , fitness );
        for( size_t i=0 ; i<population.size() ; ++i )
            fitness[i] = fitness_f( population[i] , c );
        obs.next_generation( population , fitness );
        
        std::cout << "Iteration " << i << std::endl;
        std::cout << "Best individuals" << std::endl << gpcxx::best_individuals( population , fitness , 1 ) << std::endl;
        std::cout << "Statistics : " << gpcxx::calc_population_statistics( population ) << std::endl << std::endl;
    }
    //]
    
    std::ofstream fout( "evolution.dat" );
    obs.write( fout );

    return 0;
}
