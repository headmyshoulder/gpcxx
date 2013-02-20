# Date: 2013-01-25
# Author: Karsten Ahnert (karsten.ahnert@gmx.de)

include_directories ( ${gtest_SOURCE_DIR}/include )
include_directories ( ${gtest_SOURCE_DIR} )


add_executable ( util_tests create_random_indices.cpp sort_indices.cpp )


target_link_libraries ( util_tests gtest gtest_main )

add_test( NAME util_tests COMMAND util_tests )

# add_custom_command(TARGET UtilTests POST_BUILD COMMAND UtilTests )