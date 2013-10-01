# Date: 2013-01-25
# Author: Karsten Ahnert (karsten.ahnert@gmx.de)

include_directories ( ${gtest_SOURCE_DIR}/include )
include_directories ( ${gtest_SOURCE_DIR} )


add_executable ( stat_tests
  node_statistics.cpp )

target_link_libraries ( stat_tests gtest gtest_main )

add_test( NAME stat_tests COMMAND stat_tests )
