/*
 * gpcxx/canonic/summarize_constants.hpp
 * Date: 2015-09-17
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_CANONIC_SUMMARIZE_CONSTANTS_HPP_INCLUDED
#define GPCXX_CANONIC_SUMMARIZE_CONSTANTS_HPP_INCLUDED

#include <gpcxx/tree/transform_tree.hpp>

#include <algorithm>


namespace gpcxx {

template< typename ConstantGenerator >
struct summarize_constants
{
    ConstantGenerator m_generator;
    summarize_constants( ConstantGenerator const& generator )
    : m_generator( generator ) {}
    
    template< typename Tree , typename Cursor >
    rule_result operator()( Tree& t , Cursor c ) const
    {
        if( ( c.size() > 0 ) && std::all_of( c.begin() , c.end() , []( auto const& n ) { return n.constant(); } ) )
        {
            using node_type = typename Tree::node_type;
            using context_type = typename node_type::context_type;
            using result_type = typename node_type::result_type;
            
            context_type context {};
            result_type res = c->eval( context );
            
            Tree tmp_tree;
            tmp_tree.insert( tmp_tree.root() , m_generator( res ) );
            t.swap_subtrees( c , tmp_tree , tmp_tree.root() );
            return ascent;
        }
        return descent;
    }
};

template< typename ConstantGenerator >
summarize_constants< ConstantGenerator > make_summarize_constant( ConstantGenerator const& generator )
{
    return summarize_constants< ConstantGenerator > { generator };
}



} // namespace gpcxx


#endif // GPCXX_CANONIC_SUMMARIZE_CONSTANTS_HPP_INCLUDED
