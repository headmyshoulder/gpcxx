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

#include <ostream>
#include <cassert>

namespace gpcxx {
    
template< typename Population , typename Fitness >
void write_population_json( std::ostream& out , Population const& pop , Fitness const& fit , size_t indent_i = 0 , std::string const& newline = "\n" )
{
    assert( pop.size() == fit.size() );
    
    out << indent( indent_i ) << "[" << newline;
    for( size_t i=0 ; i<pop.size() ; ++i )
    {
        out << indent( indent_i );
    }
    out << indent( indent_i ) << "]";
}


} // namespace gpcxx


#endif // GPCXX_PERFORMANCE_BENCHMARKS_WRITE_POPULATION_JSON_HPP_INCLUDED
