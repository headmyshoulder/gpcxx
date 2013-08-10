# Date: 2013-01-25
# Author: Karsten Ahnert (karsten.ahnert@gmx.de)

# set ( CMAKE_C_COMPILER "/home/karsten/boost/testing/gcc-4.7.0/build/bin/gcc" )
# set ( CMAKE_CXX_COMPILER "//home/karsten/boost/testing/gcc-4.7.0/build/bin/g++" )
set ( CMAKE_C_COMPILER "gcc-4.7" )
set ( CMAKE_CXX_COMPILER "g++-4.7" )


project ( genetic_programming )
cmake_minimum_required ( VERSION 2.8 )

set ( CMAKE_C_COMPILER "g++-4.8" )
set ( CMAKE_CXX_COMPILER "g++-4.8" )
# set ( CMAKE_CXX_FLAGS "-std=c++11 -Wall" )
set ( CMAKE_CXX_FLAGS "-std=c++11" )

# Add path for custom modules
set ( CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
  ${CMAKE_CURRENT_SOURCE_DIR}/third_party/ctoolbox 
  )

include ( CreateVersionFile )

message ( STATUS "Building project ${CMAKE_PROJECT_NAME}" )
message ( STATUS "Version ${GP_VERSION_SHORT}" )


set ( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR} )

find_package ( Boost REQUIRED COMPONENTS thread system )

include_directories ( ${Boost_INCLUDE_DIRS} )
include_directories ( ${CMAKE_CURRENT_SOURCE_DIR}/include )

add_subdirectory ( examples )
add_subdirectory ( performance )
add_subdirectory ( doc )


if ( IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/third_party/gtest )
  
  include_directories ( third_party/gtest third_party/gtest/include third_party/gmock third_party/gmock/include )
  
  add_library ( gtest third_party/gtest/src/gtest-all.cc )
  target_link_libraries ( gtest pthread )
  add_library ( gtest_main third_party/gtest/src/gtest_main.cc )
  add_library ( gmock third_party/gmock/src/gmock-all.cc )
  
  enable_testing ( true )
  
  add_subdirectory ( test )

else () 

  message ( STATUS "googletest not found!" )

endif ()
