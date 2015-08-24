/*
 * gpcxx/io/detail/read_bracket.hpp
 * Date: 2015-08-18
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_IO_DETAIL_READ_BRACKET_HPP_INCLUDED
#define GPCXX_IO_DETAIL_READ_BRACKET_HPP_INCLUDED

#include <gpcxx/util/exception.hpp>

#include <boost/algorithm/string/find.hpp>
#include <boost/range.hpp>


namespace gpcxx {
namespace detail {

    


template< typename Rng1 , typename Rng2 , typename Rng3 >    
std::string get_node( Rng1 const& rng , Rng2 const& p1 , Rng3 const& p2 )
{
    auto m = std::min( boost::begin( p1 ) , boost::begin( p2 ) );
    return std::string { boost::begin( rng ) , m } ;
}


template< typename Rng >
std::string get_string( Rng const& rng )
{
    return std::string{ boost::begin( rng ) , boost::end( rng ) };
}



template< typename Rng , typename Tree , typename Cursor , typename Mapper >
Rng read_bracket_impl( Rng const& rng , Tree& tree , Cursor cursor , Mapper const& mapper , std::string const& opening , std::string const& closing )
{
    auto p1 = boost::algorithm::find_first( rng , opening );
    auto p2 = boost::algorithm::find_first( rng , closing );

    if( boost::empty( p2 ) )
    {
        throw gpcxx_exception( "Missing closing bracket" );
    }

    std::string node = get_node( rng , p1 , p2 );
    auto c = tree.insert_below( cursor , mapper( node ) );

    while( boost::begin( p1 ) < boost::begin( p2 ) )
    {
        auto next = boost::make_iterator_range( boost::end( p1 ) , boost::end( rng ) );
        // cout << "Inside loop \"" << get_string( next ) << "\"" << endl;
        auto n = read_bracket_impl( next , tree , c , mapper , opening , closing );
        p1 = boost::algorithm::find_first( n , opening );
        p2 = boost::algorithm::find_first( n , closing );
        if( boost::empty( p2 ) )
        {
            throw gpcxx_exception( "Missing closing bracket" );
        }
    }
    return boost::make_iterator_range( boost::end( p2 ) , boost::end( rng ) );
}

template< typename Rng , typename Tree , typename Mapper >
void read_bracket( Rng const& rng , Tree& tree , Mapper const& mapper , std::string const& opening , std::string const& closing )
{
    try
    {
        auto opening_pos = boost::algorithm::find_first( rng , opening );
        if( boost::empty( opening_pos ) )
        {
            throw gpcxx_exception( "Could not find opening" );
        }
        
        auto closing_pos = read_bracket_impl( boost::make_iterator_range( boost::end( opening_pos ) , boost::end( rng ) ) , tree , tree.root() , mapper , opening , closing );
        
        if( ! boost::empty( closing_pos ) )
        {
            throw gpcxx_exception( "Could not find closing" );
        }
    }
    catch( gpcxx_exception& e )
    {
        throw gpcxx_exception( e.what() + std::string( " in \"" ) + get_string( rng ) + "\"" );
    }
}


} // namespace detail
} // namespace gpcxx


#endif // GPCXX_IO_DETAIL_READ_BRACKET_HPP_INCLUDED
