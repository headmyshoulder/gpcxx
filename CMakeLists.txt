# Date: 2013-01-25
# Author: Karsten Ahnert (karsten.ahnert@gmx.de)

# set ( CMAKE_C_COMPILER "/usr/bin/gcc-4.7" )
# set ( CMAKE_CXX_COMPILER "/usr/bin/g++-4.7" )

set ( CMAKE_C_COMPILER "/home/karsten/boost/testing/gcc-4.7.0/build/bin/gcc" )
set ( CMAKE_CXX_COMPILER "//home/karsten/boost/testing/gcc-4.7.0/build/bin/g++" )


project ( genetic_programming )
cmake_minimum_required ( VERSION 2.8 )

# Add path for custom modules
set ( CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
  )

set ( CMAKE_CXX_FLAGS "-std=c++11" )


set ( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR} )

find_package ( Boost REQUIRED COMPONENTS thread system )
find_package ( Amboss REQUIRED )

include_directories ( ${Boost_INCLUDE_DIRS} )
include_directories ( ${CMAKE_CURRENT_SOURCE_DIR}/include )
include_directories ( ${AMBOSS_INCLUDE_DIR} )

# add_subdirectory ( app )


if ( IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/third_party/gtest )
  
  enable_testing ( true )
  add_subdirectory ( third_party/gtest )
  add_subdirectory ( test )

else () 

  message ( STATUS "googletest not found!" )

endif ()
