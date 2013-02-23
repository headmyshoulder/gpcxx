# Date: 2013-01-25
# Author: Karsten Ahnert (karsten.ahnert@gmx.de)

project ( genetic_programming )
cmake_minimum_required ( VERSION 2.8 )

# Add path for custom modules
set ( CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
  )


if ( CMAKE_COMPILER_IS_GNUCXX )
  set ( CMAKE_CXX_FLAGS "-std=c++0x" )
endif ()

set ( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR} )

find_package ( Boost REQUIRED COMPONENTS thread system )
find_package ( Amboss REQUIRED )

include_directories ( ${Boost_INCLUDE_DIRS} )
include_directories ( ${CMAKE_CURRENT_SOURCE_DIR}/include )
include_directories ( ${AMBOSS_INCLUDE_DIR} )

add_subdirectory ( app )


if ( IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/third_party/gtest )
  
  enable_testing ( true )
  add_subdirectory ( third_party/gtest )
  add_subdirectory ( test )

else () 

  message ( STATUS "googletest not found!" )

endif ()
