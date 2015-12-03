/*
 * gpcxx/tree/transform_tree.hpp
 * Date: 2015-08-12
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_TREE_TRANSFORM_TREE_HPP_INCLUDED
#define GPCXX_TREE_TRANSFORM_TREE_HPP_INCLUDED

#include <gpcxx/util/exception.hpp>

#include <string>
#include <ostream>

namespace gpcxx {

// template< typename Tree >
// using rule = std::function< return_value( Tree& , typename Tree::cursor ) >;
// 
// 
// template< typename Tree >
// using rule_engine = std::vector< rule< Tree > >;
    
enum rule_result { ascent , descent , repeat };

inline std::string to_string( rule_result v )
{
    switch( v )
    {
        case ascent:
            return "ascent";
            break;
        case descent:
            return "descent";
            break;
        case repeat:
            return "repeat";
            break;
        default:
            return "unknown";
            break;
    }
}

inline std::ostream& operator<<( std::ostream& out , rule_result const& v )
{
    out << to_string( v );
    return out;
}


namespace detail {

    /// \returns true if the rules should by applied to the parent again
    template< typename Rules , typename Tree >
    bool apply_rules_to_cursor( Rules const& rules , typename Tree::cursor c , Tree& tree , size_t max_trials = 1000 )
    {
        size_t count = 0;
        bool redo = true;
        while( redo )
        {
            ++count;
            if( count == max_trials )
                throw gpcxx_exception( "Maximal number of trials has been reached in apply_rules_to_cursor" );

            redo = false;
            for( auto const& rule : rules )
            {
                auto t = tree;
                auto ret = rule( tree , c );
                if( ret == repeat )
                {
                    redo = true;
                    break;
                }
                else if( ret == ascent )
                {
                    if( ! c.is_root() ) return true;
                    else
                    {
                        redo = true;
                        break;
                    }
                    
                }
            }
        }

        
        bool ascent = false;
        for( size_t i=0 ; i<c.size() ; ++i )
        {
            ascent = apply_rules_to_cursor( rules , c.children( i ) , tree );
            if( ascent ) break;
        }
        return ascent;
    }
    
    
} // namespace detail

template< typename Rules , typename Tree >
void transform_tree( Rules const& rules , Tree& tree , size_t max_trials = 1000 )
{
    bool repeat = true;
    size_t count = 0;
    while( repeat )
    {
        repeat = detail::apply_rules_to_cursor( rules , tree.root() , tree );
        ++count;
        if( count == max_trials )
            throw gpcxx_exception( "Maximal number of trials reached in transform_tree." );
    }
}

template< typename Rules , typename Tree >
Tree transform_tree_copy( Rules const& rules , Tree tree , size_t max_trials = 1000 )
{
    transform_tree( rules , tree , max_trials );
    return tree;
}

} // namespace gpcxx


#endif // GPCXX_TREE_TREE_TRANSFORM_HPP_INCLUDED
