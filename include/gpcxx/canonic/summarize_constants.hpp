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
        using node_type = typename Tree::node_type;
        using context_type = typename node_type::context_type;
        using result_type = typename node_type::result_type;
        
        size_t start_constant = 0;
        for( size_t i=0 ; i<c.size() ; ++i )
            if( ! c.children(i)->constant() ) start_constant = i+1;
            
        if( start_constant != c.size() )
        {
            if( start_constant == 0 )
            {
                context_type context {};
                result_type res = c->eval( context );
                
                Tree tmp_tree;
                tmp_tree.insert( tmp_tree.root() , m_generator( res ) );
                t.swap_subtrees( c , tmp_tree , tmp_tree.root() );
            }
            else
            {
                if( ( c.size() > 1 ) && ( ( start_constant + 1 ) == c.size() ) )
                    return descent;
                
                Tree tmp1;
                tmp1.insert( tmp1.root() , *c );
                for( size_t i = start_constant ; i <c.size() ; ++i )
                    tmp1.insert_below( tmp1.root() , *( c.children(i) ) );
                
                context_type context {};
                result_type res = tmp1.root()->eval( context );
                
                while( c.size() > start_constant )
                    t.erase( c.children( start_constant ) );
                t.insert_below( c , m_generator( res ) );
            }
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
