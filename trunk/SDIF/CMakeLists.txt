#
# cmake configuration file
#
# This file is part of the IRCAM SDIF Library (http://www.ircam.fr/sdif)
#
# Copyright (C) 2006 by IRCAM - Centre Pompidou, Paris, France.
#
# created by Axel Roebel                     7.12.2006
#
# This file computes/replaces some default values:
#
# CMAKE_BUILD_TYPE     : default value is "release" other selectable value is "debug"
# CMAKE_INSTALL_PREFIX : default value is "/u/formes/share" 
# PKGCONFIGDIR         : /u/formes/share/lib/$ENV{SYS}/pkgconfig 
#
# The default values can be disabled by means of setting USECMAKEDEFAULTS:BOOL=ON on the command line
# if you choose this you are expected to provide reasonable values for all of the above
# variables or you are happy with the cmake defaults.
#
# On macosx and if the SDKs are available it replaces gcc/c++ by an script that sets
# an environment variable MACOSX_DEPLOYMENT_TARGET to 10.3 (for ppc architectures)
# and 10.4 (for intel architectures) 
#
# For partly replacing the default values you may run cmake twice, first to create
# a CMakeCache.txt with the above default values and second to override some of the default values
# by means of command line arguments.
#
# default values can be reestablished if the related variable is set to the value "default"
#
#  OPTIONS provided
#  USE_THREADS:BOOL=ON/OFF enable disable use of threads
#  BUILD_MING:BOOL=ON/OFF  if cygwin add -mno-cygwin to compiler commands

PROJECT(SDIF)

FILE(READ ${CMAKE_CURRENT_SOURCE_DIR}/configure.in CONFIGIN)
STRING(REGEX REPLACE ".*SDIF_VERSION_MAJOR=\([0-9]+\).*" "\\1" SDIF_VERSION_MAJOR ${CONFIGIN})
STRING(REGEX REPLACE ".*SDIF_VERSION_MINOR=\([0-9]+\).*" "\\1" SDIF_VERSION_MINOR ${CONFIGIN})
STRING(REGEX REPLACE ".*SDIF_VERSION_RELEASE=\([0-9]+\).*" "\\1" SDIF_VERSION_RELEASE ${CONFIGIN})

SET(VERSION  ${SDIF_VERSION_MAJOR}.${SDIF_VERSION_MINOR}.${SDIF_VERSION_RELEASE})
SET(SDIF_VERSION  ${SDIF_VERSION_MAJOR}.${SDIF_VERSION_MINOR}.${SDIF_VERSION_RELEASE})

MESSAGE("===============================================================")
MESSAGE("== Configuring SDIF   ${VERSION} ")
MESSAGE("===============================================================")

SET(PACKAGE "\"SDIF\"")

SET(CMAKE_VERBOSE_MAKEFILE false)
SET(CMAKE_SKIP_RPATH OFF)
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmModules)


# # prevent the use of the autoconf default values 
# # we impose our own handling
# EXTERNAL_CXXFLAGS=$CXXFLAGS

# AM_INIT_AUTOMAKE($PACKAGE,$VERSION)

# AM_CONFIG_HEADER(easdif/easdif_config.hpp)

IF(APPLE)
  INCLUDE(REDIRECT_COMPILERS_TO_SYSROOT OPTIONAL)
ENDIF(APPLE)


IF (WIN32)
  IF(CYGWIN AND NOT MINGW)
    OPTION(BUILD_MINGW "Build MingW Version" ON)
    IF(BUILD_MINGW)
      ADD_DEFINITIONS (-mno-cygwin )
      #SET(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -mno-cygwin")
      #SET(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -mno-cygwin")
      LIST(APPEND CMAKE_REQUIRED_DEFINITIONS -mno-cygwin)
      LIST(APPEND CMAKE_REQUIRED_FLAGS -mno-cygwin)
      LINK_LIBRARIES(-mno-cygwin)
    ENDIF(BUILD_MINGW)
  ENDIF(CYGWIN AND NOT MINGW)
ENDIF(WIN32)

# set default values for build type and install prefix
INCLUDE(SET_LOCAL_DEFAULTS OPTIONAL)

INCLUDE(GET_ARCH)

IF   ( "$ENV{SYS}" STREQUAL "" )
  SET(ARCHDIR ${ARCH})
  SET(INSTALL_LIB_DIR  lib)
  SET(INSTALL_BIN_DIR  bin)
ELSE ( "$ENV{SYS}" STREQUAL "" )
  SET(ARCHDIR $ENV{SYS})
  SET(INSTALL_LIB_DIR lib/$ENV{SYS})
  SET(INSTALL_BIN_DIR bin/$ENV{SYS})
