# Date: 2013-01-25
# Author: Karsten Ahnert (karsten.ahnert@gmx.de)

include_directories ( ${gtest_SOURCE_DIR}/include )
include_directories ( ${gtest_SOURCE_DIR} )

set ( GPCXX_STD_THREAD 1 )
set ( GPCXX_BOOST_THREAD 2 )
set ( GPCXX_DUMMY_THREAD 3 )

set ( GPCXX_USE_THREAD_IMPLEMENTATION ${GPCXX_BOOST_THREAD} )

add_definitions(
    -DGPCXX_BOOST_THREAD=${GPCXX_BOOST_THREAD}
    -DGPCXX_STD_THREAD=${GPCXX_STD_THREAD}
    -DGPCXX_DUMMY_THREAD=${GPCXX_DUMMY_THREAD}
)


add_executable ( util_tests create_random_indices.cpp sort_indices.cpp version.cpp iterate_until.cpp array_unpack.cpp parallel_algorithm.cpp)


target_link_libraries ( util_tests gtest gtest_main )

if( ${GPCXX_USE_THREAD_IMPLEMENTATION} STREQUAL ${GPCXX_BOOST_THREAD} )
    
    add_definitions(
        -DGPCXX_USE_THREAD_IMPLEMENTATION=${GPCXX_BOOST_THREAD}
    )
    find_package ( Boost REQUIRED COMPONENTS thread )
    
    target_link_libraries(util_tests ${Boost_LIBRARIES} )
    
else ()
    
    find_package( Threads )
    add_definitions(
        -DGPCXX_USE_THREAD_IMPLEMENTATION=${GPCXX_STD_THREAD}
    )
    
endif ()

add_test( NAME util_tests COMMAND util_tests )

# add_custom_command(TARGET UtilTests POST_BUILD COMMAND UtilTests )