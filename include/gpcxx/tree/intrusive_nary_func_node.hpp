/*
 * gpcxx/tree/intrusive_nary_func_node.hpp
 * Date: 2015-05-18
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_TREE_INTRUSIVE_NARY_FUNC_NODE_HPP_INCLUDED
#define GPCXX_TREE_INTRUSIVE_NARY_FUNC_NODE_HPP_INCLUDED


#include <gpcxx/tree/intrusive_nary_node.hpp>

#include <functional>


namespace gpcxx {

// Attention this class is not intended to be copied around
template< typename Res , typename Context , size_t Arity = 2 >
class intrusive_nary_func_node : public gpcxx::intrusive_nary_node< intrusive_nary_func_node< Res , Context , Arity > , Arity >
{
public:
    
    using result_type = Res;
    using context_type = Context;
    using node_type = intrusive_nary_func_node< result_type , context_type, Arity >;
    
    
    typedef std::function< result_type( context_type const& , node_type const& ) > func_type;
    
    intrusive_nary_func_node( func_type f )
    : m_func( std::move( f ) ) { }
    
    result_type eval( context_type const & context ) const
    {
        return m_func( context , *this );
    }
    
private:
    
    func_type m_func;
};



} // namespace gpcxx


#endif // GPCXX_TREE_INTRUSIVE_NARY_FUNC_NODE_HPP_INCLUDED
