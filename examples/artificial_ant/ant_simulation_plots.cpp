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


#include <boost/lexical_cast.hpp>
#include <boost/range/numeric.hpp>

#include <string>
#include <iostream>
#include <sstream>
#include <random>
#include <algorithm>
#include <tuple>
#include <cmath>

using run_ant_result = struct 
{
    double time;
    size_t generation;
    bool has_optimal_fitness;
    int best_result;
};

using run_ant_result_avg = struct 
{
    size_t count;
    double time;
    double avg_generations;
    double min_generations;
    double max_generations;
    double std_generations;
    double avg_fitness;
    double min_fitness;
    double max_fitness;
    double std_fitness;
    int optimal_fitness_count;
};

run_ant_result_avg make_stat(std::vector<run_ant_result> runs)
{
    run_ant_result_avg avg_result { runs.size(), 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 ,  0 };
    if( runs.empty() )
        return avg_result;
    
    avg_result.min_generations = runs.front().generation;
    avg_result.max_generations = runs.front().generation;
    avg_result.min_fitness = runs.front().best_result;
    avg_result.max_fitness = runs.front().best_result;
    for(run_ant_result oner : runs)
    {
        avg_result.time += oner.time / runs.size();
        avg_result.avg_generations += (double)oner.generation / runs.size();
        avg_result.avg_fitness += (double)oner.best_result / runs.size();
        avg_result.optimal_fitness_count += oner.has_optimal_fitness ? 1 : 0;      
        avg_result.min_generations = std::min( avg_result.min_generations, (double)oner.generation );
        avg_result.max_generations = std::max( avg_result.max_generations, (double)oner.generation );
        avg_result.min_fitness = std::min( avg_result.min_fitness, (double)oner.best_result );
        avg_result.max_fitness = std::max( avg_result.max_fitness, (double)oner.best_result );
    }
    
    double var_generations = 0;
    double var_fitness = 0;
    for(run_ant_result oner : runs)
    {
        var_generations += ( oner.generation - avg_result.avg_generations ) * ( oner.generation - avg_result.avg_generations );
        var_fitness += ( oner.best_result - avg_result.avg_fitness ) * ( oner.best_result - avg_result.avg_fitness );
    }
    avg_result.std_generations = std::sqrt(var_generations);
    avg_result.std_fitness = std::sqrt(var_fitness);
    
    return avg_result;
}


using rng_type = std::mt19937;


