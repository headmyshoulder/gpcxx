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

#include <string>
#include <algorithm>

namespace gpcxx {
namespace detail {


template< typename Iter , typename Tree , typename Cursor , typename Mapper >
Iter read_bracket_impl( Iter first , Iter last , Tree& tree , Cursor cursor , std::string const& opening , std::string const& closing , Mapper const& mapper )
{
//     if( first == last ) return first;
// 
//     auto i = std::min( std::find( first , last , opening ) , std::find( first , last , closing ) );
//     if i == last:
//         throw no closing bracket in 
//     create new node
//     if i == opening:
//         i = iterate while opening
//     if i != last 
//         throw "No closing bracket in "
// 
//     return first;
}


} // namespace detail
} // namespace gpcxx


#endif // GPCXX_IO_DETAIL_READ_BRACKET_HPP_INCLUDED
