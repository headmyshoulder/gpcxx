/*
 * gpcxx/canonic/group.hpp
 * Date: 2015-09-02
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_CANONIC_GROUP_HPP_INCLUDED
#define GPCXX_CANONIC_GROUP_HPP_INCLUDED

#include <gpcxx/util/exception.hpp>

#include <utility>


namespace gpcxx {

template< typename Node >
class group
{
public:
    
    using node_type = Node;
    
    group( node_type operation , node_type identity , node_type inverse_operation , node_type inverse_function )
    : m_operation { std::move( operation ) }
    , m_identity { std::move( identity ) }
    , m_inverse_operation { std::move( inverse_operation ) }
    , m_inverse_function { std::move( inverse_function ) }
    {
        if( m_operation.count_nodes() > 1 ) throw gpcxx_exception( "Group operation node contains children. It cannot be used as a prototyp." );
        if( m_identity.count_nodes() > 1 ) throw gpcxx_exception( "Group identity operation node contains children. It cannot be used as a prototyp." );
        if( m_inverse_operation.count_nodes() > 1 ) throw gpcxx_exception( "Group inverse operation node contains children. It cannot be used as a prototyp." );
        if( m_inverse_function.count_nodes() > 1 ) throw gpcxx_exception( "Group inverse function node contains children. It cannot be used as a prototyp." );
    }
    
    node_type const& operation( void ) const { return m_operation; }
    
    node_type const& identity( void ) const { return m_identity; }
    
    node_type const& inverse_operation( void ) const { return m_inverse_operation; }
    
    node_type const& inverse_function( void ) const { return m_inverse_function; }
    
private:
    
    node_type m_operation;
    node_type m_identity;    
    node_type m_inverse_operation;    
    node_type m_inverse_function;
};

    


} // namespace gpcxx


#endif // GPCXX_CANONIC_GROUP_HPP_INCLUDED
