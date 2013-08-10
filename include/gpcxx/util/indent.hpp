/*
  gpcxx/util/indent.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GPCXX_UTIL_INDENT_HPP_DEFINED
#define GPCXX_UTIL_INDENT_HPP_DEFINED

#include <string>

namespace gpcxx {

inline std::string indent( size_t count , std::string const& indent_string = "  " )
{
    std::string ret = "";
    for( size_t i=0 ; i<count ; ++i )
        ret += indent_string;
    return ret;
}

} // namespace gpcxx


#endif // GPCXX_UTIL_INDENT_HPP_DEFINED
