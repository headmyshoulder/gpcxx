/*
 * gpcxx/tree/intrusive_nodes/intrusive_nary_named_func_node.hpp
 * Date: 2015-05-18
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_TREE_INTRUSIVE_NODES_INTRUSIVE_NARY_NAMED_FUNC_NODE_HPP_INCLUDED
#define GPCXX_TREE_INTRUSIVE_NODES_INTRUSIVE_NARY_NAMED_FUNC_NODE_HPP_INCLUDED

#include <gpcxx/tree/intrusive_nodes/intrusive_nary_node.hpp>

#include <functional>
#include <string>


namespace gpcxx {

template< typename Res , typename Context, size_t Arity = 2 >
class intrusive_nary_named_func_node : public gpcxx::intrusive_nary_node< intrusive_nary_named_func_node< Res , Context , Arity > , Arity >
{
public:
    
    using result_type = Res;
    using context_type = Context;
    using node_type = intrusive_nary_named_func_node< result_type , context_type , Arity >;
    
    typedef std::function< result_type( context_type& , node_type const& ) > func_type;
    
    intrusive_nary_named_func_node( func_type f , std::string name )
    : m_func( std::move( f ) ) , m_name( std::move( name ) ) { }
    
    result_type eval( context_type & context ) const
    {
        return m_func( context , *this );
    }
    
    std::string const& name( void ) const
    {
        return m_name;
    }
    
    bool operator==( intrusive_nary_named_func_node const &other ) const
    {
        return m_name == other.m_name;
    }
    
    bool operator!=( intrusive_nary_named_func_node const& other ) const
    {
        return ! ( *this == other );
    }
    
private:
    
    func_type m_func;
    std::string m_name;
};

template< typename Res , typename Context , size_t MaxArity >
std::ostream& operator<<( std::ostream &out , intrusive_nary_named_func_node< Res , Context , MaxArity > const& node )
{
    out << node.name();
    return out;
}



} // namespace gpcxx


#endif // GPCXX_TREE_INTRUSIVE_NODES_INTRUSIVE_NARY_NAMED_FUNC_NODE_HPP_INCLUDED
