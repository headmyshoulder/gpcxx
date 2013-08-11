/*
  identity.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef IDENTITY_HPP_DEFINED
#define IDENTITY_HPP_DEFINED


namespace gpcxx {


struct identity
{
    template< typename T >
    T operator()( T const &t ) const
    {
        return t;
    }
};



} // namespace gpcxx


#endif // IDENTITY_HPP_DEFINED
