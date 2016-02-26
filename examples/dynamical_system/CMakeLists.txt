# CMakeLists.txt
# Date: 2015-02-18
# Author: Karsten Ahnert (karsten.ahnert@gmx.de)
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or
# copy at http://www.boost.org/LICENSE_1_0.txt)
#

add_executable ( lorenz lorenz.cpp )

add_executable ( lorenz_multi lorenz_multi.cpp generate_data.cpp )
add_executable ( lorenz_single lorenz_single.cpp generate_data.cpp )
add_executable ( lorenz_reconstruct lorenz_reconstruct.cpp )

target_link_libraries ( lorenz_multi ${Boost_PROGRAM_OPTIONS_LIBRARY} )
target_link_libraries ( lorenz_single ${Boost_PROGRAM_OPTIONS_LIBRARY} )
target_link_libraries ( lorenz_reconstruct ${Boost_PROGRAM_OPTIONS_LIBRARY} )