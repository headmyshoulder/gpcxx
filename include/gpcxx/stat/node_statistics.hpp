/*
 * gpcxx/stat/node_statistics.hpp
 * Date: 2013-09-30
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_STAT_NODE_STATISTICS_HPP_INCLUDED
#define GPCXX_STAT_NODE_STATISTICS_HPP_INCLUDED

#include <gpcxx/util/assert.hpp>

#include <ostream>




namespace gpcxx {

struct node_statistics
{
    size_t num_nodes = 0;
    size_t num_terminals = 0;
    size_t num_unaries = 0;
    size_t num_binaries = 0;
};

namespace detail
{
    template< typename NodeCursor >
    void update_statistics( NodeCursor const &n , node_statistics &stat )
    {
        stat.num_nodes++;
        if( n.size() == 0 ) stat.num_terminals++;
        else if( n.size() == 1 ) stat.num_unaries++;
        else if( n.size() == 2 ) stat.num_binaries++;
        else GPCXX_ASSERT( false );
        for( size_t i=0 ; i<n.size() ; ++i ) update_statistics( n.children(i) , stat );
    }
}

template< typename Tree >
node_statistics calc_node_statistics_tree( Tree const& tree )
{
    node_statistics stat;
    detail::update_statistics( tree.root() , stat );
    return stat;
}

template< typename Pop >
node_statistics calc_node_statistics( Pop const& pop )
{
    GPCXX_ASSERT( pop.size() > 0 );

    size_t n = pop.size();

    node_statistics stat;
    for( size_t i=0 ; i<n ; ++i )
    {
        node_statistics tmp = calc_node_statistics_tree( pop[i] );
        stat.num_nodes += tmp.num_nodes;
        stat.num_terminals += tmp.num_terminals;
        stat.num_unaries += tmp.num_unaries;
        stat.num_binaries += tmp.num_binaries;
    }
    return stat;
}



inline std::ostream& operator<<( std::ostream &out , node_statistics const& stat )
{
    out << "num_nodes = " << stat.num_nodes
        << ", terminals = " << stat.num_terminals
        << ", unaries = " << stat.num_unaries
        << ", binaries = " << stat.num_binaries;
    return out;
}


} // namespace gpcxx


#endif // GPCXX_STAT_NODE_STATISTICS_HPP_INCLUDED
