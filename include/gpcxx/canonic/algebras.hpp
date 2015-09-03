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
    }
    
    void add_associative_operation( node_type n )
    {
    }
    
    void add_commutative_magma( node_type n )
    {
    }
    
    template< typename Cursor >
    bool is_commutative( Cursor cursor ) const
    {
        auto pred = [cursor]( auto const& n ) { return ( cursor->name() ) == n; };
        return std::find_if( m_abelian_magmas.begin() , m_abelian_magmas.end() , pred ) != m_abelian_magmas.end();
    }
    
    template< typename Cursor >
    bool is_associative( Cursor cursor ) const
    {
        auto pred = [cursor]( auto const& n ) { return cursor->name() == n; };
        return std::find_if( m_associative_operations.begin() , m_associative_operations.end() , pred ) != m_associative_operations.end();
    }
    
    template< typename Cursor >
    optional_group get_group( Cursor cursor ) const
    {
        auto pred = [cursor]( auto const& n ) { return cursor->name() == n.first; };
        auto iter = std::find_if( m_groups.begin() , m_groups.end() , pred );
        if( iter == m_groups.end() ) return boost::none_t {} ;
        else return optional_group { iter->second };
    }
    
    template< typename Cursor >
    optional_group  get_group_from_inverse_operation( Cursor cursor ) const
    {
        auto pred = [cursor]( auto const& n ) { return cursor->name() == n.second.inverse_operation(); };
        auto iter = std::find_if( m_groups.begin() , m_groups.end() , pred );
        if( iter == m_groups.end() ) return boost::none_t {} ;
        else return optional_group { iter->second };
    }
    
    template< typename Cursor >
    optional_group  get_group_from_inverse_transformation( Cursor cursor ) const
    {
        auto pred = [cursor]( auto const& n ) { return cursor->name() == n.second.inverse_transformation(); };
        auto iter = std::find_if( m_groups.begin() , m_groups.end() , pred );
        if( iter == m_groups.end() ) return boost::none_t {} ;
        else return optional_group { iter->second };
    }

private:

    std::vector< node_type > m_abelian_magmas;

    // TODO: find a better name, the real algebra name
    std::vector< node_type > m_associative_operations;

    std::vector< group_type > m_groups;
};



} // namespace gpcxx


#endif // GPCXX_CANONIC_ALGEBRAS_HPP_INCLUDED
