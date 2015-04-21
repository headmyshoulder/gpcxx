/*
 * gpcxx/operator/detail/operator_base.hpp
 * Date: 2015-04-21
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_OPERATOR_DETAIL_OPERATOR_BASE_HPP_INCLUDED
#define GPCXX_OPERATOR_DETAIL_OPERATOR_BASE_HPP_INCLUDED

#include <cstddef>


namespace gpcxx {
namespace detail {


template < size_t Arity >
struct operator_base
{
    static const size_t arity = Arity;
};


} // namespace detail
} // namespace gpcxx


#endif // GPCXX_OPERATOR_DETAIL_OPERATOR_BASE_HPP_INCLUDED
