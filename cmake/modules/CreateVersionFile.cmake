find_package ( Git )

if ( NOT EXISTS "${CMAKE_SOURCE_DIR}/.git/" )
  return ()
endif ()

execute_process ( COMMAND git describe --abbrev=4 HEAD
                  COMMAND sed -e "s/-/./g"
                  OUTPUT_VARIABLE GP_GIT_VERSION
                  OUTPUT_STRIP_TRAILING_WHITESPACE )
                  
execute_process ( COMMAND git status -uno -s 
                  OUTPUT_VARIABLE GP_GIT_STATUS 
                  OUTPUT_STRIP_TRAILING_WHITESPACE )


string ( REGEX REPLACE "^v([0-9]+)\\..*" "\\1" GP_VERSION_MAJOR "${GP_GIT_VERSION}" )
string ( REGEX REPLACE "^v[0-9]+\\.([0-9]+).*" "\\1" GP_VERSION_MINOR "${GP_GIT_VERSION}" )
string ( REGEX REPLACE "^v[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" GP_VERSION_PATCH "${GP_GIT_VERSION}" )
string ( REGEX REPLACE "^v[0-9]+\\.[0-9]+\\.[0-9]+.(.*)" "\\1" GP_VERSION_SHA1 "${GP_GIT_VERSION}" )

if ( ${GP_VERSION_PATCH} STREQUAL ${GP_GIT_VERSION} )
  set ( GP_VERSION_PATCH "0" )
  set ( GP_VERSION_SHA1 "0" )
endif ()

math ( EXPR GP_VERSION_PATCH "${GP_VERSION_PATCH} + 1" )

set ( GP_VERSION_SHORT "${GP_VERSION_MAJOR}.${GP_VERSION_MINOR}.${GP_VERSION_PATCH}" )

# message ( STATUS "${GP_VERSION_MAJOR}" )
# message ( STATUS "${GP_VERSION_MINOR}" )
# message ( STATUS "${GP_VERSION_PATCH}" )
# message ( STATUS "${GP_VERSION_SHA1}" )
# message ( STATUS "${GP_VERSION_SHORT}" )

if ( NOT ( GP_GIT_STATUS STREQUAL "" ) )

  message ( STATUS "generating new config_version.hpp" )
  configure_file ( ${CMAKE_SOURCE_DIR}/include/gp/config_version.hpp.cmake ${CMAKE_SOURCE_DIR}/include/gp/config_version.hpp )

endif ()

