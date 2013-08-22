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


set ( QUICKBOOK_ROOT $ENV{QUICKBOOK_ROOT} )
set ( DOCBOOK_ROOT $ENV{DOCBOOK_ROOT} )
set ( BOOSTBOOK_ROOT $ENV{BOOSTBOOK_ROOT} )


find_program ( QUICKBOOK_EXECUTABLE NAMES quickbook DOC "the quickbook tool" PATHS ${QUICKBOOK_ROOT} )

# Find the Boostbook DTD
# Find the DocBook DTD (version 4.2)
# Find the DocBook XSL stylesheets
find_path ( BOOSTBOOK_DTD_DIR boostbook.dtd PATHS "${BOOSTBOOK_ROOT}/dtd" )
find_path ( BOOSTBOOK_XSL_DIR html.xsl PATHS "${BOOSTBOOK_ROOT}/xsl" )
find_path ( DOCBOOK_DTD_DIR docbookx.dtd PATHS "${DOCBOOK_ROOT}/docbook-dtd"  )
find_path ( DOCBOOK_XSL_DIR html/html.xsl PATHS "${DOCBOOK_ROOT}/docbook-xsl" )

message ( STATUS ${DOCBOOK_ROOT} )
message ( STATUS ${BOOSTBOOK_ROOT} )

if ( NOT QUICKBOOK_EXECUTABLE )
  message ( WARNING "could not find quickbook" )
endif ()

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