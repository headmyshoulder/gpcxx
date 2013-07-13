find_package ( Git )

execute_process( COMMAND git describe --abbrev=4 HEAD
                 COMMAND sed -e "s/-/./g"
                 OUTPUT_VARIABLE POD_VERSION
                 OUTPUT_STRIP_TRAILING_WHITESPACE )
                 
message ( STATUS "${POD_VERSION}" )
