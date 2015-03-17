# Date: 2013-01-25
# Author: Karsten Ahnert (karsten.ahnert@gmx.de)

add_subdirectory ( io )
add_subdirectory ( operator )
add_subdirectory ( tree )
add_subdirectory ( util )
add_subdirectory ( generate )
add_subdirectory ( eval )
add_subdirectory ( stat )


# if ( ${GPCXX_TEST_COVERAGE} )
# 
#   # Resets coverage statistics
#   add_custom_target ( coverage_reset
#     COMMAND lcov --zerocounters --directory .
#     COMMAND lcov --capture --initial --directory . --base-directory "${PROJECT_SOURCE_DIR}/vexcl" --no-external --output-file coverage.info
#     VERBATIM
#   )
# 
#   # Converts accumulated coverage statistics into coverage/index.html
#   # Run
#   # make tests
#   # for all OpenCL platforms after
#   # make coverage_reset
#   # and before
#   # make coverage
#   add_custom_target ( coverage
#     COMMAND lcov --directory . --base-directory "${PROJECT_SOURCE_DIR}/vexcl" --no-external --capture --output-file coverage.info
#     COMMAND lcov --remove coverage.info '/usr*' '*/cl.hpp' -o coverage.info
#     COMMAND genhtml coverage.info --output-directory coverage
#     VERBATIM
#     )
# 
# endif ()