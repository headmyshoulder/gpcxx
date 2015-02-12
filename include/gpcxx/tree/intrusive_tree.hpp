/*
  gpcxx/tree/intrusive_tree.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GPCXX_TREE_INTRUSIVE_TREE_HPP_DEFINED
#define GPCXX_TREE_INTRUSIVE_TREE_HPP_DEFINED

#include <gpcxx/tree/detail/tree_base.hpp>


namespace gpcxx {
    
template< typename Node , typename Allocator = std::allocator< Node > >
using intrusive_tree = detail::tree_base< Node , Allocator >;


} // namespace gpcxx

#endif // GPCXX_TREE_INTRUSIVE_TREE_HPP_DEFINED
