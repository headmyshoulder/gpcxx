/*
 * gpcxx/canonic/left_lift.hpp
 * Date: 2015-09-15
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_CANONIC_LEFT_LIFT_HPP_INCLUDED
#define GPCXX_CANONIC_LEFT_LIFT_HPP_INCLUDED

#include <gpcxx/canonic/algebras_rule.hpp>
#include <gpcxx/tree/transform_tree.hpp>

namespace gpcxx {

    
template< typename Algebras >
struct left_lift : protected algebras_rule< Algebras >
{
    using algebras_rule< Algebras >::algebras_rule;
    
    template< typename Tree , typename Cursor >
    gpcxx::rule_result operator()( Tree& t , Cursor c ) const
    {
        if( this->m_algebras->is_associative( *c ) )
        {
            if( c.size() < 2 )
                throw gpcxx::gpcxx_exception( "Associative nodes need to have more then 1 children." );

            auto left = c.children( 0 );
            if( this->m_algebras->is_associative( *left ) && ( (*c) == (*left) ) )
            {
                while( left->size() > 0 )
                {
                    // attention: left get invalidated in this call
                    t.move_and_insert_subtree( left , left.children( 0 ) );
                    ++left;
                }
                t.erase( left );
                return gpcxx::repeat;
            }
        }
        return gpcxx::descent;
    }    
};



} // namespace gpcxx


#endif // GPCXX_CANONIC_LEFT_LIFT_HPP_INCLUDED
