# option(KARROT_BUILD_DOCS "Build documentation" OFF)
# if(NOT KARROT_BUILD_DOCS)
  # return()
# endif()

find_package ( Doxygen REQUIRED )
find_package ( XSLTPROC REQUIRED )


find_program( Quickbook_EXECUTABLE NAMES quickbook DOC "the quickbook tool" )

set ( DOCBOOK_PATH "/home/karsten/boost/docbook/" )
set ( BOOST_BOOK_PATH "${BOOST_ROOT}/tools/boostbook" )

# Find the Boostbook DTD
# Find the DocBook DTD (version 4.2)
# Find the DocBook XSL stylesheets
find_path ( BOOSTBOOK_DTD_DIR boostbook.dtd PATHS "${BOOST_BOOK_PATH}/dtd" )
find_path ( BOOSTBOOK_XSL_DIR html.xsl PATHS "${BOOST_BOOK_PATH}/xsl" )
find_path ( DOCBOOK_DTD_DIR docbookx.dtd PATHS "${DOCBOOK_PATH}/docbook-dtd"  )
find_path ( DOCBOOK_XSL_DIR html/html.xsl PATHS "${DOCBOOK_PATH}/docbook-xsl" )

  
if ( NOT BOOSTBOOK_DTD_DIR )
  message ( WARNING "could not find Boostbook DTD!" )
endif ()

if ( NOT BOOSTBOOK_XSL_DIR )
  message ( WARNING "could not find Boostbook XSL stylesheets!" )
endif ()

if ( NOT DOCBOOK_DTD_DIR )
  message ( WARNING "could not find DocBook DTD!" )
endif ()

if ( NOT DOCBOOK_XSL_DIR )
  message ( WARNING "could not find DocBook XSL stylesheets!" )
endif ()


set ( catalog "${CMAKE_CURRENT_BINARY_DIR}/catalog.xml" )
configure_file ( catalog.xml.in "${catalog}" )

set ( xml_dir "${CMAKE_CURRENT_BINARY_DIR}/xml" )




add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/genetic_programming.qbk"
  COMMAND
    ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/genetic_programming.qbk"
    "${CMAKE_CURRENT_BINARY_DIR}/genetic_programming.qbk"
  DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/genetic_programming.qbk"
  )

add_custom_command(
  OUTPUT
    "${CMAKE_CURRENT_BINARY_DIR}/genetic_programming.xml"
  COMMAND
    ${Quickbook_EXECUTABLE}
    --input-file "${CMAKE_CURRENT_BINARY_DIR}/genetic_programming.qbk"
    --include-path "${CMAKE_CURRENT_BINARY_DIR}"
    --include-path "${CMAKE_CURRENT_SOURCE_DIR}"
    --output-file "${CMAKE_CURRENT_BINARY_DIR}/genetic_programming.xml"
  DEPENDS
    "${CMAKE_CURRENT_BINARY_DIR}/genetic_programming.qbk"
  )

xsltproc(
  INPUT "${CMAKE_CURRENT_BINARY_DIR}/genetic_programming.xml"
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/genetic_programming.dbk"
  CATALOG "${catalog}"
  STYLESHEET "${BOOSTBOOK_XSL_DIR}/docbook.xsl"
  DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/genetic_programming.xml"
  )

xsltproc(
  INPUT "${CMAKE_CURRENT_BINARY_DIR}/genetic_programming.dbk"
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/html/index.html"
  CATALOG "${catalog}"
  STYLESHEET "${BOOSTBOOK_XSL_DIR}/xhtml.xsl"
  )

add_custom_target ( documentation
  DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/html/index.html"
  )