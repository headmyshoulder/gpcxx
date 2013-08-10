/*
  gpcxx/util/version.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GPCXX_UTIL_VERSION_HPP_DEFINED
#define GPCXX_UTIL_VERSION_HPP_DEFINED

#include <gpcxx/config_version.hpp>

#include <string>


namespace gpcxx {

inline std::string get_version_string( void )
{
    return std::string( "v" ) + std::to_string( GPCXX_VERSION_MAJOR ) + "." + std::to_string( GPCXX_VERSION_MINOR ) + "." + std::to_string( GPCXX_VERSION_PATCH );
}

constexpr int get_version_major( void )
{
    return GPCXX_VERSION_MAJOR;
}

constexpr int get_version_minor( void )
{
    return GPCXX_VERSION_MINOR;
}

constexpr int get_version_patch( void )
{
    return GPCXX_VERSION_PATCH;
}

inline std::string get_version_sha1( void )
{
    return "GPCXX_VERSION_SHA1";
}


} // namespace gpcxx


#endif // GPCXX_UTIL_VERSION_HPP_DEFINED
