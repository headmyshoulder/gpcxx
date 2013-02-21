# Date: 2013-01-25
# Author: Karsten Ahnert (karsten.ahnert@gmx.de)

project ( genetic_programming )
cmake_minimum_required ( VERSION 2.8 )


if ( CMAKE_COMPILER_IS_GNUCXX )
  set ( CMAKE_CXX_FLAGS "-std=c++0x" )
endif ()

include_directories ( ${Boost_INCLUDE_DIRS} )
include_directories ( ${CMAKE_CURRENT_SOURCE_DIR}/include )

# add_executable ( main main.cpp )
# add_executable ( test_copy test_copy.cpp )
# add_executable ( main_optimize main_optimize.cpp )
# add_executable ( test_prob test_prob.cpp )

# add_subdirectory ( stuff )

add_subdirectory ( app )

if ( IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/gtest )
  
  enable_testing ( true )
  add_subdirectory ( gtest )
  add_subdirectory ( test )

else () 

  message ( STATUS "googletest not found!" )

endif ()
