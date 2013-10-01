/*
  gpcxx/stat/population_statistics.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GPCXX_STAT_POPULATION_STATISTICS_HPP_DEFINED
#define GPCXX_STAT_POPULATION_STATISTICS_HPP_DEFINED

#include <ostream>
#include <cassert>
#include <cmath>

namespace gpcxx {
    
struct population_statistics
{
    double height_mean , height_stddev , nodes_mean , nodes_stddev , number_nodes;
};

template< typename Pop >
population_statistics calc_population_statistics( Pop const& pop )
{
    assert( pop.size() > 0 );
    
    size_t n = pop.size();
    
    size_t height_mean = 0 , height_sq_mean = 0 , nodes_mean = 0 , nodes_sq_mean = 0;
    for( size_t i=0 ; i<n ; ++i )
    {
        size_t height = pop[i].root().height();
        size_t nodes = pop[i].size();
        height_mean += height;
        height_sq_mean += height * height ;
        nodes_mean += nodes;
        nodes_sq_mean += nodes * nodes ;
    }
    population_statistics stat;
    stat.number_nodes = nodes_mean;
    stat.height_mean = double( height_mean ) / double( n );
    stat.height_stddev = std::sqrt( double( height_sq_mean ) / double( n ) - stat.height_mean * stat.height_mean );
    stat.nodes_mean = double( nodes_mean ) / double( n );
    stat.number_nodes = nodes_mean;
    stat.nodes_stddev = std::sqrt( double( nodes_sq_mean ) / double( n ) - stat.nodes_mean * stat.nodes_mean );
    return stat;
};

    

inline std::ostream& operator<<( std::ostream &out , population_statistics const& stat )
{
    out << "height mean = " << stat.height_mean
        << ", height stddev = " << stat.height_stddev
        << ", nodes mean = " << stat.nodes_mean
        << ", nodes stddev = " << stat.nodes_stddev
        << ", number of nodes = " << stat.number_nodes;
    return out;
}





} // namespace gpcxx


#endif // GPCXX_STAT_POPULATION_STATISTICS_HPP_DEFINED
