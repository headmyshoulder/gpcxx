/*
  gpcxx/tree/basic_tree.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GPCXX_TREE_BASIC_TREE_HPP_DEFINED
#define GPCXX_TREE_BASIC_TREE_HPP_DEFINED

#include <gpcxx/tree/detail/tree_base.hpp>
#include <gpcxx/tree/detail/basic_node.hpp>
#include <gpcxx/tree/detail/node_base.hpp>



namespace gpcxx {

template< typename T , size_t MaxArity = 2 , typename Allocator = std::allocator< T > >
using basic_tree = detail::tree_base< detail::basic_node< T , detail::node_base< detail::descending_array_node< MaxArity > > > , Allocator >;

template< typename T , typename Allocator = std::allocator< T > >
using basic_growing_tree = detail::tree_base< detail::basic_node< T , detail::node_base< detail::descending_vector_node< typename Allocator::template rebind< void* >::other > > > , Allocator >;



} // namespace gpcxx






#endif // GPCXX_TREE_BASIC_TREE_HPP_DEFINED
