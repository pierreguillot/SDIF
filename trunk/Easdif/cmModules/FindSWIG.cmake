# - Find SWIG
# This module finds an installed SWIG.  It sets the following variables:
#  SWIG_FOUND - set to true if SWIG is found
#  SWIG_DIR - the directory where swig is installed
#  SWIG_EXECUTABLE - the path to the swig executable
#  SWIG_VERSION   - the version number of the swig executable
#
# All informations are collected from the SWIG_EXECUTABLE so the 
# version to be found can be changed from the command line by 
# means of setting SWIG_EXECUTABLE
#

SET(SWIG_FOUND FALSE)

FIND_PROGRAM(SWIG_EXECUTABLE swig )

IF(SWIG_EXECUTABLE)
  SET(SWIG ${SWIG_EXECUTABLE})
  EXECUTE_PROCESS(COMMAND ${SWIG_EXECUTABLE} -swiglib OUTPUT_VARIABLE SWIG_dir_tmp RESULT_VARIABLE result)

  IF(NOT ${result} EQUAL 0)
    MESSAGE(SEND_ERROR "Command \"${SWIG_EXECUTABLE} -swiglib\" failed with output:\n${SWIG_dir_tmp}")
  ELSE(NOT ${result} EQUAL 0)
    STRING(REGEX  REPLACE "[\n\r]+" ";"  SWIG_dir_tmp_nocr  ${SWIG_dir_tmp})
    MESSAGE("libdir ${SWIG_dir_tmp_nocr}")
    FIND_PATH(SWIG_DIR  swig.swg PATHS ${SWIG_dir_tmp_nocr})
    IF(SWIG_DIR)
      SET(SWIG_FOUND 1)
      SET(SWIG_USE_FILE ${CMAKE_ROOT}/Modules/UseSWIG.cmake)   
      EXECUTE_PROCESS(COMMAND ${SWIG_EXECUTABLE} -version OUTPUT_VARIABLE SWIG_version_tmp RESULT_VARIABLE result)
      IF(${result} EQUAL 0)
        STRING(REGEX  REPLACE ".*SWIG Version[^0-9.]*\([0-9.]+\).*" "\\1"  SWIG_version_tmp_2  ${SWIG_version_tmp})        
        SET(SWIG_VERSION ${SWIG_version_tmp_2} CACHE STRING "Swig version")
      ELSE(${result} EQUAL 0)
        MESSAGE(SEND_ERROR "Command \"${SWIG_EXECUTABLE} -version\" failed with output:\n${SWIG_version_tmp}")
      ENDIF(${result} EQUAL 0)
    ENDIF(SWIG_DIR)    
  ENDIF(NOT ${result} EQUAL 0)
ENDIF(SWIG_EXECUTABLE)

IF(NOT SWIG_FOUND)
  IF(SWIG_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Swig was not found on the system. Please specify the location of Swig.")
  ELSE(SWIG_FIND_REQUIRED)
    IF(NOT FIND_SWIG_QUIETLY)
      MESSAGE(STATUS "SWIG_EXECUTABLE -- not found")
    ENDIF(NOT FIND_SWIG_QUIETLY)
  ENDIF(SWIG_FIND_REQUIRED)
ELSE(NOT SWIG_FOUND)
  IF(NOT FIND_SWIG_QUIETLY)
    MESSAGE(STATUS "using swig ${SWIG_DIR}")
  ENDIF(NOT FIND_SWIG_QUIETLY)
ENDIF(NOT SWIG_FOUND)


