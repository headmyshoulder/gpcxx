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
                size_t num_children = left.size();
                auto node = c.node();
                for( size_t i=0 ; i<num_children ; ++i )
                {
                    node->insert_child( i , left.children(i).node() );
                }
                node->remove_child( left.node() );
                
                // t.m_size--;

                
//                 auto val = *left;
//                 Tree tmp_tree;
//                 tmp_tree.insert( tmp_tree.root() , val );
//                 tmp_tree.swap_subtrees( tmp_tree.root() , t , left );
// 
//                 t.insert( c.children( 0 ) , val );
//                 t.insert( c.children( 0 ) , val );
//                 t.erase( c.children( 2 ) );
//                 t.swap_subtrees( c.children( 0 ) , tmp_tree , tmp_tree.root().children( 0 ) );
//                 t.swap_subtrees( c.children( 1 ) , tmp_tree , tmp_tree.root().children( 1 ) );
                return gpcxx::repeat;
            }
        }
        return gpcxx::descent;
    }    
};



} // namespace gpcxx


#endif // GPCXX_CANONIC_LEFT_LIFT_HPP_INCLUDED
