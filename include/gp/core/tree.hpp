/*
 * traits.hpp
 * Date: 2013-02-03
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef TRAITS_HPP_INCLUDED
#define TRAITS_HPP_INCLUDED


namespace gp {


    template< class Tree , class Enable = void > struct root_node_impl;

    template< class Tree >
    typename root_node_impl< Tree >::type root_node( Tree &tree )
    {
        return root_node_impl< Tree >::apply( tree );
    }

} // namespace gp

#endif // TRAITS_HPP_INCLUDED
