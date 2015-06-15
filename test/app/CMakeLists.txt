# Date: 2013-01-25
# Author: Karsten Ahnert (karsten.ahnert@gmx.de)

include_directories ( ${gtest_SOURCE_DIR}/include )
include_directories ( ${gtest_SOURCE_DIR} )


add_executable ( app_tests
  generate_evenly_spaced_test_data.cpp
  generate_random_test_data.cpp
  )

target_link_libraries ( app_tests gtest gtest_main )

add_test( NAME app_tests COMMAND app_tests )
