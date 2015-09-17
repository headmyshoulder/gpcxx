/*
 * gpcxx/canonic/remove_double_inverse_functions.hpp
 * Date: 2015-09-17
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_CANONIC_REMOVE_DOUBLE_INVERSE_FUNCTIONS_HPP_INCLUDED
#define GPCXX_CANONIC_REMOVE_DOUBLE_INVERSE_FUNCTIONS_HPP_INCLUDED

#include <gpcxx/canonic/algebras_rule.hpp>
#include <gpcxx/tree/transform_tree.hpp>
#include <gpcxx/util/exception.hpp>


namespace gpcxx {

template< typename Algebras >
struct remove_double_inverse_functions : protected algebras_rule< Algebras >
{
    using algebras_rule< Algebras >::algebras_rule;
    
    template< typename Tree , typename Cursor >
    rule_result operator()( Tree& t , Cursor c ) const
    {
        auto optional_group = this->m_algebras->get_group_from_inverse_function( *c );
        if( optional_group )
        {
            
            auto const& group = optional_group.get();
            if( c.size() != 1 )
                throw gpcxx::gpcxx_exception( "An inverse functions needs to have exactly 1 child." );
            
            if( (* c.children(0) ) == group.inverse_function() )
            {
                if( c.children(0).size() != 1 )
                    throw gpcxx::gpcxx_exception( "An inverse functions needs to have exactly 1 child." );
                if( c.is_root() )
                {
                    Tree tmp;
                    t.swap_subtrees( c.children(0).children(0) , tmp , tmp.root() );
                    t.swap_subtrees( c , tmp , tmp.root() );
                }
                else
                {
                    auto r = t.insert( c , c.children(0).children(0) );
                    ++r;
                    t.erase( r );
                }
                return gpcxx::ascent;
            }
        }                
        return descent;
    }
};


} // namespace gpcxx


#endif // GPCXX_CANONIC_REMOVE_DOUBLE_INVERSE_FUNCTIONS_HPP_INCLUDED
