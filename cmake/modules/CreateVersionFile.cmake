find_package ( Git )

if ( NOT EXISTS "${CMAKE_SOURCE_DIR}/.git/" )
  message ( WARNING "coud not find .git repository in ${CMAKE_SOURCE_DIR}/.git/" )
  return ()
endif ()



set ( GPCXX_VERSION_FILE ${CMAKE_SOURCE_DIR}/include/gpcxx/config_version.hpp )
set ( GPCXX_VERSION_FILE_TEMPLATE ${CMAKE_SOURCE_DIR}/include/gpcxx/config_version.hpp.cmake )

execute_process ( COMMAND git describe --tags --abbrev=4 --match v*.* HEAD
                  COMMAND sed -e "s/-/./g"
                  OUTPUT_VARIABLE GPCXX_GIT_VERSION
                  OUTPUT_STRIP_TRAILING_WHITESPACE
                  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/" )

message ( STATUS "${GPCXX_GIT_VERSION}" )
                  
execute_process ( COMMAND git status -uno -s 
                  OUTPUT_VARIABLE GPCXX_GIT_STATUS 
                  OUTPUT_STRIP_TRAILING_WHITESPACE
                  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/" )


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

if ( ( NOT ( GPCXX_GIT_STATUS STREQUAL "" ) ) OR ( NOT EXISTS ${GPCXX_VERSION_FILE} ) )

  message ( STATUS "generating new version file ${GPCXX_VERSION_FILE} from ${GPCXX_VERSION_FILE_TEMPLATE}" )
  configure_file ( ${GPCXX_VERSION_FILE_TEMPLATE} ${GPCXX_VERSION_FILE}  )

endif ()

