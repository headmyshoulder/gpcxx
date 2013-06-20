/*
  gp/stat/population_statistics.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GP_STAT_POPULATION_STATISTICS_HPP_DEFINED
#define GP_STAT_POPULATION_STATISTICS_HPP_DEFINED

#include <ostream>
#include <cassert>

namespace gp {

    
template< typename Pop , typename Fitness >
void write_population_statistics( std::ostream &out , const Pop& pop , const Fitness &fitness )
{
    assert( pop.size() > 0 );
    
    size_t n = pop.size();
    
    size_t height_mean = 0 , height_sq_mean = 0 , nodes_mean = 0 , nodes_sq_mean = 0;
    for( size_t i=0 ; i<n ; ++i )
    {
        height_mean += pop[i].height();
        height_sq_mean += pop[i].height() * pop[i].height();
        nodes_mean += pop[i].num_elements();
        nodes_sq_mean += pop[i].num_elements() * pop[i].num_elements();
    }
    double height_mean_ = double( height_mean ) / double( n );
    double height_sq_mean_ = double( height_sq_mean ) / double( n );
    double nodes_mean_ = double( nodes_mean ) / double( n );
    double nodes_sq_mean_ = double( nodes_sq_mean ) / double( n );
    out << "height mean = " << height_mean_
        << ", height stddev = " << std::sqrt( height_sq_mean_ - height_mean_ * height_mean_ )
        << ", nodes mean = " << nodes_mean_
        << ", nodes stddev = " << std::sqrt( nodes_sq_mean_ - nodes_mean_ * nodes_mean_ );
}

template< typename Pop , typename Fitness >
struct population_statistics_writer
{
    Pop const& m_pop;
    Fitness const& m_fitness;
    population_statistics_writer( Pop const& pop , Fitness const& fitness )
    : m_pop( pop ) , m_fitness( fitness ) { }
    
    std::ostream& operator()( std::ostream &out ) const
    {
        write_population_statistics( out , m_pop , m_fitness );
        return out;
    }
};

template< class Pop , class Fitness >
std::ostream& operator<<( std::ostream &out , population_statistics_writer< Pop , Fitness > const& b )
{
    return b( out );
}

template< class Pop , class Fitness >
population_statistics_writer< Pop , Fitness > population_statistics( Pop const& pop , Fitness const& fitness )
{
    return population_statistics_writer< Pop , Fitness >( pop , fitness );
}





} // namespace gp


#endif // GP_STAT_POPULATION_STATISTICS_HPP_DEFINED
