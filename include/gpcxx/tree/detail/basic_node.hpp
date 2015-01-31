/*
  gpcxx/tree/detail/basic_node.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GPCXX_TREE_DETAIL_BASIC_NODE_HPP_DEFINED
#define GPCXX_TREE_DETAIL_BASIC_NODE_HPP_DEFINED

#include <gpcxx/tree/detail/node_base.hpp>

namespace gpcxx {
namespace detail {



template< typename T , size_t MaxArity >
class basic_node : public node_base< MaxArity >
{
public:
    
    typedef T value_type;
    typedef basic_node< value_type , MaxArity > node_type;
    typedef node_base< MaxArity > node_base_type;
    typedef node_type* node_pointer;
    typedef node_type& node_reference;
    
    typedef value_type& reference;
    typedef value_type const& const_reference;
    typedef value_type* pointer;

    reference operator*() { return m_data; } 

    const_reference operator*() const { return m_data; } 
    
    basic_node( value_type data , node_base_type *parent = nullptr ) : node_base_type( parent ) ,  m_data( std::move( data ) ) {}
    
    
 
private:

    value_type m_data;
};



} // namespace detail
} // namespace gpcxx


#endif // GPCXX_TREE_DETAIL_BASIC_NODE_HPP_DEFINED
