

if ( ${DOXYGEN_FOUND} AND ${XSLTPROC_FOUND} AND ${QuickBook_FOUND} )


set ( catalog "${CMAKE_CURRENT_BINARY_DIR}/catalog.xml" )
configure_file ( catalog.xml.in "${catalog}" )

set ( xml_dir "${CMAKE_CURRENT_BINARY_DIR}/xml" )
set ( doxyfile "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile" )

file(WRITE "${doxyfile}"
  "QUIET = YES\n"
  "WARN_IF_UNDOCUMENTED = NO\n"
  "GENERATE_LATEX = NO\n"
  "GENERATE_HTML = NO\n"
  "GENERATE_XML = YES\n"
  "XML_OUTPUT = \"${xml_dir}\"\n"
  "INPUT = \"${CMAKE_SOURCE_DIR}/include/gpcxx\"\n"
  "RECURSIVE = YES\n"
  )
  

# add_custom_command(
#   OUTPUT
#     "${xml_dir}/index.xml"
#     "${xml_dir}/combine.xslt"
#   COMMAND
#     ${DOXYGEN_EXECUTABLE} ${doxyfile}
#   DEPENDS
#     "${CMAKE_SOURCE_DIR}/include/gpcxx"
#   )
#   
# # Collect Doxygen XML into a single XML file
# xsltproc(
#   INPUT "${xml_dir}/index.xml"
#   OUTPUT "${xml_dir}/all.xml"
#   STYLESHEET "${xml_dir}/combine.xslt"
#   )
# 
#   # Transform single Doxygen XML file into BoostBook XML
# xsltproc(
#   INPUT "${xml_dir}/all.xml"
#   OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/reference.xml"
#   STYLESHEET "${BOOSTBOOK_XSL_DIR}/doxygen/doxygen2boostbook.xsl"
#   PARAMETERS "boost.doxygen.header.prefix=gpcxx"
#   )

file( GLOB_RECURSE QBK_FILES *.qbk )

message ( STATUS "Qbk files for doc: ${QBK_FILES}" )

add_custom_command ( OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/gpcxx.qbk"
  COMMAND
    ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/gpcxx.qbk"
    "${CMAKE_CURRENT_BINARY_DIR}/gpcxx.qbk"
  DEPENDS
    ${QBK_FILES}
  )

add_custom_command ( OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/gpcxx.xml"
  COMMAND
    ${QUICKBOOK_EXECUTABLE}
    --input-file "${CMAKE_CURRENT_BINARY_DIR}/gpcxx.qbk"
    --include-path "${CMAKE_CURRENT_BINARY_DIR}"
    --include-path "${CMAKE_CURRENT_SOURCE_DIR}"
    --output-file "${CMAKE_CURRENT_BINARY_DIR}/gpcxx.xml"
  DEPENDS
    ${QBK_FILES}
    "${CMAKE_CURRENT_BINARY_DIR}/gpcxx.qbk"
#     "${CMAKE_CURRENT_BINARY_DIR}/reference.xml"
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
  INPUT "${CMAKE_CURRENT_BINARY_DIR}/gpcxx.xml"
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/gpcxx.dbk"
  CATALOG "${catalog}"
  STYLESHEET "${BOOSTBOOK_XSL_DIR}/docbook.xsl"
  DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/gpcxx.xml"
  PARAMETERS chunk.section.depth=2 chunk.first.sections=1 toc.section.depth=1 toc.max.depth=1 generate.section.toc.level=2
  )

xsltproc(
  INPUT "${CMAKE_CURRENT_BINARY_DIR}/gpcxx.dbk"
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/html/index.html"
  CATALOG "${catalog}"
  STYLESHEET "${BOOSTBOOK_XSL_DIR}/xhtml.xsl"
  )

add_custom_target ( documentation
  ALL
  DEPENDS
  "${CMAKE_CURRENT_BINARY_DIR}/html/index.html"
  "${CMAKE_CURRENT_BINARY_DIR}/html/boostbook.css"
  "${CMAKE_CURRENT_BINARY_DIR}/html/images" 
  )


else ()

  message ( WARNING "Could not find Doxygon, Xstlproc, or Quickbook. Doc will not be generated" )

endif ()
