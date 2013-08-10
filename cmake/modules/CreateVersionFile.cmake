find_package ( Git )

if ( NOT EXISTS "${CMAKE_SOURCE_DIR}/.git/" )
  return ()
endif ()

execute_process ( COMMAND git describe --abbrev=4 HEAD
                  COMMAND sed -e "s/-/./g"
                  OUTPUT_VARIABLE GPCXX_GIT_VERSION
                  OUTPUT_STRIP_TRAILING_WHITESPACE )
                  
execute_process ( COMMAND git status -uno -s 
                  OUTPUT_VARIABLE GPCXX_GIT_STATUS 
                  OUTPUT_STRIP_TRAILING_WHITESPACE )


string ( REGEX REPLACE "^v([0-9]+)\\..*" "\\1" GPCXX_VERSION_MAJOR "${GPCXX_GIT_VERSION}" )
string ( REGEX REPLACE "^v[0-9]+\\.([0-9]+).*" "\\1" GPCXX_VERSION_MINOR "${GPCXX_GIT_VERSION}" )
string ( REGEX REPLACE "^v[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" GPCXX_VERSION_PATCH "${GPCXX_GIT_VERSION}" )
string ( REGEX REPLACE "^v[0-9]+\\.[0-9]+\\.[0-9]+.(.*)" "\\1" GPCXX_VERSION_SHA1 "${GPCXX_GIT_VERSION}" )

if ( ${GPCXX_VERSION_PATCH} STREQUAL ${GPCXX_GIT_VERSION} )
  set ( GPCXX_VERSION_PATCH "0" )
  set ( GPCXX_VERSION_SHA1 "0" )
endif ()

math ( EXPR GPCXX_VERSION_PATCH "${GPCXX_VERSION_PATCH} + 1" )

set ( GPCXX_VERSION_SHORT "${GPCXX_VERSION_MAJOR}.${GPCXX_VERSION_MINOR}.${GPCXX_VERSION_PATCH}" )

# message ( STATUS "${GPCXX_VERSION_MAJOR}" )
# message ( STATUS "${GPCXX_VERSION_MINOR}" )
# message ( STATUS "${GPCXX_VERSION_PATCH}" )
# message ( STATUS "${GPCXX_VERSION_SHA1}" )
# message ( STATUS "${GPCXX_VERSION_SHORT}" )

if ( NOT ( GPCXX_GIT_STATUS STREQUAL "" ) )

  message ( STATUS "generating new config_version.hpp" )
  configure_file ( ${CMAKE_SOURCE_DIR}/include/gpcxx/config_version.hpp.cmake ${CMAKE_SOURCE_DIR}/include/gpcxx/config_version.hpp )

endif ()

