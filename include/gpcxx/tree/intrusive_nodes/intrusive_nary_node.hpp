/*
 * gpcxx/tree/intrusive_nodes/intrusive_nary_node.hpp
 * Date: 2015-05-18
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_TREE_INTRUSIVE_NODES_INTRUSIVE_NARY_NODE_HPP_INCLUDED
#define GPCXX_TREE_INTRUSIVE_NODES_INTRUSIVE_NARY_NODE_HPP_INCLUDED


#include <gpcxx/tree/detail/node_base.hpp>


#include <array>
#include <algorithm>
#include <cassert>

    
namespace gpcxx {
    
namespace detail {

template< typename Node > class tree_base_cursor;

}



template< typename Node , size_t MaxArity = 2 >
class intrusive_nary_node : public detail::node_base< detail::descending_array_node< MaxArity > > 
{
    template< typename N > friend class detail::tree_base_cursor;
   
public:
    
    using node_type = Node;
    using node_base_type = detail::node_base< detail::descending_array_node< MaxArity > >;
    using node_pointer = node_type*;
    using const_node_pointer = node_type const*;
    using value_type = node_type;
    using reference = value_type&;
    using const_reference = value_type const&;
    
    intrusive_nary_node( node_type *parent = nullptr ) noexcept
    : node_base_type( parent )
    {
    }
    
    intrusive_nary_node( intrusive_nary_node const& )
    : node_base_type()
    {
    }
    
    intrusive_nary_node( intrusive_nary_node && )
    : node_base_type()
    {
    }
    
    intrusive_nary_node& operator=( intrusive_nary_node const& )
    {
        return *this;
    }
    
    intrusive_nary_node& operator=( intrusive_nary_node && )
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
    
private:
    
    reference get( void ) noexcept { return *static_cast< Node* >( this ); }
    
    const_reference get( void ) const noexcept { return *static_cast< Node const* >( this ); }
    
    
};


} // namespace gpcxx



#endif // GPCXX_TREE_INTRUSIVE_NODES_INTRUSIVE_NARY_NODE_HPP_INCLUDED
