/*
 * gpcxx/tree/intrusive_node.hpp
 * Date: 2013-10-14
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_TREE_INTRUSIVE_NODE_HPP_INCLUDED
#define GPCXX_TREE_INTRUSIVE_NODE_HPP_INCLUDED

#include <gpcxx/tree/detail/node_base.hpp>

#include <array>
#include <algorithm>
#include <cassert>

    
namespace gpcxx {

template< typename Node >
class intrusive_tree;



template< typename Node , size_t MaxArity = 2 >
class intrusive_node : public detail::node_base< MaxArity >
{
    template< typename T >
    friend class intrusive_tree;
   
public:
    
    using node_type = Node;
    using node_base_type = detail::node_base< MaxArity >;
    using node_pointer = node_type*;
    using const_node_pointer = node_type const*;
    
    intrusive_node( node_type *parent = nullptr ) noexcept
    : node_base_type( parent )
    {
    }
    
    intrusive_node( intrusive_node const& ) = default;
    intrusive_node( intrusive_node && ) = default;
    
    intrusive_node& operator=( intrusive_node const& )
    {
        return *this;
    }
    
    intrusive_node& operator=( intrusive_node && )
    {
        return *this;
    }

    node_type& child( size_t i ) noexcept
    {
        return static_cast< node_type& >( *( this->child_node(i) ) );
    }
    
    node_type const& child( size_t i ) const noexcept
    {
        return static_cast< node_type const& >( *( this->child_node(i) ) );
    }
    
    void clear_children( void )
    {
        std::fill( this->m_children.begin() , this->m_children.end() , nullptr );
    }
};


} // namespace gpcxx





#endif // GPCXX_TREE_INTRUSIVE_NODE_HPP_INCLUDED
