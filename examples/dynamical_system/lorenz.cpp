/*
 * lorenz.cpp
 * Date: 2016-01-26
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/app.hpp>
#include <gpcxx/eval.hpp>
#include <gpcxx/evolve.hpp>
#include <gpcxx/tree.hpp>
#include <gpcxx/intrusive_nodes.hpp>
#include <gpcxx/operator.hpp>
#include <gpcxx/stat.hpp>
#include <gpcxx/generate.hpp>
#include <gpcxx/io.hpp>

#include <boost/numeric/odeint/stepper/runge_kutta4.hpp>
#include <boost/numeric/odeint/integrate/integrate_const.hpp>

#include <vector>
#include <array>
#include <cmath>

static const size_t dim = 3;
using state_type = std::array< double , dim >;
using stepper_type = boost::numeric::odeint::runge_kutta4< state_type > ;
using state_container = std::vector< state_type >;
using data_type = std::pair< state_container , state_container >;


using context_type = gpcxx::regression_context< double , dim >;
using node_type = gpcxx::intrusive_named_func_node< double , const context_type > ;
using tree_type = gpcxx::intrusive_tree< node_type >;
using individual_type = std::array< tree_type , dim >;




void lorenz( const state_type &x , state_type &dxdt , double t )
{
    const double sigma = 10.0;
    const double R = 28.0;
    const double b = 8.0 / 3.0;
    dxdt[0] = sigma * ( x[1] - x[0] );
    dxdt[1] = R * x[0] - x[1] - x[0] * x[2];
    dxdt[2] = -b * x[2] + x[0] * x[1];
}

struct lorenz_reconstructed
{
    lorenz_reconstructed( individual_type individual ) : m_individual( std::move( individual ) ) {}
    void operator()( state_type const& x , state_type& dxdt , double t ) const
    {
        dxdt[0] = m_individual[0].root()->eval( x );
        dxdt[1] = m_individual[1].root()->eval( x );
        dxdt[2] = m_individual[2].root()->eval( x );
    }
    individual_type m_individual;
};


data_type generate_data( void )
{
    data_type data;
    state_type x { 10.0 , 10.0 , 10.0 };
    state_type dxdt;
    double dt = 0.1;
    stepper_type stepper;
    for( double t = 0.0 ; t<100.0 ; t+= dt )
    {
        lorenz( x , dxdt , t );
        data.first.push_back( x );
        data.second.push_back( dxdt );
        stepper.do_step( lorenz , x , dxdt , t , dt );
    }
    return data;
}

void plot_data( data_type const& data )
{
    for( size_t i=0 ; i<data.first.size() ; ++i )
    {
        state_type const& x = data.first[i];
        state_type const& dxdt = data.second[i];
        std::cout << x[0] << " " << x[1] << " " << x[2] << " " << dxdt[0] << " " << dxdt[1] << " " << dxdt[2] << std::endl;
    }
}

std::array< std::pair< double , double > , dim > normalize_data( state_container& data )
{
    std::array< std::pair< double , double > , dim > means;
    for( size_t j=0 ; j<dim ; ++j )
    {
        means[j].first = means[j].second = 0.0;
    }
        
    for( size_t i=0 ; i<data.size() ; ++i )
    {
        assert( data[i].size() == dim );
        for( size_t j=0 ; j<dim ; ++j )
        {
            means[j].first += data[i][j];
            means[j].second += data[i][j] * data[i][j];
        }
    }
    double len = double( data.size() );
    for( size_t j=0 ; j<dim ; ++j )
    {
        double mean = means[j].first / len;
        double sqmean = means[j].second / len;
        means[j].first = mean;
        means[j].second = sqrt( sqmean - mean * mean );
    }
    for( size_t i=0 ; i<data.size() ; ++i )
    {
        for( size_t j=0 ; j<dim ; ++j )
        {
            data[i][j] = ( data[i][j] - means[j].first ) / means[j].second;
        }
    }

    return means;
}

template< typename Pop , typename Fitness >
void write_best_individuals( std::ostream &out , const Pop& p , const Fitness &f , size_t num_individuals , bool write_individual = false )
{
    std::vector< size_t > idx;
    gpcxx::sort_indices( f , idx );
    for( size_t i=0 ; i<num_individuals ; ++i )
    {
        if( write_individual )
        {
            out << i << " " << f[ idx[i] ] << " : \n";
            for( auto const& tree : p[ idx[i] ] )
                out << "\t" << gpcxx::simple( tree ) << "\n";
        }
        else
        {
            out << i << " " << f[ idx[i] ] << "\n";
        }
    }
    out << "\n" << std::flush;
}



bool is_number( double t )
{
    return ( ! std::isnan( t ) ) && ( ! std::isinf( t ) );
}


int main( int argc , char** argv )
{
    std::cout.precision( 14 );
    using namespace std;
    auto training_data = generate_data();
    auto xstat = normalize_data( training_data.first );
    auto ystat = normalize_data( training_data.second );
    for( size_t j=0 ; j<dim ; ++j )
        cout << xstat[j].first << " " << xstat[j].second << " " << ystat[j].first << " " << ystat[j].second << "\n";
    cout << endl;
    // plot_data( training_data );
    
    //[ define_the_tree
    using rng_type = std::mt19937;
    rng_type rng;
    //]
    
    
    
    //[ define_terminal_set
    auto erc_gen = gpcxx::make_intrusive_erc_generator< node_type >( []( auto& rng ) {
        std::normal_distribution<> dist( 0.0 , 1.0 );
        return dist( rng ); } );
    auto terminal_gen = gpcxx::make_uniform_symbol_erc< node_type >(
        std::vector< node_type >{
            node_type { gpcxx::array_terminal< 0 >{}                                 ,      "x" } ,
            node_type { gpcxx::array_terminal< 1 >{}                                 ,      "y" } ,
            node_type { gpcxx::array_terminal< 2 >{}                                 ,      "z" } } ,
        0.5 ,
        erc_gen );
    //]

    //[ define_function_set
//     auto unary_gen = gpcxx::make_uniform_symbol( std::vector< node_type > {
//         node_type { gpcxx::sin_func {}                                               ,      "s" } ,
//         node_type { gpcxx::cos_func {}                                               ,      "c" } 
// //         node_type { gpcxx::exp_func {}                                               ,      "e" } ,
// //         node_type { gpcxx::log_func {}                                               ,      "l" }
//     } );

    auto binary_gen = gpcxx::make_uniform_symbol( std::vector< node_type > {
        node_type { gpcxx::plus_func {}                                              ,      "+" } ,
        node_type { gpcxx::minus_func {}                                             ,      "-" } ,
        node_type { gpcxx::multiplies_func {}                                        ,      "*" } ,
        node_type { gpcxx::divides_func {}                                           ,      "/" }
    } );
    //]

    //[ define_node_generator
    auto node_generator = gpcxx::node_generator< node_type , rng_type , 2 > {
        { 1.0 , 0 , terminal_gen } ,
//        { 1.0 , 1 , unary_gen } ,
        { 1.0 , 2 , binary_gen } };
    //]
        
        
    //[ define_gp_parameters
    size_t population_size = 512 * 32;
    size_t generation_size = 2000;
    size_t number_elite = 1;
    double mutation_rate = 0.2;
    double crossover_rate = 0.6;
    double reproduction_rate = 0.3;
    size_t min_tree_height = 1 , max_tree_height = 8;
    size_t tournament_size = 15;
    //]

        
    //[ define_population_and_fitness
    using population_type = std::vector< individual_type >;
    using fitness_type = std::vector< double >;
    
    fitness_type fitness ( population_size , 0.0 );
    population_type population ( population_size );
    //]
    
    
    //[ define_evolution
    using evolver_type = gpcxx::dynamic_pipeline< population_type , fitness_type , rng_type >;
    evolver_type evolver( rng , number_elite );
    //]

    //[define_evaluator
    auto evaluator = []( auto const& individual , auto const& context ) {
        return state_type {
            individual[0].root()->eval( context ) ,
            individual[1].root()->eval( context ) ,
            individual[2].root()->eval( context ) }; };
    auto distance = []( auto const& y , auto const& ytrain ) {
        const double weight_x = 1.0;
        const double weight_y = 1.0;
        const double weight_z = 1.0;
        state_type diff { y[0] - ytrain[0] , y[1] - ytrain[1] , y[2] - ytrain[2] };
        return weight_x * diff[0] * diff[0] + weight_y * diff[1] * diff[1] + weight_z * diff[2] * diff[2]; };
    //]
        
    //[define_genetic_operators
    auto tree_generator = gpcxx::make_ramp( rng , node_generator , min_tree_height , max_tree_height , 0.5 );
    auto fitness_f1 = gpcxx::make_multi_regression_fitness( evaluator , distance );
    auto fitness_f = [ fitness_f1 ]( auto const& individual , auto const& x , auto const& y ) {
        return fitness_f1.get_chi2( individual , x , y ); };
    evolver.add_operator( gpcxx::make_multi_mutation(
            rng , 
            gpcxx::make_point_mutation( rng , tree_generator , max_tree_height , 20 ) ,
            gpcxx::make_tournament_selector( rng , tournament_size ) )
        , mutation_rate );
    evolver.add_operator( gpcxx::make_multi_crossover( 
            rng ,
            gpcxx::make_one_point_crossover_strategy( rng , 10 ) ,
            gpcxx::make_tournament_selector( rng , tournament_size ) )
        , crossover_rate );
    evolver.add_operator( gpcxx::make_reproduce(
            gpcxx::make_tournament_selector( rng , tournament_size ) )
        , reproduction_rate );
    //]

    

    //[init_population
    std::ofstream fout( "evolution.dat" );
    for( size_t i=0 ; i<population.size() ; )
    {
        for( size_t j=0 ; j<dim ; ++j )
        {
            population[i][j].clear();
            tree_generator( population[i][j] );
        }
        fitness[i] = fitness_f( population[i] , training_data.first , training_data.second );
        if( ! is_number( fitness[i] ) )
        {
            continue;
        }
        ++i;
    }
    
    std::cout << "Initial population" << std::endl;
    write_best_individuals( std::cout , population , fitness , 10 );
    write_best_individuals( fout , population , fitness , 10 , true );
    //]

    //[main_loop
    for( size_t i=0 ; i<generation_size ; ++i )
    {
        evolver.next_generation( population , fitness );
        for( size_t i=0 ; i<population.size() ; ++i )
            fitness[i] = fitness_f( population[i] , training_data.first , training_data.second );
            
        std::cout << "Iteration " << i << std::endl;
        write_best_individuals( std::cout , population , fitness , 10 );
        write_best_individuals( fout , population , fitness , 10 , true );
    }
    //]



    
    return 0;
}
