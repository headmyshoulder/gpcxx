/*
 * polish.hpp
 * Date: 2013-02-03
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef POLISH_HPP_INCLUDED
#define POLISH_HPP_INCLUDED

#include <gp/core/tree_node.hpp>

#include <ostream>
#include <iostream>

namespace gp {

    template< class Tree >
    void print_simple( const Tree &tree , std::ostream& = std::cout )
    {
        auto n = root_node( tree );
    }

}


#endif // POLISH_HPP_INCLUDED
