/*
 * gpcxx/tree/basic_named_intrusive_node.hpp
 * Date: 2013-10-15
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_TREE_BASIC_NAMED_INTRUSIVE_NODE_HPP_INCLUDED
#define GPCXX_TREE_BASIC_NAMED_INTRUSIVE_NODE_HPP_INCLUDED

#include <gpcxx/tree/intrusive_node.hpp>




namespace gpcxx {


template< typename Res , typename Context >
class basic_named_intrusive_node : public gpcxx::intrusive_node< basic_named_intrusive_node< Res , Context > >
{
public:
    
    typedef Res result_type;
    typedef Context context_type;
    typedef basic_named_intrusive_node< result_type , context_type > node_type;
    
    typedef std::function< result_type( context_type const& , node_type const& ) > func_type;
    
    basic_named_intrusive_node( func_type f , std::string name )
    : m_func( std::move( f ) ) , m_name( std::move( name ) ) { }
    
    result_type eval( context_type const & context ) const
    {
        return m_func( context , *this );
    }
    
    std::string const& name( void ) const { return m_name; }
    
private:
    
    func_type m_func;
    std::string m_name;
};


} // namespace gpcxx


#endif // GPCXX_TREE_BASIC_NAMED_INTRUSIVE_NODE_HPP_INCLUDED
