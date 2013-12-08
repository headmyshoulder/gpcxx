# Date: 2013-01-25
# Author: Karsten Ahnert (karsten.ahnert@gmx.de)

add_executable ( performance_pagie2 performance_pagie2.cpp )
add_executable ( performance_pagie2_intrusive performance_pagie2_intrusive.cpp )

add_executable ( performance_pagie2_intrusive_multi performance_pagie2_intrusive_multi.cpp )

find_package( Threads )
target_link_libraries( performance_pagie2_intrusive_multi ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES} )