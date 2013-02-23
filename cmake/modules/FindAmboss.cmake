#
# http://root.cern.ch/viewvc/trunk/cint/reflex/cmake/modules/FindCppUnit.cmake
#
# - Find CppUnit
# This module finds an installed CppUnit package.
#
# It sets the following variables:
#  AMBOSS_FOUND       - Set to false, or undefined, if CppUnit isn't found.
#  AMBOSS_INCLUDE_DIR - The Amboss include directory.



SET(AMBOSS_ROOT $ENV{AMBOSS_ROOT})

FIND_PATH(AMBOSS_INCLUDE_DIR Amboss/KML.h ${AMBOSS_ROOT}/include )

IF (AMBOSS_INCLUDE_DIR)
   SET(AMBOSS_FOUND TRUE)
ENDIF (AMBOSS_INCLUDE_DIR)

IF (AMBOSS_FOUND)

   # show which Amboss Directory was found only if not quiet
   IF (NOT AMBOSS_FIND_QUIETLY)
      MESSAGE(STATUS "Found Amboss include directory : ${AMBOSS_INCLUDE_DIR}")
   ENDIF (NOT AMBOSS_FIND_QUIETLY)

ELSE (AMBOSS_FOUND)

   # fatal error if Amboss is required but not found
   IF (AMBOSS_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find Amboss. Make sure Amboss can be found in the default include path or in the environment variable AMBOSS_ROOT")
   ELSE (AMBOSS_FIND_REQUIRED)
     MESSAGE(STATUS "Could not find Amboss. Make sure Amboss can be found in the default include path or in the environment variable AMBOSS_ROOT")
   ENDIF (AMBOSS_FIND_REQUIRED)

ENDIF (AMBOSS_FOUND)
