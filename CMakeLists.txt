# Date: 2013-01-25
# Author: Karsten Ahnert (karsten.ahnert@gmx.de)

project ( genetic_programming )
cmake_minimum_required ( VERSION 2.6 )

# Add path for custom modules
set ( CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
  )

find_package ( Boost REQUIRED )

if ( CMAKE_COMPILER_IS_GNUCXX )
  set ( CMAKE_CXX_FLAGS "-std=c++0x" )
endif ()

include_directories ( ${Boost_INCLUDE_DIRS} )
include_directories ( include )

# add_executable ( main main.cpp )
# add_executable ( test_copy test_copy.cpp )
# add_executable ( main_optimize main_optimize.cpp )
# add_executable ( test_prob test_prob.cpp )

add_subdirectory ( stuff )


if ( IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/gtest )
  
  message ( STATUS "huhu" )
  enable_testing ( true )
  add_subdirectory ( gtest )
  add_subdirectory ( test )

else () 

  message ( STATUS "googletest not found!" )

endif ()
