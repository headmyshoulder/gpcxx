/*
 * gpcxx/operator/operator_result.hpp
 * Date: 2015-03-23
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_OPERATOR_OPERATOR_RESULT_HPP_INCLUDED
#define GPCXX_OPERATOR_OPERATOR_RESULT_HPP_INCLUDED

#include <utility>
#include <type_traits>

namespace gpcxx {


template< typename Individuum , typename Iterator >
struct operator_result
{
    Individuum individuum;
    Iterator iterator;
    
    operator_result( Individuum const& _individuum , Iterator _iterator )
    : individuum( _individuum ) , iterator( _iterator )
    { }
    
    operator_result( Individuum&& _individuum , Iterator _iterator )
    : individuum( std::move( _individuum ) ) , iterator( _iterator )
    { }
    
    
    operator Individuum( void ) const
    {
        return individuum;
    }
};

template< typename Individuum , typename Iterator >
operator_result< typename std::remove_reference< Individuum >::type , Iterator >
make_operator_result( Individuum&& individuum , Iterator iterator )
{
    return operator_result< typename std::remove_reference< Individuum >::type , Iterator > ( std::forward< Individuum >( individuum ) , iterator );
}

} // namespace gpcxx


#endif // GPCXX_OPERATOR_OPERATOR_RESULT_HPP_INCLUDED
