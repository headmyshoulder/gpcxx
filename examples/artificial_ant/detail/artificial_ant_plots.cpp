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
#include "simulation.hpp"
#include "nodes.hpp"
#include "santa_fe_trail.hpp"

#include <gpcxx/app.hpp>
#include <gpcxx/evolve.hpp>
#include <gpcxx/generate.hpp>
#include <gpcxx/io.hpp>
#include <gpcxx/operator.hpp>
#include <gpcxx/stat.hpp>


#include <boost/lexical_cast.hpp>
#include <boost/range/numeric.hpp>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/moment.hpp>
using namespace boost::accumulators;


#include <string>
#include <iostream>
#include <sstream>
#include <random>
#include <algorithm>
#include <tuple>
#include <cmath>

using rng_type = std::mt19937;

template<typename T>
class frange
{
public:
    frange()
        :frange( 0 , 0 , 1 )
    {
    }

    frange( frange const & ) = default;

    frange( T start , T end , T step )
        :m_start( start ) , m_end( end ) , m_step( step ) , m_iteration( 0 )
    {
    }

    frange( T start , T end )
        :frange( start , end , 1 )
    {
    }

    frange( T start )
        :frange( start, start, 1 )
    {
    }

    T value() const
    {
        return m_start + ( m_step * m_iteration );
    }

    bool has_next() const
    {
        return m_iteration < iteration_max();
    }

    void make_step()
    {
        if(m_iteration < iteration_max())
            ++m_iteration;
    }

private:
    int iteration_max() const
    {
        return static_cast< int >( ( m_end - m_start ) / m_step );
    }

private:
    T m_start;
    T m_end;
    T m_step;
    int m_iteration;
};

using arguments_type = std::unordered_map< std::string, frange< double > >;

struct ant_result
{
    double time;
    size_t generation;
    bool has_optimal_fitness;
    int fitness;
};

struct ant_result_stats
{
    ant_result_stats()
    : count(0),time(0.0),avg_generations(0.0),min_generations(0),max_generations(0),std_generations(0.0),avg_fitness(0.0),min_fitness(0),max_fitness(0),std_fitness(0.0),optimal_fitness_count(0)
    {
    }

    size_t count;
    double time;
    double avg_generations;
    size_t min_generations;
    size_t max_generations;
    double std_generations;
    double avg_fitness;
    double min_fitness;
    double max_fitness;
    double std_fitness;
    size_t optimal_fitness_count;
};

ant_result_stats make_stat(std::vector<ant_result> runs)
{
    ant_result_stats avg_runs;
    if( runs.empty() )
        return avg_runs;
    
    avg_runs.min_generations = runs.front().generation;
    avg_runs.max_generations = runs.front().generation;
    avg_runs.min_fitness = runs.front().fitness;
    avg_runs.max_fitness = runs.front().fitness;
    for(ant_result r : runs)
    {
        avg_runs.time += r.time / runs.size();
        avg_runs.avg_generations += double( r.generation ) / runs.size();
        avg_runs.avg_fitness     += double( r.fitness) / runs.size();
        avg_runs.optimal_fitness_count += r.has_optimal_fitness ? 1 : 0;
        avg_runs.min_generations = std::min( avg_runs.min_generations, r.generation );
        avg_runs.max_generations = std::max( avg_runs.max_generations, r.generation );
        avg_runs.min_fitness = std::min( avg_runs.min_fitness, double( r.fitness) );
        avg_runs.max_fitness = std::max( avg_runs.max_fitness, double( r.fitness) );
    }
    
    double var_generations = 0;
    double var_fitness = 0;
    for(ant_result r : runs)
    {
        var_generations += ( r.generation  - avg_runs.avg_generations ) * ( r.generation  - avg_runs.avg_generations );
        var_fitness     += ( r.fitness - avg_runs.avg_fitness )     * ( r.fitness - avg_runs.avg_fitness );
    }
    avg_runs.std_generations = std::sqrt(var_generations);
    avg_runs.std_fitness = std::sqrt(var_fitness);
    
    return avg_runs;
}


ant_result ant_simulation_gp(
    size_t const population_size,
    size_t const generation_max,
    double const number_elite,
    double const mutation_rate,
    double const crossover_rate,
    double const crossover_pip_rate,
    bool const   crossover_use_pip,
    double const reproduction_rate,
    size_t const min_tree_height,
    size_t const init_max_tree_height,
    size_t const max_tree_height,
    size_t const tournament_size,
    rng_type rng,
    bool print_generations,
    size_t current_run
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

    if(crossover_use_pip)
        evolver.crossover_function() = gpcxx::make_crossover(
                gpcxx::make_one_point_crossover_pip_strategy(rng, max_tree_height, crossover_pip_rate ),
                gpcxx::make_tournament_selector(rng, tournament_size));
    else
        evolver.crossover_function() = gpcxx::make_crossover(
                gpcxx::make_one_point_crossover_strategy(rng, max_tree_height ),
                gpcxx::make_tournament_selector(rng, tournament_size));

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

        if(print_generations)
        {
            std::cout << gpcxx::indent(0) << "Generation " << current_run << ":" << generation << newl;
            std::cout << gpcxx::indent(1) << "Evolve time " << evolve_time << newl;
            std::cout << gpcxx::indent(1) << "Eval time " << eval_time << newl;
            std::cout << gpcxx::indent(1) << "Best individuals\n" << gpcxx::best_individuals(population, fitness, 2, 3, false) << newl;
            std::cout << gpcxx::indent(1) << "Statistics : " << gpcxx::calc_population_statistics(population) << newl << std::endl;
        }
        
        generation++;
        has_optimal_fitness = ( 0 == *std::min_element( fitness.begin(), fitness.end() ) );
    } while( !has_optimal_fitness && generation < generation_max );

    return { overall_timer.seconds() , generation , has_optimal_fitness, *std::min_element( fitness.begin(), fitness.end() ) };
}






