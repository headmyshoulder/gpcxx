# Date: 2013-01-25
# Author: Karsten Ahnert (karsten.ahnert@gmx.de)


include_directories ( ${PROJECT_SOURCE_DIR}/third_party/de )

add_executable ( symb_reg_de symb_reg_de.cpp )

target_link_libraries( symb_reg_de ${Boost_LIBRARIES} )