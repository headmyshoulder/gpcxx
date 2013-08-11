# Date: 2013-07-14
# Author: Karsten Ahnert (karsten.ahnert@gmx.de)

include_directories ( ${gtest_SOURCE_DIR}/include )
include_directories ( ${gtest_SOURCE_DIR} )


add_executable ( eval_tests
  static_eval.cpp
  static_eval_erc.cpp
  )

target_link_libraries ( eval_tests gtest gtest_main )

add_test( NAME eval_tests COMMAND eval_tests )

# add_custom_command(TARGET UtilTests POST_BUILD COMMAND UtilTests )
