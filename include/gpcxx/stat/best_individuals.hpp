/*
 * best_individuals.hpp
 * Date: 2013-02-22
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef GPCXX_STAT_BEST_INDIVIDUALS_HPP_INCLUDED
#define GPCXX_STAT_BEST_INDIVIDUALS_HPP_INCLUDED

#include <gpcxx/util/sort_indices.hpp>
#include <gpcxx/util/indent.hpp>
#include <gpcxx/io/simple.hpp>

#include <ostream>
#include <vector>

namespace gpcxx {


template< class Pop , class Fitness >
void write_best_individuals( std::ostream &out , const Pop& p , const Fitness &f , size_t ind , size_t num_individuals )
{
    std::vector< size_t > idx;
    auto iter = gpcxx::sort_indices( f , idx );
    bool first = true;
    for( size_t i=0 ; i<num_individuals ; ++i )
    {
        if( first ) first = false; else out << "\n";
        out << indent( ind ) << i << " " << f[ idx[i] ] << " : " << simple( p[ idx[i] ] );
    }
}

template< class Pop , class Fitness >
struct best_individuals_writer
{
    Pop const& m_pop;
    Fitness const& m_fitness;
    size_t m_indent;
    size_t m_num_individuals;
    best_individuals_writer( Pop const& pop , Fitness const& fitness , size_t indent , size_t num_individuals )
    : m_pop( pop ) , m_fitness( fitness ) , m_indent( indent ) , m_num_individuals( num_individuals ) { }
    
    std::ostream& operator()( std::ostream &out ) const
    {
        write_best_individuals( out , m_pop , m_fitness , m_indent , m_num_individuals );
        return out;
    }
};

template< class Pop , class Fitness >
std::ostream& operator<<( std::ostream &out , best_individuals_writer< Pop , Fitness > const& b )
{
    return b( out );
}

template< class Pop , class Fitness >
best_individuals_writer< Pop , Fitness > best_individuals( Pop const& pop , Fitness const& fitness , size_t indent = 0 , size_t num_individuals = 10 )
{
    return best_individuals_writer< Pop , Fitness >( pop , fitness , indent , num_individuals );
}

} // namespace gpcxx

#endif // GPCXX_STAT_BEST_INDIVIDUALS_HPP_INCLUDED
