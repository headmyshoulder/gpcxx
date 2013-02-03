/*
 * node_traits.hpp
 * Date: 2013-02-03
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef NODE_TRAITS_HPP_INCLUDED
#define NODE_TRAITS_HPP_INCLUDED

#include <gp/util/pointer_types.hpp>

#include <boost/utility/enable_if.hpp>

namespace gp {
namespace node_traits {

    template< class Node , class Enabler = void >
    struct child
    {
        typedef typename Node::child_type type;
    };

    template< class Node >
    struct child< Node , typename boost::enable_if< typename is_pointer_type< Node >::type >::type >
    {
        typedef typename unwrap_pointer< Node >::type node_type;
        typedef typename child< node_type >::type type;
    };



    template< class Node , class Enabler = void >
    struct parent
    {
        typedef typename Node::parent_type type;
    };

    template< class Node >
    struct parent< Node , typename boost::enable_if< typename is_pointer_type< Node >::type >::type >
    {
        typedef typename unwrap_pointer< Node >::type node_type;
        typedef typename parent< node_type >::type type;
    };

        


    

} // node_traits
} // namespace gp

#endif // NODE_TRAITS_HPP_INCLUDED
