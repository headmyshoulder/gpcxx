/*
 * gpcxx/io/best_individuals.hpp
 * Date: 2013-02-22
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef GPCXX_IO_BEST_INDIVIDUALS_HPP_INCLUDED
#define GPCXX_IO_BEST_INDIVIDUALS_HPP_INCLUDED

#include <gpcxx/util/sort_indices.hpp>
#include <gpcxx/util/indent.hpp>
#include <gpcxx/io/simple.hpp>

#include <ostream>
#include <vector>

namespace gpcxx {


template< typename Pop , typename Fitness , typename SymbolMapper >
void write_best_individuals( std::ostream &out , const Pop& p , const Fitness &f , size_t ind , size_t num_individuals , bool write_infix , SymbolMapper const& mapper )
{
    std::vector< size_t > idx;
    auto iter = gpcxx::sort_indices( f , idx );
    bool first = true;
    for( size_t i=0 ; i<num_individuals ; ++i )
    {
        if( first ) first = false; else out << "\n";
        out << indent( ind ) << i << " " << f[ idx[i] ] << " : " << simple( p[ idx[i] ] , write_infix , mapper );
    }
}

template< typename Pop , typename Fitness , typename SymbolMapper >
struct best_individuals_writer
{
    Pop const& m_pop;
    Fitness const& m_fitness;
    size_t m_indent;
    size_t m_num_individuals;
    bool m_write_infix;
    SymbolMapper const& m_mapper;
    best_individuals_writer( Pop const& pop , Fitness const& fitness , size_t indent , size_t num_individuals , bool write_infix , SymbolMapper const& mapper )
    : m_pop( pop ) , m_fitness( fitness ) , m_indent( indent ) , m_num_individuals( num_individuals ) , m_write_infix( write_infix ) , m_mapper( mapper ) { }
    
    std::ostream& operator()( std::ostream &out ) const
    {
        write_best_individuals( out , m_pop , m_fitness , m_indent , m_num_individuals , m_write_infix , m_mapper );
        return out;
    }
};

template< typename Pop , typename Fitness , typename SymbolMapper >
std::ostream& operator<<( std::ostream &out , best_individuals_writer< Pop , Fitness , SymbolMapper > const& b )
{
    return b( out );
}

template< typename Pop , typename Fitness , typename SymbolMapper = gpcxx::identity >
best_individuals_writer< Pop , Fitness , SymbolMapper > best_individuals( Pop const& pop , Fitness const& fitness , size_t indent = 0 , size_t num_individuals = 10 , bool write_infix = true , SymbolMapper const &mapper = SymbolMapper() )
{
    return best_individuals_writer< Pop , Fitness , SymbolMapper >( pop , fitness , indent , num_individuals , write_infix , mapper );
}

} // namespace gpcxx

#endif // GPCXX_STAT_BEST_INDIVIDUALS_HPP_INCLUDED
