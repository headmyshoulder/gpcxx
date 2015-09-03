/*
 * gpcxx/canonic/algebras.hpp
 * Date: 2015-09-02
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_CANONIC_ALGEBRAS_HPP_INCLUDED
#define GPCXX_CANONIC_ALGEBRAS_HPP_INCLUDED

#include <gpcxx/canonic/group.hpp>

#include <boost/optional.hpp>

#include <algorithm>
#include <vector>

namespace gpcxx {

template< typename Node >
class algebras
{
public:
    
    using node_type = Node;
    using group_type = group< node_type >;
    using optional_group = boost::optional< group_type >;
    
    void add_group( group_type g )
    {
        add_associative_operation( g.operation() );
        if( ! get_group( g.operation() ) )
            m_groups.emplace_back( g );
    }
    
    void add_abelian_group( group_type g )
    {
        add_group( g );
        add_abelian_magma( g.operation() );
    }
    
    void add_associative_operation( node_type n )
    {
        if( ! is_associative( n ) )
            m_associative_operations.emplace_back( n );
    }
    
    void add_abelian_magma( node_type n )
    {
        if( ! is_commutative( n ) )
            m_abelian_magmas.emplace_back( n );
    }
    
    bool is_commutative( node_type const& node ) const
    {
        return get_commutative_iter( node ) != m_abelian_magmas.end();
    }
    
    bool is_associative( node_type const& node ) const
    {
        return get_associative_iter( node )  != m_associative_operations.end();
    }
    
    optional_group get_group( node_type const& node ) const
    {
        auto iter = get_group_iter( node );
        if( iter == m_groups.end() ) return boost::none_t {} ;
        else return optional_group { *iter };
    }
    
    optional_group  get_group_from_inverse_operation( node_type const& node ) const
    {
        auto iter = get_group_from_inverse_operation_iter( node );
        if( iter == m_groups.end() ) return boost::none_t {} ;
        else return optional_group { *iter };
    }
    
    optional_group  get_group_from_inverse_function( node_type const& node ) const
    {
        auto iter = get_group_from_inverse_function_iter( node );
        if( iter == m_groups.end() ) return boost::none_t {} ;
        else return optional_group { *iter };
    }

private:
    
    auto get_commutative_iter( node_type const& node ) const
    {
        auto pred = [&node]( auto const& n ) { return node == n; };
        return std::find_if( m_abelian_magmas.begin() , m_abelian_magmas.end() , pred );
    }

    auto get_associative_iter( node_type const& node ) const
    {
        auto pred = [&node]( auto const& n ) { return node == n; };
        return std::find_if( m_associative_operations.begin() , m_associative_operations.end() , pred );
    }

    auto get_group_iter( node_type const& node ) const
    {
        auto pred = [&node]( auto const& n ) { return node == n.operation(); };
        return std::find_if( m_groups.begin() , m_groups.end() , pred );
    }

    auto get_group_from_inverse_operation_iter( node_type const& node ) const
    {
        auto pred = [&node]( auto const& n ) { return node == n.inverse_operation(); };
        return std::find_if( m_groups.begin() , m_groups.end() , pred );
    }

    auto get_group_from_inverse_function_iter( node_type const& node ) const
    {
        auto pred = [&node]( auto const& n ) { return node == n.inverse_function(); };
        return std::find_if( m_groups.begin() , m_groups.end() , pred );
    }


    std::vector< node_type > m_abelian_magmas;

    // TODO: find a better name, the real algebra name
    std::vector< node_type > m_associative_operations;

    std::vector< group_type > m_groups;
};



} // namespace gpcxx


#endif // GPCXX_CANONIC_ALGEBRAS_HPP_INCLUDED
