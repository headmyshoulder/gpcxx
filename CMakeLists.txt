# Date: 2013-01-25
# Author: Karsten Ahnert (karsten.ahnert@gmx.de)

project ( gpcxx )

cmake_minimum_required ( VERSION 2.8 )


set ( THIRDPARTY_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/third_party" )

option ( GPCXX_BUILD_DOCS "Enable to build the docs" ON )
option ( GPCXX_BUILD_PERFORMANCE_TESTS "Enable to build the performance tests" ON )
option ( GPCXX_BUILD_EXAMPLES "Enable to build the examples" ON )
option ( GPCXX_BUILD_TESTS "Enable to build the tests" ON )
option ( GPCXX_TEST_COVERAGE "Analyze test coverage with gcov/lcov" OFF )
option ( GPCXX_DOC_GITHUB_LINKS "Use links to github" ON )
option ( GPCXX_CHECK_HEADER_COMPILE "Check if headers compile standalone" OFF )

# Add path for custom modules
set ( CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
  )

set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall -Wpedantic -pedantic -Werror" )

if ( "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++" )
endif ()
# elseif (CMAKE_COMPILER_IS_GNUCXX )
# endif ()

if ( ${GPCXX_TEST_COVERAGE} )
  message ( STATUS "Build with test coverage support." )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage" )
endif ()

include ( CreateVersionFile )
include ( ForwardingHeader )
include ( CheckHeaders )

message ( STATUS "Building project ${CMAKE_PROJECT_NAME}" )
message ( STATUS "Version ${GPCXX_VERSION_SHORT}" )

find_package ( Boost REQUIRED COMPONENTS thread system )

include_directories ( SYSTEM ${Boost_INCLUDE_DIRS} )
include_directories ( ${CMAKE_CURRENT_SOURCE_DIR}/include )

create_forwarding_header ( include/gpcxx/app.hpp include/gpcxx/app )
create_forwarding_header ( include/gpcxx/benchmark_problems.hpp include/gpcxx/app/benchmark_problems )
create_forwarding_header ( include/gpcxx/primitive_sets.hpp include/gpcxx/app/primitive_sets )
create_forwarding_header ( include/gpcxx/eval.hpp include/gpcxx/eval )
create_forwarding_header ( include/gpcxx/evolve.hpp include/gpcxx/evolve )
create_forwarding_header ( include/gpcxx/generate.hpp include/gpcxx/generate )
create_forwarding_header ( include/gpcxx/io.hpp include/gpcxx/io )
create_forwarding_header ( include/gpcxx/operator.hpp include/gpcxx/operator )
create_forwarding_header ( include/gpcxx/stat.hpp include/gpcxx/stat )
create_forwarding_header ( include/gpcxx/tree.hpp include/gpcxx/tree include/gpcxx/tree/iterator )
create_forwarding_header ( include/gpcxx/intrusive_nodes.hpp include/gpcxx/tree/intrusive_nodes )
create_forwarding_header ( include/gpcxx/util.hpp include/gpcxx/util )
create_forwarding_header ( include/gpcxx/gpcxx.hpp include/gpcxx )
create_forwarding_header ( include/gpcxx/canonic.hpp include/gpcxx/canonic )

if ( ${GPCXX_CHECK_HEADER_COMPILE} )
  check_if_headers_compile ( include/gpcxx/app )
  check_if_headers_compile ( include/gpcxx/eval )
  check_if_headers_compile ( include/gpcxx/evolve )
  check_if_headers_compile ( include/gpcxx/generate )
  check_if_headers_compile ( include/gpcxx/io )
  check_if_headers_compile ( include/gpcxx/operator )
  check_if_headers_compile ( include/gpcxx/stat )
  check_if_headers_compile ( include/gpcxx/tree )
  check_if_headers_compile ( include/gpcxx/util )
  check_if_headers_compile ( include/gpcxx/canonic )
endif ()


if ( ${GPCXX_BUILD_EXAMPLES} )
  add_subdirectory ( examples )
endif ()

if ( ${GPCXX_BUILD_PERFORMANCE_TESTS} )
  add_subdirectory ( performance )
endif ()

if ( ${GPCXX_BUILD_DOCS} )
  add_subdirectory ( doc )
endif ()


if ( ${GPCXX_BUILD_TESTS} )

  set( GMOCK_ROOT "${THIRDPARTY_ROOT}/gmock" )
  set( GTEST_ROOT "${THIRDPARTY_ROOT}/gmock/gtest" )

  if ( IS_DIRECTORY ${GMOCK_ROOT}  AND IS_DIRECTORY ${GTEST_ROOT} )
  
    include_directories ( SYSTEM ${GTEST_ROOT} ${GTEST_ROOT}/include ${GMOCK_ROOT} ${GMOCK_ROOT}/include )
  
    add_library ( gtest ${GTEST_ROOT}/src/gtest-all.cc )
    target_link_libraries ( gtest pthread )
    add_library ( gtest_main ${GTEST_ROOT}/src/gtest_main.cc )
    add_library ( gmock ${GMOCK_ROOT}/src/gmock-all.cc )
  
    enable_testing ( true )
  
    add_subdirectory ( test )

  else () 

    if ( NOT IS_DIRECTORY ${GMOCK_ROOT} )
      message ( STATUS "Google C++ Mockin Framework not found in \"${GMOCK_ROOT}\" tests will be disabled !" )
    endif ()

    if ( NOT IS_DIRECTORY ${GTEST_ROOT} )
      message ( STATUS "Google C++ Testing Framework not found in \"${GTEST_ROOT}\" tests will be disabled !" )
    endif ()

    message ( STATUS "get googlemock: svn checkout http://googlemock.googlecode.com/svn/tags/release-1.7.0/ \"${THIRDPARTY_ROOT}/gmock\"" )

  endif ()

endif ()