run_ant_result run_ant_gp(
    size_t const population_size,
    size_t const generation_max,
    double const number_elite,
    double const mutation_rate,
    double const crossover_rate,
    double const crossover_pip_rate,
    double const reproduction_rate,
    size_t const min_tree_height,
    size_t const init_max_tree_height,
    size_t const max_tree_height,
    size_t const tournament_size,
    rng_type rng
)
{
    using namespace ant_example;
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
    
    auto tree_generator      = gpcxx::make_basic_generate_strategy( rng , node_generator , min_tree_height , max_tree_height );
    auto init_tree_generator = gpcxx::make_basic_generate_strategy( rng , node_generator , min_tree_height , init_max_tree_height );
    for( auto & individum :  population )
        init_tree_generator( individum );

    using evolver_type = gpcxx::static_pipeline< population_type , fitness_type , rng_type > ;
    evolver_type evolver( number_elite , mutation_rate , crossover_rate , reproduction_rate , rng );

    evolver.mutation_function() = gpcxx::make_mutation(
         gpcxx::make_point_mutation( rng , tree_generator , max_tree_height , 20 ) ,
         gpcxx::make_tournament_selector( rng , tournament_size ) );
    
    evolver.crossover_function() = gpcxx::make_crossover(
        gpcxx::make_one_point_crossover_pip_strategy( rng , max_tree_height, crossover_pip_rate ) ,
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
    tree_type const & fittest_individual = population[fittest_individual_position];
      
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
    std::ostream & out,
    size_t avarage_over
)
{
    
    rng_type rng;

    
    out << "i " <<  "\t" 
        << "count" << "\t" 
        << "avg_time" << "\t" 
        << "avg_generations" << "\t" 
        << "min_generations" << "\t"
        << "max_generations" << "\t"  
        << "std_generations" << "\t"  
        << "avg_fitness" << "\t" 
        << "min_fitness" << "\t"
        << "max_fitness" << "\t"  
        << "std_fitness" << "\t"  
        << "optimal_fitness_count" << "\t" 
        << "population_size" << "\t"
        << "generation_max" << "\t"
        << "number_elite" << "\t"
        << "mutation_rate" << "\t"
        << "crossover_rate" << "\t"
        << "crossover_pip_rate" << "\t"
        << "reproduction_rate" << "\t"
        << "min_tree_height" << "\t"
        << "init_max_tree_height" << "\t"
        << "max_tree_height" << "\t"
        << "tournament_size" 
        << std::endl;
    size_t iteration = 0;

    bool any_has_next = false;
    do
    {
        auto population_size =      arguments["population_size"].value();
        auto generation_max =       arguments["generation_max"].value();
        auto number_elite =         arguments["number_elite"].value();
        auto mutation_rate =        arguments["mutation_rate"].value();
        auto crossover_rate =       arguments["crossover_rate"].value();
        auto crossover_pip_rate =   arguments["crossover_pip_rate"].value();
        auto reproduction_rate =    arguments["reproduction_rate"].value();
        auto min_tree_height =      arguments["min_tree_height"].value();
        auto init_max_tree_height = arguments["init_max_tree_height"].value();
        auto max_tree_height =      arguments["max_tree_height"].value();
        auto tournament_size =      arguments["tournament_size"].value();
    
        std::vector<run_ant_result> results;
        for(int avg_run = 0; avg_run < avarage_over; ++avg_run)
        {
            rng.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
            results.emplace_back(run_ant_gp(
                population_size,
                generation_max,
                number_elite,
                mutation_rate,
                crossover_rate,
                crossover_pip_rate,
                reproduction_rate,
                min_tree_height,
                init_max_tree_height,
                max_tree_height,
                tournament_size,
                rng
            ));
         }
        auto stats = make_stat(results);
        out << std::fixed;
        out << iteration <<  "\t"
            << stats.count << "\t" 
            << stats.time << "\t" 
            << stats.avg_generations << "\t" 
            << stats.min_generations << "\t"
            << stats.max_generations << "\t"  
            << stats.std_generations << "\t"  
            << stats.avg_fitness << "\t" 
            << stats.min_fitness << "\t"
            << stats.max_fitness << "\t"  
            << stats.std_fitness << "\t"  
            << stats.optimal_fitness_count << "\t" 
            << population_size << "\t"
            << generation_max << "\t"
            << number_elite << "\t"
            << mutation_rate << "\t"
            << crossover_rate << "\t"
            << crossover_pip_rate << "\t"
            << reproduction_rate << "\t"
            << min_tree_height << "\t"
            << init_max_tree_height << "\t"
            << max_tree_height << "\t"
            << tournament_size 
            << std::endl;
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
        { "population_size" ,   frange< double >( 500 ) },
        { "generation_max" ,    frange< double >( 1000 ) },
        { "number_elite" ,      frange< double >( 2 ) },
        { "mutation_rate" ,     frange< double >( 0 ) },
        { "crossover_rate" ,    frange< double >( 0.9 ) },
        { "crossover_pip_rate" ,frange< double >( 0.9 ) },
        { "reproduction_rate" , frange< double >( 0.1 ) },
        { "min_tree_height" ,   frange< double >( 6 ) },
        { "init_max_tree_height" , frange< double >( 6 ) },
        { "max_tree_height" ,   frange< double >( 17 ) },
        { "tournament_size" ,   frange< double >( 7 ) }
    };

    if(((argc - 3) % 4)  != 0 || argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << 
        " <filename> <avg_over_n> <argumentname> <range_start> <range_end> <range_step> [[<argumentname> <range_start> <range_end> <range_step>] [...]] " << std::endl;
        return 1;
    }
    std::string filename { argv[1] };
    int avg_over_n    = boost::lexical_cast<int>( argv[2] );
    arguments_type arguments;
    for(int i = 0; i < ((argc - 3) / 4); ++i)
    {
        size_t offset = (i*4)+3;
        std::string  argname { argv[offset + 0] };
        double start    = boost::lexical_cast<double>( argv[offset + 1] );
        double end      = boost::lexical_cast<double>( argv[offset + 2] );
        double step     = boost::lexical_cast<double>( argv[offset + 3] );
        arguments[argname] = frange<double>{ start, end, step };
    }
    std::unordered_map<std::string, arguments_type > variations{ { filename, arguments } };
    
    for( auto const & task : variations )
    {
        std::cout << "Task:" << task.first << "\n";
        arguments_type default_arguments_copy{ task.second };
        default_arguments_copy.insert(default_arguments.begin(), default_arguments.end());
    
        std::ofstream outfile{ task.first };
        run_ant_gp_wrapper(default_arguments_copy, outfile, avg_over_n);
    }
}