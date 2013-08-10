/*
  gp/util/version.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GP_UTIL_VERSION_HPP_DEFINED
#define GP_UTIL_VERSION_HPP_DEFINED

#include <gp/config_version.hpp>

#include <string>


namespace gp {

inline std::string get_version_string( void )
{
    return std::string( "v" ) + std::to_string( GP_VERSION_MAJOR ) + "." + std::to_string( GP_VERSION_MINOR ) + "." + std::to_string( GP_VERSION_PATCH );
}

constexpr int get_version_major( void )
{
    return GP_VERSION_MAJOR;
}

constexpr int get_version_minor( void )
{
    return GP_VERSION_MINOR;
}

constexpr int get_version_patch( void )
{
    return GP_VERSION_PATCH;
}

inline std::string get_version_sha1( void )
{
    return "GP_VERSION_SHA1";
}


} // namespace gp


#endif // GP_UTIL_VERSION_HPP_DEFINED
