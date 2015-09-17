/*
 * gpcxx/canonic/remove_invserse_operations.hpp
 * Date: 2015-09-17
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_CANONIC_REMOVE_INVSERSE_OPERATIONS_HPP_INCLUDED
#define GPCXX_CANONIC_REMOVE_INVSERSE_OPERATIONS_HPP_INCLUDED

#include <gpcxx/canonic/algebras_rule.hpp>
#include <gpcxx/tree/transform_tree.hpp>
#include <gpcxx/util/exception.hpp>


namespace gpcxx {

template< typename Algebras >
struct remove_inverse_operations : protected algebras_rule< Algebras >
{
    using algebras_rule< Algebras >::algebras_rule;
    
    template< typename Tree , typename Cursor >
    rule_result operator()( Tree& t , Cursor c ) const
    {
        auto optional_group = this->m_algebras->get_group_from_inverse_operation( *c );
        if( optional_group )
        {
            auto const& group = optional_group.get();
            
            if( c.size() != 2 )
                throw gpcxx::gpcxx_exception( "An inverse operations needs to have exactly 2 children." );
            
            *c = group.operation();
            t.insert_above( c.children( 1 ) , group.inverse_function() );
            return ascent;
        }
        return descent;
    }
};


} // namespace gpcxx


#endif // GPCXX_CANONIC_REMOVE_INVSERSE_OPERATIONS_HPP_INCLUDED
