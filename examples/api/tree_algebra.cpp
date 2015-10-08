/*
 * tree_algebra.cpp
 * Date: 2015-10-08
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/tree.hpp>
#include <gpcxx/canonic.hpp>

#include <gpcxx/util.hpp>

#include <iostream>

int main( int argc , char** argv )
{
    //[ algebraic_node
    const static size_t dim = 1;
    using result_type = double;
    using context_type = std::array< double , dim >;
    using node_type = gpcxx::algebraic_node< result_type , context_type >;
    using tree_type = gpcxx::intrusive_tree< node_type >;
    //]
    
    tree_type t;
    
    GPCXX_UNUSED_VAR( t );
    
    return 0;
}

