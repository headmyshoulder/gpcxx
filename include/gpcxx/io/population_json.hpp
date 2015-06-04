/*
 * gpcxx/performance/benchmarks/write_population_json.hpp
 * Date: 2015-06-03
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_PERFORMANCE_BENCHMARKS_WRITE_POPULATION_JSON_HPP_INCLUDED
#define GPCXX_PERFORMANCE_BENCHMARKS_WRITE_POPULATION_JSON_HPP_INCLUDED

#include <gpcxx/util/indent.hpp>
#include <gpcxx/io/json.hpp>
#include <gpcxx/io/polish.hpp>

#include <ostream>
#include <cassert>

namespace gpcxx {
    
template< typename Population , typename Fitness >
void write_population_json( std::ostream& out , Population const& pop , Fitness const& fit , size_t indent_i , std::string const& newline )
{
    assert( pop.size() == fit.size() );
    
    out << indent( indent_i ) << "[" << newline;
    for( size_t i=0 ; i<pop.size() ; ++i )
    {
        out << indent( indent_i + 1 ) << "{" << newline;
        out << indent( indent_i + 2 ) << "\"tree\" : " << newline << gpcxx::json( pop[i] , newline , "  " , indent_i + 2 ) << " , " << newline;
        out << indent( indent_i + 2 ) << "\"polish\" : \"" << gpcxx::polish( pop[i] , "|" ) << "\" ," << newline;
        out << indent( indent_i + 2 ) << "\"fitness\" : " << fit[i] << newline;
        out << indent( indent_i + 1 ) << "}";
        if( i != ( pop.size() -1 ) ) out << " , ";
        out << newline;
    }
    out << indent( indent_i ) << "]";
}

namespace detail {
    
template< typename Pop , typename Fitness >
struct population_json_writer
{
    Pop const& m_pop;
    Fitness const& m_fitness;
    size_t m_indent_i;
    std::string m_newline;
    population_json_writer( Pop const& pop , Fitness const& fitness , size_t indent_i , std::string const& newline )
    : m_pop( pop ) , m_fitness( fitness ) , m_indent_i( indent_i ) , m_newline( newline ) { }
    
    std::ostream& operator()( std::ostream &out ) const
    {
        write_population_json( out , m_pop , m_fitness , m_indent_i , m_newline );
        return out;
    }
};

template< typename Pop , typename Fitness >
std::ostream& operator<<( std::ostream &out , population_json_writer< Pop , Fitness > const& b )
{
    return b( out );
}

} // namespace detail

template< typename Pop , typename Fitness >
detail::population_json_writer< Pop , Fitness > population_json( Pop const& pop , Fitness const& fitness , size_t indent_i = 0 , std::string const& newline = "\n" )
{
    return detail::population_json_writer< Pop , Fitness >( pop , fitness , indent_i , newline );
}

} // namespace gpcxx


#endif // GPCXX_PERFORMANCE_BENCHMARKS_WRITE_POPULATION_JSON_HPP_INCLUDED