ant_result ant_simulation_gp_stats_helper(
    arguments_type arguments,
    std::ostream & out,
    size_t repeat_ntimes,
    bool print_header,
    bool print_generations
)
{
    rng_type rng;

    if(print_header) {
        out << "i " << "\t"
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
            << "crossover_use_pip" << "\t"
            << "reproduction_rate" << "\t"
            << "min_tree_height" << "\t"
            << "init_max_tree_height" << "\t"
            << "max_tree_height" << "\t"
            << "tournament_size"
            << std::endl;
    }
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
        auto crossover_use_pip =    arguments["crossover_use_pip"].value();
        auto reproduction_rate =    arguments["reproduction_rate"].value();
        auto min_tree_height =      arguments["min_tree_height"].value();
        auto init_max_tree_height = arguments["init_max_tree_height"].value();
        auto max_tree_height =      arguments["max_tree_height"].value();
        auto tournament_size =      arguments["tournament_size"].value();

        auto print_row = [&](std::ostream & os, std::vector<ant_result> const & results)
        {
            auto stats = make_stat(results);
            os << std::fixed
                << iteration << "\t"
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
                << crossover_use_pip << "\t"
                << reproduction_rate << "\t"
                << min_tree_height << "\t"
                << init_max_tree_height << "\t"
                << max_tree_height << "\t"
                << tournament_size
                << std::endl;
        };

        std::vector<ant_result> results;
        for( size_t i = 0; i < repeat_ntimes; ++i)
        {
            rng.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
            results.emplace_back(ant_simulation_gp(
                population_size,
                generation_max,
                number_elite,
                mutation_rate,
                crossover_rate,
                crossover_pip_rate,
                crossover_use_pip,
                reproduction_rate,
                min_tree_height,
                init_max_tree_height,
                max_tree_height,
                tournament_size,
                rng,
                print_generations,
                i
            ));
            print_row( std::cout , results );
        }
        print_row( out , results );
        ++iteration;
        any_has_next = std::any_of(arguments.begin(), arguments.end(), [](arguments_type::value_type const & v){
            return v.second.has_next(); 
        });
        for_each(arguments.begin(), arguments.end(), [](arguments_type::value_type & v){
            if(v.second.has_next()) v.second.make_step(); 
        });
    } while( any_has_next );
    
    return ant_result {};
}


int main( int argc , char *argv[] )
{
    int mandatory_args_count = 5;
    int range_args_count = 4;
    if(((argc - mandatory_args_count) % range_args_count)  != 0 || argc < mandatory_args_count)
    {
        std::cerr << "Usage: " << argv[0] <<
            " <filename> <avg_over_n> <print_header> <print_generations> "\
            "[[<argumentname> <range_start> <range_end> <range_step>]"\
            " [<argumentname> <range_start> <range_end> <range_step>] [...]] "
                        << std::endl;
        return 1;
    }

    arguments_type  const default_arguments
    {
        { "population_size" ,   frange< double >( 500 ) },
        { "generation_max" ,    frange< double >( 500 ) },
        { "number_elite" ,      frange< double >( 0 ) },
        { "mutation_rate" ,     frange< double >( 0.0 ) },
        { "crossover_rate" ,    frange< double >( 0.9 ) },
        { "crossover_pip_rate" ,frange< double >( 0.9 ) },
        { "crossover_use_pip" , frange< double >( 1 ) }   ,
        { "reproduction_rate" , frange< double >( 0.0 ) },
        { "min_tree_height" ,   frange< double >( 1 ) },
        { "init_max_tree_height" , frange< double >( 6 ) },
        { "max_tree_height" ,   frange< double >( 17 ) },
        { "tournament_size" ,   frange< double >( 7 ) }
    };


    std::string filename { argv[1] };
    size_t repeate_ntimes    = boost::lexical_cast<size_t>( argv[2] );
    bool print_header = boost::lexical_cast<bool>( argv[3] );
    bool print_generations = boost::lexical_cast<bool>( argv[4] );
    arguments_type arguments;

    for(int i = 0; i < ((argc - mandatory_args_count) / range_args_count); ++i)
    {
        size_t offset = (i*range_args_count)+mandatory_args_count;
        std::string  arg_name { argv[offset + 0] };
        double start    = boost::lexical_cast<double>( argv[offset + 1] );
        double end      = boost::lexical_cast<double>( argv[offset + 2] );
        double step     = boost::lexical_cast<double>( argv[offset + 3] );
        arguments[arg_name] = frange<double>{ start, end, step };
    }
    std::unordered_map<std::string, arguments_type > variations{ { filename, arguments } };
    
    for( auto const & task : variations )
    {
        arguments_type default_arguments_copy{ task.second };
        default_arguments_copy.insert(default_arguments.begin(), default_arguments.end());
    
        std::ofstream outfile{ task.first };
        ant_simulation_gp_stats_helper( default_arguments_copy, outfile, repeate_ntimes, print_header, print_generations );
    }
}
