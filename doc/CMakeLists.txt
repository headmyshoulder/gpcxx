find_package ( Doxygen REQUIRED )
find_package ( XSLTPROC REQUIRED )
find_package ( QuickBook )


set ( catalog "${CMAKE_CURRENT_BINARY_DIR}/catalog.xml" )
configure_file ( catalog.xml.in "${catalog}" )

set ( xml_dir "${CMAKE_CURRENT_BINARY_DIR}/xml" )




add_custom_command ( OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/genetic_programming.qbk"
  COMMAND
    ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/genetic_programming.qbk"
    "${CMAKE_CURRENT_BINARY_DIR}/genetic_programming.qbk"
  DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/genetic_programming.qbk"
  )

add_custom_command ( OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/genetic_programming.xml"
  COMMAND
    ${QUICKBOOK_EXECUTABLE}
    --input-file "${CMAKE_CURRENT_BINARY_DIR}/genetic_programming.qbk"
    --include-path "${CMAKE_CURRENT_BINARY_DIR}"
    --include-path "${CMAKE_CURRENT_SOURCE_DIR}"
    --output-file "${CMAKE_CURRENT_BINARY_DIR}/genetic_programming.xml"
  DEPENDS
    "${CMAKE_CURRENT_BINARY_DIR}/genetic_programming.qbk"
  )
  
add_custom_command ( OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/html/boostbook.css"
  COMMAND
   ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/boostbook.css"
    "${CMAKE_CURRENT_BINARY_DIR}/html/boostbook.css"
  DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/boostbook.css"
  )
  
add_custom_command ( OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/html/images" 
  COMMAND
   ${CMAKE_COMMAND} -E copy_directory 
    "${CMAKE_CURRENT_SOURCE_DIR}/images"
    "${CMAKE_CURRENT_BINARY_DIR}/html/images"
  DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/images"
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
  DEPENDS
  "${CMAKE_CURRENT_BINARY_DIR}/html/index.html"
  "${CMAKE_CURRENT_BINARY_DIR}/html/boostbook.css"
  "${CMAKE_CURRENT_BINARY_DIR}/html/images" 
  )