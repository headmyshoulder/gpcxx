/*
  gpcxx/tree/detail/basic_node.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GPCXX_TREE_DETAIL_BASIC_NODE_HPP_DEFINED
#define GPCXX_TREE_DETAIL_BASIC_NODE_HPP_DEFINED

#include <utility>

namespace gpcxx {
namespace detail {


template< typename Node > class tree_base_cursor;



template< typename T , typename NodeBase  >
class basic_node : public NodeBase
{
    template< typename N > friend class tree_base_cursor;
    
public:
    
    using value_type = T;
    using reference = value_type&;
    using const_reference = value_type const&;
    
    using node_type = basic_node< value_type , NodeBase >;
    using node_base_type = NodeBase ;
    using node_pointer = node_type*;
    using node_reference = node_type&;
    


    reference operator*() { return m_data; } 

    const_reference operator*() const { return m_data; } 
    
    basic_node( value_type data , node_base_type *parent = nullptr ) : node_base_type( parent ) ,  m_data( std::move( data ) ) {}
    
    
 
private:
    
    reference get( void ) noexcept { return m_data; }
    
    const_reference get( void ) const noexcept { return m_data; }

    value_type m_data;
};



} // namespace detail
} // namespace gpcxx


#endif // GPCXX_TREE_DETAIL_BASIC_NODE_HPP_DEFINED