ENDIF( "$ENV{SYS}" STREQUAL "" )

SET(INSTALL_INC_DIR include)


############################################################
# Checks for programs.

FIND_PROGRAM(HAVEDOT NAMES dot )



## Configure SDIF

MESSAGE("CHECK FOR HEADERS")
INCLUDE(CheckIncludeFile)

CHECK_INCLUDE_FILE(stdint.h    HAVE_STDINT_H)
CHECK_INCLUDE_FILE(stdlib.h    HAVE_STDLIB_H)
CHECK_INCLUDE_FILE(stdarg.h    HAVE_STDARG_H)
CHECK_INCLUDE_FILE(dlfcn.h     HAVE_DLFCN_H)
CHECK_INCLUDE_FILE(string.h    HAVE_STRING_H)
CHECK_INCLUDE_FILE(strings.h   HAVE_STRINGS_H)
CHECK_INCLUDE_FILE(inttypes.h  HAVE_INTTYPES_H)
CHECK_INCLUDE_FILE(memory.h    HAVE_MEMORY_H)
CHECK_INCLUDE_FILE(float.h     HAVE_FLOAT_H)
CHECK_INCLUDE_FILE(sys/stat.h  HAVE_SYS_STAT_H)
CHECK_INCLUDE_FILE(sys/types.h HAVE_SYS_TYPES_H)
CHECK_INCLUDE_FILE(errno.h     HAVE_ERRNO_H)
CHECK_INCLUDE_FILE(unistd.h    HAVE_UNISTD_H)

# only very rough test
IF(HAVE_STDLIB_H AND HAVE_STDARG_H AND HAVE_STRING_H AND HAVE_FLOAT_H)
SET(STDC_HEADERS 1)
ENDIF(HAVE_STDLIB_H AND HAVE_STDARG_H AND HAVE_STRING_H AND HAVE_FLOAT_H)

INCLUDE(CheckFunctionExists)
CHECK_FUNCTION_EXISTS(getenv HAVE_GETENV)
CHECK_FUNCTION_EXISTS(ftruncate HAVE_FTRUNCATE)

INCLUDE(FindThreads)
OPTION(USE_THREADS "Enable threads" ON)

IF(USE_THREADS)
  IF(CMAKE_USE_PTHREADS_INIT)
    ADD_DEFINITIONS(-DHAVE_PTHREAD)
    SET(HAVE_PTHREAD 1)
    LINK_LIBRARIES (${CMAKE_THREAD_LIBS_INIT})
  ENDIF(CMAKE_USE_PTHREADS_INIT)
ENDIF(USE_THREADS)


############################################################
# Particular compiler characteristics
MESSAGE("PARTICULAR COMPILER CHARACTERISTICS")
#dnl Check for BigEndian (WORDS_BIGENDIAN)
INCLUDE(TestBigEndian)
TEST_BIG_ENDIAN(WORDS_BIGENDIAN)

#dnl Check for Long 64 bits
INCLUDE(CHECK_SIZEOF)
CHECK_SIZEOF(long 4)
#AC_CHECK_SIZEOF(long)
	

# EaSDIF Configure
IF   (CMAKE_BUILD_TYPE MATCHES debug)
  SET(DEBUGEXT _debug)
ELSE (CMAKE_BUILD_TYPE MATCHES debug)
  SET(DEBUGEXT )
ENDIF(CMAKE_BUILD_TYPE MATCHES debug)

############################################################
# dnl find SDIF objects that have to be included into Easdif
# AC_MSG_RESULT(FIND SDIF FILES)
# AC_SUBST(SDIFCPPLIB)
# SDIFCPPLIB=../SDIF/sdifcpp/libsdif-cpp.la


#CONFIGURE_FILE(SDIF/sdif/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config/config.h)
FILE(READ ${CMAKE_CURRENT_SOURCE_DIR}/sdif/config.h.in CONFIG)
STRING(REGEX REPLACE "#undef[ \t]*\([^ \t\n\r]*\)" "#cmakedefine \\1 @\\1@" SDIFCMCONFIG ${CONFIG})
STRING(CONFIGURE ${SDIFCMCONFIG} SDIFCMCONFIGURED)

IF(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/sdifconfig/config.h)
  FILE(READ ${CMAKE_CURRENT_BINARY_DIR}/sdifconfig/config.h CURRENTCONFIG)
ENDIF(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/sdifconfig/config.h)

IF(NOT "${CURRENTCONFIG}" STREQUAL "${SDIFCMCONFIGURED}")
  MESSAGE("Writing sdif config")
  FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/sdifconfig/config.h ${SDIFCMCONFIGURED})
