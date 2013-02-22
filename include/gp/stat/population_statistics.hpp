/*
 * population_statistics.hpp
 * Date: 2013-02-22
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef POPULATION_STATISTICS_HPP_INCLUDED
#define POPULATION_STATISTICS_HPP_INCLUDED

#include <gp/util/sort_indices.hpp>
#include <gp/io/simple.hpp>

#include <ostream>
#include <vector>

namespace gp {


template< class Pop , class Fitness >
void report_population( const Pop& p , const Fitness &f , std::ostream &out )
{
    std::vector< size_t > idx;
    auto iter = gp::sort_indices( f , idx );
    for( size_t i=0 ; i<10 ; ++i )
    {
        out << i << " " << f[ idx[i] ] << " : ";
        print_simple( p[ idx[i] ] , out );
        out << endl;
    }
}

} // namespace gp

#endif // POPULATION_STATISTICS_HPP_INCLUDED
