# Date: 2013-01-25
# Author: Karsten Ahnert (karsten.ahnert@gmx.de)

include_directories ( ${gtest_SOURCE_DIR}/include )
include_directories ( ${gtest_SOURCE_DIR} )


add_executable ( tree_tests
  basic_nary_tree.cpp
  basic_tree.cpp
  general_tree.cpp
  intrusive_tree.cpp
  preorder_iterator.cpp
  postorder_iterator.cpp
  tree_base.cpp
  )

target_link_libraries ( tree_tests gtest gtest_main )

add_test( NAME tree_tests COMMAND tree_tests )

# add_custom_command(TARGET UtilTests POST_BUILD COMMAND UtilTests )
