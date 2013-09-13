/*
 * gpcxx/tree/detail/node_helpers.hpp
 * Date: 2013-09-14
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_TREE_DETAIL_NODE_HELPERS_HPP_INCLUDED
#define GPCXX_TREE_DETAIL_NODE_HELPERS_HPP_INCLUDED

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>

#include <type_traits>



namespace gpcxx {
namespace detail {


template< typename Node >
struct node_value_getter : public boost::mpl::eval_if<
    std::is_const< Node > ,
    std::add_const< typename Node::value_type > ,
    boost::mpl::identity< typename Node::value_type >
  >
{
};


template< typename Node >
struct node_base_getter : public boost::mpl::eval_if<
    std::is_const< Node > ,
    std::add_const< typename Node::node_base_type > ,
    boost::mpl::identity< typename Node::node_base_type >
    >
{
};


} // namespace detail
} // namespace gpcxx



#endif // GPCXX_TREE_DETAIL_NODE_HELPERS_HPP_INCLUDED
