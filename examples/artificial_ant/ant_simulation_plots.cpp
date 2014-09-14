/*
 * gpcxx/examples/artificial_ant/ant_simulation_plots.cpp
 * Date: 2014-09-01
 * Author: Gerard Choinka (gerard.choinka+gpcxx@gmail.com)
 * Copyright: Gerard Choinka
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include "ant_simulation.hpp"
#include "ant_simulation_nodes.hpp"
#include "santa_fe_trail.hpp"

#include <gpcxx/app.hpp>
#include <gpcxx/evolve.hpp>
#include <gpcxx/generate.hpp>
#include <gpcxx/io.hpp>
#include <gpcxx/operator.hpp>
#include <gpcxx/stat.hpp>

#include <string>
#include <iostream>
#include <sstream>
#include <random>
#include <algorithm>
#include <tuple>

using run_ant_result = struct {
    double time;
    size_t generation;
    bool has_optimal_fitness;
    int best_result;
};


run_ant_result run_ant_gp(
    size_t const population_size = 8192,
    size_t const generation_max = 200,
    double const number_elite = 2,
    double const mutation_rate = 0.1,
    double const crossover_rate = 0.6,
    double const reproduction_rate = 0.3,
    size_t const min_tree_height = 5,
    size_t const max_tree_height = 5,
    size_t const tournament_size = 15
)
{
    using namespace ant_example;
    using rng_type = std::mt19937;
    rng_type rng;
    char const newl { '\n' };
    
    board const b{ santa_fe::x_size, santa_fe::y_size };
    int const max_steps { 400 };
    ant_simulation::food_trail_type const santa_fe_trail { santa_fe::make_santa_fe_trail( b ) };
    ant_simulation const                  ant_sim_santa_fe{ santa_fe_trail, b.get_size_x(), b.get_size_y(), { 0, 0 }, east, max_steps };

    gpcxx::uniform_symbol< node_type > terminal_gen { std::vector< node_type >{
        node_type { ant_move_task_terminal{} ,          "m" } ,
        node_type { ant_turn_left_task_terminal{} ,     "l" } ,
        node_type { ant_turn_right_task_terminal{} ,    "r" } 
    } };
    
    gpcxx::uniform_symbol< node_type > binary_gen { std::vector< node_type > {
        node_type { prog2{} , "p2" } ,
        node_type { if_food_ahead{} , "if" }
    } };
    
    gpcxx::uniform_symbol< node_type > ternary_gen { std::vector< node_type > {
        node_type { prog3{} , "p3" }
    } };
    
    gpcxx::node_generator< node_type , rng_type , 3 > node_generator {
        { double( terminal_gen.num_symbols() ) , 0 , terminal_gen } ,
        { double( binary_gen.num_symbols() ) , 2 , binary_gen } ,
        { double( ternary_gen.num_symbols() ) , 3 , ternary_gen } };
    

    population_type population( population_size );
    
    auto tree_generator = gpcxx::make_basic_generate_strategy( rng , node_generator , min_tree_height , max_tree_height );
    for( auto & individum :  population )
        tree_generator( individum );

    using evolver_type = gpcxx::static_pipeline< population_type , fitness_type , rng_type > ;
    evolver_type evolver( number_elite , mutation_rate , crossover_rate , reproduction_rate , rng );

    evolver.mutation_function() = gpcxx::make_mutation(
         gpcxx::make_point_mutation( rng , tree_generator , max_tree_height , 20 ) ,
         gpcxx::make_tournament_selector( rng , tournament_size ) );
    
    evolver.crossover_function() = gpcxx::make_crossover( 
        gpcxx::make_one_point_crossover_strategy( rng , max_tree_height ) ,
        gpcxx::make_tournament_selector( rng , tournament_size ) );
    
    evolver.reproduction_function() = gpcxx::make_reproduce( gpcxx::make_tournament_selector( rng , tournament_size ) );
    
    evaluator       fitness_f;
    fitness_type    fitness( population_size , 0.0 );
    
    gpcxx::timer overall_timer; 
    bool    has_optimal_fitness { false };
    size_t  generation { 0 };
    do
    {
        gpcxx::timer iteration_timer;
        if( generation != 0 )
            evolver.next_generation( population , fitness );
        double evolve_time { iteration_timer.seconds() };
        
        iteration_timer.restart();
        std::transform( population.begin() , population.end() , fitness.begin() , [&]( tree_type const &t ) { 
            return fitness_f( t , ant_sim_santa_fe ); 
        } );
        double eval_time = iteration_timer.seconds();
             
        generation++;
        has_optimal_fitness = ( 0 == *std::min_element( fitness.begin(), fitness.end() ) );
    } while( !has_optimal_fitness && generation < generation_max );
    
    auto fittest_individual_position = std::distance( fitness.begin(), std::min_element( fitness.begin(), fitness.end() ) ); 
    tree_type const & fittest_individual { population[fittest_individual_position] };
      
    return { overall_timer.seconds() , generation , has_optimal_fitness, *std::min_element( fitness.begin(), fitness.end() ) };
}


template<typename T>
class frange
{
public:
    frange() = default;
    frange(frange const &) = default;
    
    frange(T start, T end, T step)
    :m_start(start), m_end(end), m_step(step)
    { 
    }
    
    frange(T start, T end)
    :m_start(start), m_end(end), m_step(1)
    { 
    }
    
    frange(T start)
    :m_start(start), m_end(start), m_step(1)
    { 
    }
    
    T value() const 
    {
        return m_start + (m_step * iteration);
    }
    
    bool has_next() const
    {
        return iteration < iteration_max();
    }
    
    void make_step() 
    {
        if(iteration < iteration_max())
            ++iteration;
    }
    
private:
    int iteration_max() const
    {   
        return static_cast<int>((m_end-m_start)/m_step);
    }
    
private:
    T m_start;
    T m_end;
    T m_step;
    int iteration;
};

using arguments_type = std::unordered_map< std::string, frange< double > >;

run_ant_result run_ant_gp_wrapper(
    arguments_type arguments,
    std::ostream & out
)
{
    out << "# i " <<  "\t" 
        << "res.generation" << "\t" 
        << "res.time" << "\t" 
        << "res.has_optimal_fitness" << "\t" 
        << "res.best_result" << "\t"  
        << "res.time / res.generation" << "\t" 
        << "population_size" << "\t"
        << "generation_max" << "\t"
        << "number_elite" << "\t"
        << "mutation_rate" << "\t"
        << "crossover_rate" << "\t"
        << "reproduction_rate" << "\t"
        << "min_tree_height" << "\t"
        << "max_tree_height" << "\t"
        << "tournament_size" 
        << "\n";
    size_t iteration = 0;
    bool any_has_next = false;
    do
    {
        auto population_size =   arguments["population_size"].value();
        auto generation_max =    arguments["generation_max"].value();
        auto number_elite =      arguments["number_elite"].value();
        auto mutation_rate =     arguments["mutation_rate"].value();
        auto crossover_rate =    arguments["crossover_rate"].value();
        auto reproduction_rate = arguments["reproduction_rate"].value();
        auto min_tree_height =   arguments["min_tree_height"].value();
        auto max_tree_height =   arguments["max_tree_height"].value();
        auto tournament_size =   arguments["tournament_size"].value();
    
        auto res = run_ant_gp(
            population_size,
            generation_max,
            number_elite,
            mutation_rate,
            crossover_rate,
            reproduction_rate,
            min_tree_height,
            max_tree_height,
            tournament_size                   
        );
        
        out << iteration <<  "\t" 
            << res.generation << "\t" 
            << res.time << "\t" 
            << res.has_optimal_fitness << "\t" 
            << res.best_result << "\t"  
            << res.time / res.generation << "\t" 
            << population_size << "\t"
            << generation_max << "\t"
            << number_elite << "\t"
            << mutation_rate << "\t"
            << crossover_rate << "\t"
            << reproduction_rate << "\t"
            << min_tree_height << "\t"
            << max_tree_height << "\t"
            << tournament_size 
            << "\n";
        ++iteration;
        any_has_next = std::any_of(arguments.begin(), arguments.end(), [](decltype(arguments)::value_type const & v){ 
            return v.second.has_next(); 
        });
        for_each(arguments.begin(), arguments.end(), [](decltype(arguments)::value_type & v){ 
            if(v.second.has_next()) v.second.make_step(); 
        });
        
    }while(any_has_next);
}


int main( int argc , char *argv[] )
{
    arguments_type  const default_arguments 
    {
        { "population_size" ,   frange< double >( 1000 ) },
        { "generation_max" ,    frange< double >( 1000) },
        { "number_elite" ,      frange< double >( 2 ) },
        { "mutation_rate" ,     frange< double >( 0.1 ) },
        { "crossover_rate" ,    frange< double >( 0.6 ) },
        { "reproduction_rate" , frange< double >( 0.3 ) },
        { "min_tree_height" ,   frange< double >( 1 ) },
        { "max_tree_height" ,   frange< double >( 8 ) },
        { "tournament_size" ,   frange< double >( 15 ) }
    };
    
    std::unordered_map<std::string, arguments_type > variations 
    {
        { "population_variation.dat",        { { "population_size", frange< double >( 25, 1000, 25 ) } } },
        { "number_elite_variation.dat",      { { "number_elite",    frange< double >( 1, 100, 5 ) } } },
        { "mutation_rate_variation.dat",     { { "mutation_rate",   frange< double >( 0, 1, 0.05 ) } } },
        { "crossover_rate_variation.dat",    { { "crossover_rate",  frange< double >( 0, 1, 0.05 ) } } },
        { "reproduction_rate_variation.dat", { { "crossover_rate",  frange< double >( 0, 1, 0.05 ) } } },
        { "min_tree_height_variation.dat",   { { "min_tree_height", frange< double >( 1, 16, 1 ) }, { "max_tree_height", frange< double >( 16 ) } } },
        { "max_tree_height_variation.dat",   { { "min_tree_height", frange< double >( 1  ) }, { "max_tree_height", frange< double >( 1, 16, 1 ) } } },
        { "tournament_size_variation.dat",   { { "tournament_size", frange< double >( 1, 50, 5 ) } } }
    };
    
    for( auto const & task : variations )
    {
        arguments_type default_arguments_copy{ task.second };
        default_arguments_copy.insert(default_arguments.begin(), default_arguments.end());
    
        std::ofstream outfile{ task.first };
        run_ant_gp_wrapper(default_arguments_copy, outfile);
    }
}
