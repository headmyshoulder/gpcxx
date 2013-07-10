# Date: 2013-01-25
# Author: Karsten Ahnert (karsten.ahnert@gmx.de)

include_directories ( ${gtest_SOURCE_DIR}/include )
include_directories ( ${gtest_SOURCE_DIR} )


add_executable ( operator_tests
   mutation.cpp
   simple_mutation_strategy.cpp
   one_point_crossover_strategy.cpp
   fitness_prob.cpp
   random_selector.cpp
   tournament_selector.cpp
   crossover.cpp
   reproduce.cpp
  )

target_link_libraries ( operator_tests gtest gtest_main )

add_test( NAME operator_tests COMMAND operator_tests )

# add_custom_command(TARGET UtilTests POST_BUILD COMMAND UtilTests )