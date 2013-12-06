# Date: 2013-01-25
# Author: Karsten Ahnert (karsten.ahnert@gmx.de)

# set ( CMAKE_C_COMPILER "/home/karsten/boost/testing/gcc-4.7.0/build/bin/gcc" )
# set ( CMAKE_CXX_COMPILER "//home/karsten/boost/testing/gcc-4.7.0/build/bin/g++" )
# set ( CMAKE_C_COMPILER "gcc-4.7" )
# set ( CMAKE_CXX_COMPILER "g++-4.7" )


project ( genetic_programming )
cmake_minimum_required ( VERSION 2.8 )

# set ( CMAKE_C_COMPILER "g++-4.8" )
# set ( CMAKE_CXX_COMPILER "g++-4.8" )
# # set ( CMAKE_CXX_FLAGS "-std=c++11 -Wall" )

ADD_DEFINITIONS(
    -std=c++11
)

set ( THIRDPARTY_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/third_party" )

# Add path for custom modules
set ( CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
  ${THIRDPARTY_ROOT}/ctoolbox 
  )

include ( CreateVersionFile )

message ( STATUS "Building project ${CMAKE_PROJECT_NAME}" )
message ( STATUS "Version ${GPCXX_VERSION_SHORT}" )


set ( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR} )

find_package ( Boost REQUIRED COMPONENTS thread system )

if ( ${Boost_FOUND} )
  message ( STATUS "Using boost include dir from directory ${Boost_INCLUDE_DIRS}" )
  message ( STATUS "Using the following boost libraries ${Boost_LIBRARIES}" )
endif ()

include_directories ( ${Boost_INCLUDE_DIRS} )
include_directories ( ${CMAKE_CURRENT_SOURCE_DIR}/include )

add_subdirectory ( examples )
add_subdirectory ( performance )
add_subdirectory ( doc )

set( GMOCK_ROOT "${THIRDPARTY_ROOT}/gmock" )
set( GTEST_ROOT "${THIRDPARTY_ROOT}/gmock/gtest" )

if ( IS_DIRECTORY ${GMOCK_ROOT}  AND IS_DIRECTORY ${GTEST_ROOT} )
  
  include_directories ( ${GTEST_ROOT} ${GTEST_ROOT}/include ${GMOCK_ROOT} ${GMOCK_ROOT}/include )
  
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

  message ( STATUS "    get googlemock/googletest by: svn checkout http://googlemock.googlecode.com/svn/trunk/ \"${GMOCK_ROOT}\" " )

endif ()
