function ( check_if_headers_compile directory )

  message ( STATUS "Checking if headers in ${directory} compile." )
  
  set ( template_file "${CMAKE_SOURCE_DIR}/cmake/modules/compile_template.cpp.cmake" )
  set ( template_cpp_file "${CMAKE_BINARY_DIR}/compile_template.cpp" )
  
  file ( GLOB_RECURSE files "${CMAKE_SOURCE_DIR}/${directory}/*.hpp" )
  list ( SORT files )
  
  
  get_property ( dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)

  foreach ( f ${files} )
    
    set ( GPCXX_TEST_INCLUDE_FILE ${f} )
    
    set ( CXXFLAGS "${CXXFLAGS} -isystem ${Boost_INCLUDE_DIRS}" )
    
    configure_file ( "${template_file}" "${template_cpp_file}" )
    try_compile ( success "${CMAKE_BINARY_DIR}" "${template_cpp_file}"
        CMAKE_FLAGS "-DINCLUDE_DIRECTORIES=${dirs}"
        OUTPUT_VARIABLE output )
    
    if ( NOT ${success} )
      message ( STATUS "${f} does not compile." )
      message ( STATUS "Compiler message:" )
      message ( STATUS "" )
      message ( STATUS "\n${output}" )
      message ( STATUS "" )
      message ( STATUS "" )
      message ( STATUS "" )
      message ( STATUS "" )
      message ( FATAL_ERROR "${f} does not compile." )
    endif ()
    
    
  endforeach ()

endfunction ()
