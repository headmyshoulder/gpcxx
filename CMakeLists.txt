# Date: 2013-01-25
# Author: Karsten Ahnert (karsten.ahnert@gmx.de)

project (PROJECT)
cmake_minimum_required (VERSION 2.6) 

# Add path for custom modules
set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
  )

find_package (Boost REQUIRED)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=c++0x")        ## Optimize
endif()

include_directories ( ${Boost_INCLUDE_DIRS} )

add_executable ( main main.cpp )
add_executable ( test_copy test_copy.cpp )
add_executable ( main_optimize main_optimize.cpp )
add_executable ( test_prob test_prob.cpp )
