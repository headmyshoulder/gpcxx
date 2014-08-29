# Finds quickbook, boostbook dtd, boostbook xsl, docbock dtd, docbook xsl
#
# sets
#
# QUICKBOOK_EXECUTABLE
# BOOSTBOOK_DTD_DIR
# BOOSTBOOK_XSL_DIR
# DOCBOOK_DTD_DIR
# DOCBOOK_XSL_DIR
#
# Quickbook is search in the standard path for binaries and in 
#
# ENV( QUICKBOOK_ROOT )
#
# DOCBOOK is searched in 
# ENV( DOCBOOK_ROOT )/docbook-dtd
# ENV( DOCBOOK_ROOT )/docbook-xsl
#
# BOOSTBOOK is searched in 
# ENV( BOOSTBOOK_PATH )/dtd
# ENV( BOOSTBOOK_PATH )/xsl




if ( IS_DIRECTORY $ENV{QUICKBOOK_ROOT} )
    if ( DEFINED QUICKBOOK_ROOT )
        message ( STATUS "For QUICKBOOK_ROOT, overwriting cmake variable with environment variable." )
    endif ()
    set ( QUICKBOOK_ROOT $ENV{QUICKBOOK_ROOT} )
endif ()

if ( IS_DIRECTORY $ENV{DOCBOOK_ROOT}  )
    if ( DEFINED DOCBOOK_ROOT )
        message ( STATUS "For DOCBOOK_ROOT, overwriting cmake variable with environment variable." )
    endif ()
    set ( DOCBOOK_ROOT $ENV{DOCBOOK_ROOT} )  
endif ()

if ( IS_DIRECTORY $ENV{BOOSTBOOK_ROOT} )
    if ( DEFINED BOOSTBOOK_ROOT )
        message ( STATUS "For BOOSTBOOK_ROOT, overwriting cmake variable with environment variable." )
    endif ()
    set ( BOOSTBOOK_ROOT $ENV{BOOSTBOOK_ROOT} )
endif ()

message ( STATUS "QUICKBOOK_ROOT has the value  \"${QUICKBOOK_ROOT}\" ." )
message ( STATUS "DOCBOOK_ROOT has the value \"${DOCBOOK_ROOT}\" ." )
message ( STATUS "BOOSTBOOK_ROOT has the value  \"${BOOSTBOOK_ROOT}\" ." )


find_program ( QUICKBOOK_EXECUTABLE NAMES quickbook DOC "the quickbook tool" PATHS ${QUICKBOOK_ROOT} )

# Find the Boostbook DTD
# Find the DocBook DTD (version 4.2)
# Find the DocBook XSL stylesheets
find_path ( BOOSTBOOK_DTD_DIR boostbook.dtd PATHS "${BOOSTBOOK_ROOT}/dtd" )
find_path ( BOOSTBOOK_XSL_DIR html.xsl      PATHS "${BOOSTBOOK_ROOT}/xsl" )
find_path ( DOCBOOK_DTD_DIR   docbookx.dtd  PATHS "${DOCBOOK_ROOT}/docbook-dtd"  )
find_path ( DOCBOOK_XSL_DIR   html/html.xsl PATHS "${DOCBOOK_ROOT}/docbook-xsl" )

set ( QuickBook_FOUND TRUE )

if ( NOT QUICKBOOK_EXECUTABLE )
  message ( WARNING "Could not find quickbook. You need to install boost in order to use quickbook and to put it into your default search path or set an environment variable QUICKBOOK_ROOT on the location of quickbook. Quickbook is usually located in $BOOST_ROOT/dist/bin." )
  set ( QuickBook_FOUND FALSE )
endif ()

if ( NOT BOOSTBOOK_DTD_DIR )
  message ( WARNING "Could not find Boostbook DTD!" )
  set ( QuickBook_FOUND FALSE )
endif ()

if ( NOT BOOSTBOOK_XSL_DIR )
  message ( WARNING "Could not find Boostbook XSL stylesheets!" )
  set ( QuickBook_FOUND FALSE )
endif ()

if ( NOT DOCBOOK_DTD_DIR )
  message ( WARNING "Could not find DocBook DTD!" )
  set ( QuickBook_FOUND FALSE )
endif ()

if ( NOT DOCBOOK_XSL_DIR )
  message ( WARNING "Could not find DocBook XSL stylesheets!" )
  set ( QuickBook_FOUND FALSE )
endif ()
