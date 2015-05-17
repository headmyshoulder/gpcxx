/*
  gpcxx/tree/detail/inspect_node_base.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GPCXX_TREE_DETAIL_INSPECT_NODE_BASE_HPP_DEFINED
#define GPCXX_TREE_DETAIL_INSPECT_NODE_BASE_HPP_DEFINED

#include <gpcxx/tree/detail/node_base.hpp>
#include <gpcxx/util/indent.hpp>

namespace gpcxx {
namespace detail {
    
    


template< typename DescendingNode >
void inspect_node_base( std::ostream &out , node_base< DescendingNode > *ptr , size_t ind )
{
    out << indent( ind , "  " ) << std::string( "+-" ) << ptr << "\n";
    if( ptr != nullptr )
    {
        for( size_t i=0 ; i<ptr->size() ; ++i )
            inspect_node_base( out , ptr->child_node( i ) , ind + 1 );
    }
}


} // namespace detail
} // namespace gpcxx


#endif // GPCXX_TREE_DETAIL_INSPECT_NODE_BASE_HPP_DEFINED
