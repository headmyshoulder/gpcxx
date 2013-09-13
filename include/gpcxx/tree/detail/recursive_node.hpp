/*
 * gpcxx/tree/detail/recursive_node.hpp
 * Date: 2013-09-13
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_TREE_DETAIL_RECURSIVE_NODE_HPP_INCLUDED
#define GPCXX_TREE_DETAIL_RECURSIVE_NODE_HPP_INCLUDED

#include <boost/variant.hpp>

#include <array>

namespace gpcxx {
namespace detail {

    
template< typename T > struct recursive_node;
struct nil {};



template< typename T >
struct recursive_node_base
{
    typedef T value_type;
    typedef boost::variant<
        nil 
      , boost::recursive_wrapper< recursive_node< value_type > >
        > type;

    type m_node = { nil() };
};



template< typename T >
struct recursive_node
{
    typedef T value_type;
    
    value_type m_value = {};
    std::array< recursive_node_base< value_type > , 2 > m_children = { nil() , nil() };
    recursive_node_base< value_type > *m_parent = { nullptr };
};




} // namespace detail
} // namespace gpcxx


#endif // GPCXX_TREE_DETAIL_RECURSIVE_NODE_HPP_INCLUDED
