# Date: 2013-01-25
# Author: Karsten Ahnert (karsten.ahnert@gmx.de)

include_directories ( ${gtest_SOURCE_DIR}/include )
include_directories ( ${gtest_SOURCE_DIR} )


add_executable ( canonic_tests
  algebraic_node.cpp
  algebras.cpp
  group.cpp
  order_commutative.cpp
  canonic_complete.cpp
  )

target_link_libraries ( canonic_tests gtest gtest_main )

add_test( NAME canonic_tests COMMAND canonic_tests )