ENDIF(NOT "${CURRENTCONFIG}" STREQUAL "${SDIFCMCONFIGURED}")

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/include/sdif_version.h.in ${CMAKE_CURRENT_BINARY_DIR}/sdifconfig/sdif_version.h)

# include special traget to remove cache, system and compiler info
IF(CMAKE_GENERATOR MATCHES "^Unix Makefiles")
  INCLUDE(CLEAN_CMAKE)
  INCLUDE(CVS_Targets)
  INCLUDE(BuildTypeTargets)
ENDIF(CMAKE_GENERATOR MATCHES "^Unix Makefiles")

INCLUDE(CREATE_CVSTAGS_MACRO)
# create cvstags for current version
CREATE_CVSTAG_TARGET("SDIF_${SDIF_VERSION_MAJ}_${SDIF_VERSION_MIN}_${SDIF_VERSION_RELEASE}")

ADD_DEFINITIONS("-DHAVE_CONFIG_H")

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/sdifconfig ${CMAKE_CURRENT_BINARY_DIR}/sdif ${CMAKE_CURRENT_SOURCE_DIR}/include  )

ADD_SUBDIRECTORY( sdif ) 


########### install files ###############
FILE(TO_CMAKE_PATH "${PKGCONFIGDIR}" tmp)

IF(NOT "${tmp}" STREQUAL  "")
  IF("${tmp}" MATCHES "/.*")
    SET(PKGCONFIGPATH "${tmp}" CACHE PATH "install path for pkg config files" FORCE)
    MESSAGE("pkgconfig  abs    : ${PKGCONFIGPATH}")
  ELSE("${tmp}" MATCHES "/.*")
    IF("${tmp}" STREQUAL "default" )
      SET(PKGCONFIGPATH "${CMAKE_INSTALL_PREFIX}/${INSTALL_LIB_DIR}/pkgconfig" CACHE PATH "install path for pkg config files" FORCE)
      MESSAGE("pkgconfig  rel   : ${CMAKE_INSTALL_PREFIX}/${INSTALL_LIB_DIR}/pkgconfig")
    ELSE("${tmp}" STREQUAL "default" )
      SET(PKGCONFIGPATH "${CMAKE_INSTALL_PREFIX}/${INSTALL_LIB_DIR}/${tmp}" CACHE PATH "install path for pkg config files" FORCE)
    ENDIF("${tmp}" STREQUAL "default" )
    MESSAGE("pkgconfig  rel   : ${PKGCONFIGPATH}")  
  ENDIF("${tmp}" MATCHES "/.*")
ENDIF(NOT "${tmp}" STREQUAL  "")

# if install path is empty set to default
IF("${PKGCONFIGPATH}" STREQUAL  "")
  SET(PKGCONFIGPATH "${CMAKE_INSTALL_PREFIX}/${INSTALL_LIB_DIR}/pkgconfig" CACHE PATH "install path for pkg config files" FORCE)
ENDIF("${PKGCONFIGPATH}" STREQUAL  "")

MESSAGE("Installation in:")
MESSAGE("Prefix         : ${CMAKE_INSTALL_PREFIX}")
MESSAGE("bin            : ${CMAKE_INSTALL_PREFIX}/${INSTALL_BIN_DIR}")
MESSAGE("lib            : ${CMAKE_INSTALL_PREFIX}/${INSTALL_LIB_DIR}")
MESSAGE("includes       : ${CMAKE_INSTALL_PREFIX}/${INSTALL_INC_DIR}")
MESSAGE("pkgconfig      : ${PKGCONFIGPATH}")


IF(NOT CMAKE_BUILD_TYPE MATCHES debug)
  SET(prefix ${CMAKE_INSTALL_PREFIX})
  SET(exec_prefix ${CMAKE_INSTALL_PREFIX})
  SET(libdir ${CMAKE_INSTALL_PREFIX}/lib)
  SET(includedir ${CMAKE_INSTALL_PREFIX}/include)
  CONFIGURE_FILE(sdif.pc.in  sdif.pc )
  INSTALL(FILES  ${CMAKE_CURRENT_BINARY_DIR}/sdif.pc DESTINATION ${PKGCONFIGPATH} )
ENDIF(NOT CMAKE_BUILD_TYPE MATCHES debug)


# install sdif headers
INSTALL(FILES  ${CMAKE_CURRENT_BINARY_DIR}/sdifconfig/sdif_version.h DESTINATION ${INSTALL_INC_DIR})
INSTALL(FILES  ${CMAKE_CURRENT_SOURCE_DIR}/SDIF/include/sdif.h DESTINATION ${INSTALL_INC_DIR})


ADD_SUBDIRECTORY( tools ) 
ADD_SUBDIRECTORY( test ) 
