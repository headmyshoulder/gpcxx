/*
 * gpcxx/canonic/order_commutative.hpp
 * Date: 2015-09-08
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_CANONIC_ORDER_COMMUTATIVE_HPP_INCLUDED
#define GPCXX_CANONIC_ORDER_COMMUTATIVE_HPP_INCLUDED

#include <gpcxx/canonic/algebras_rule.hpp>
#include <gpcxx/tree/transform_tree.hpp>

#include <algorithm>

namespace gpcxx {

template< typename Algebras >
struct order_commutative : protected algebras_rule< Algebras >
{
    using algebras_rule< Algebras >::algebras_rule;
    
    template< typename Tree , typename Cursor >
    gpcxx::rule_result operator()( Tree& t , Cursor c ) const
    {
        using node_type = typename Tree::node_type;
        auto get_value = []( auto ptr1 ) {return * static_cast< node_type const* >( ptr1 ); };
        auto cmp = [get_value]( auto ptr1 , auto ptr2 ) { return get_value( ptr1 ) < get_value( ptr2 ); };
        
        auto& cc = c.node()->get_children();
        if( this->m_algebras->is_commutative( *c ) 
            && ! std::is_sorted( cc.begin() , cc.end() , cmp ) )
        {
            std::sort( cc.begin() , cc.end() , cmp );
            return gpcxx::repeat;
        }
        return gpcxx::descent;
    }
};



} // namespace gpcxx


#endif // GPCXX_CANONIC_ORDER_COMMUTATIVE_HPP_INCLUDED
