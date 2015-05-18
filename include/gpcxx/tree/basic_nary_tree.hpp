/*
 * gpcxx/tree/basic_nary_tree.hpp
 * Date: 2015-05-18
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_TREE_BASIC_NARY_TREE_HPP_INCLUDED
#define GPCXX_TREE_BASIC_NARY_TREE_HPP_INCLUDED

#include <gpcxx/tree/detail/tree_base.hpp>
#include <gpcxx/tree/detail/basic_node.hpp>
#include <gpcxx/tree/detail/node_base.hpp>


namespace gpcxx {


template< typename T , size_t MaxArity = 2 , typename Allocator = std::allocator< T > >
using basic_nary_tree = detail::tree_base< detail::basic_node< T , detail::node_base< detail::descending_array_node< MaxArity > > > , Allocator >;
    

} // namespace gpcxx


#endif // GPCXX_TREE_BASIC_NARY_TREE_HPP_INCLUDED
