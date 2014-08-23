# Date: 2013-01-25
# Author: Karsten Ahnert (karsten.ahnert@gmx.de)

include_directories ( ${gtest_SOURCE_DIR}/include )
include_directories ( ${gtest_SOURCE_DIR} )


add_executable ( generate_tests
  uniform_symbol.cpp
  uniform_symbol_erc.cpp
  basic_generate_strategy.cpp
  node_generator.cpp
  )

target_link_libraries ( generate_tests gtest gtest_main )

add_test( NAME generate_tests COMMAND generate_tests )

# add_custom_command(TARGET UtilTests POST_BUILD COMMAND UtilTests )
