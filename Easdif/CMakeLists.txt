# This file computes/replaces some default values:
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

PROJECT(EaSDIF)

SET(EASDIF_VERSION_MAJ 1)
SET(EASDIF_VERSION_MIN 2)
SET(EASDIF_VERSION_RELEASE 9)
SET(EASDIF_VERSION  "${EASDIF_VERSION_MAJ}.${EASDIF_VERSION_MIN}.${EASDIF_VERSION_RELEASE}")

MESSAGE("===============================================================")
MESSAGE("== Configuring Easdif   ${EASDIF_VERSION} ")
MESSAGE("===============================================================")

SET(PACKAGE "\"Easdif\"")


SET(CMAKE_VERBOSE_MAKEFILE false)
SET(CMAKE_SKIP_RPATH OFF)
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/SDIF/cmModules)


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

SET(CMAKE_C_COMPILER ${CMAKE_CXX_COMPILER} )
INCLUDE(CheckIncludeFileCXX)
CHECK_INCLUDE_FILE_CXX(sstream HAVE_SSTREAM)
CHECK_INCLUDE_FILE_CXX(strstream HAVE_STRSTREAM)


# include special traget to remove cache, system and compiler info
IF(CMAKE_GENERATOR MATCHES "^Unix Makefiles")
  INCLUDE(CLEAN_CMAKE)
  INCLUDE(CVS_Targets)
  INCLUDE(BuildTypeTargets)
ENDIF(CMAKE_GENERATOR MATCHES "^Unix Makefiles")

INCLUDE(CREATE_CVSTAGS_MACRO)
# create cvstags for current version
CREATE_CVSTAG_TARGET("libEasdif_${EASDIF_VERSION_MAJ}_${EASDIF_VERSION_MIN}_${EASDIF_VERSION_RELEASE}")


ADD_SUBDIRECTORY(SDIF)


FILE(READ ${CMAKE_CURRENT_SOURCE_DIR}/easdif/easdif_config.hpp.in CONFIG)
STRING(REGEX REPLACE "#undef[ \t]*\([^ \t\n\r]*\)" "#cmakedefine \\1 @\\1@" EASDIFCMCONFIG ${CONFIG})

STRING(CONFIGURE ${EASDIFCMCONFIG} EASDIFCMCONFIGURED)
IF(EXISTS  ${CMAKE_CURRENT_BINARY_DIR}/easdif/easdif_config.hpp)
  FILE(READ ${CMAKE_CURRENT_BINARY_DIR}/easdif/easdif_config.hpp CURRENTCONFIG)
ENDIF(EXISTS  ${CMAKE_CURRENT_BINARY_DIR}/easdif/easdif_config.hpp)

IF(NOT "${CURRENTCONFIG}" STREQUAL "${EASDIFCMCONFIGURED}")
  MESSAGE("Writing Easdif config")
  FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/easdif/easdif_config.hpp ${EASDIFCMCONFIGURED})
ENDIF(NOT "${CURRENTCONFIG}" STREQUAL "${EASDIFCMCONFIGURED}")

SET(VERSION "${EASDIF_VERSION_MAJ}.${EASDIF_VERSION_MIN}.${EASDIF_VERSION_RELEASE}")
CONFIGURE_FILE(easdif/easdif_version.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/easdif/easdif_version.hpp)
CONFIGURE_FILE(easdif/easdifintro.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/easdif/easdifintro.hpp)
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

ADD_DEFINITIONS("-DHAVE_CONFIG_H")

ADD_CUSTOM_TARGET(lib)
ADD_DEPENDENCIES(lib Easdif${DEBUGEXT})

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/SDIF/sdifconfig ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/SDIF/include ${CMAKE_CURRENT_SOURCE_DIR}  )

ADD_SUBDIRECTORY( easdif ) 
ADD_SUBDIRECTORY( test ) 
OPTION(DOSWIG OFF)
IF(DOSWIG)
ADD_SUBDIRECTORY( swig EXCLUDE_FROM_ALL )
ENDIF(DOSWIG)

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


SET(prefix ${CMAKE_INSTALL_PREFIX})
SET(exec_prefix ${CMAKE_INSTALL_PREFIX})
SET(libdir ${CMAKE_INSTALL_PREFIX}/lib)
SET(includedir ${CMAKE_INSTALL_PREFIX}/include)
SET(EASDIFBASENAME Easdif${DEBUGEXT})
CONFIGURE_FILE(Easdif.pc.in   Easdif${DEBUGEXT}.pc )


# # Get PKGCONFIGDIR path relative too CMAKE_INSTALL_PREFIX
# GET_FILENAME_COMPONENT(abspref  ${CMAKE_INSTALL_PREFIX} ABSOLUTE)
# STRING(REPLACE "${abspref}"  "" PKGCONFIG_REL ${abspkg} )
# #if we have complete path that is outside of CMAKE_INSTALL_PREFIX
# IF(PKGCONFIG_REL STREQUAL PKGCONFIGDIR)
# SET(remainingpath ${CMAKE_INSTALL_PREFIX})
# MESSAGE("int remains ${remainingpath}")
# SET(torelative)
# WHILE(remainingpath)
# MESSAGE("remains ${remainingpath}")
# MESSAGE("last ${lastpath}")
# GET_FILENAME_COMPONENT(lastpath ${remainingpath} NAME)
# GET_FILENAME_COMPONENT(remainingpath ${remainingpath} PATH)
# IF(lastpath)
# SET(torelative "${torelative}../")
# ENDIF(lastpath)
# ENDWHILE(remainingpath)
# MESSAGE("torelative: ${torelative}")
# SET( PKGCONFIG_REL /${torelative}${abspkg} )
# ENDIF(PKGCONFIG_REL STREQUAL PKGCONFIGDIR)
# MESSAGE("pkgconfig      : ${PKGCONFIG_REL}")

INSTALL(FILES  ${CMAKE_CURRENT_BINARY_DIR}/Easdif${DEBUGEXT}.pc DESTINATION ${PKGCONFIGPATH} )

# install sdif headers
INSTALL(FILES  ${CMAKE_CURRENT_BINARY_DIR}/sdifconfig/sdif_version.h DESTINATION ${INSTALL_INC_DIR})
INSTALL(FILES  ${CMAKE_CURRENT_SOURCE_DIR}/SDIF/include/sdif.h DESTINATION ${INSTALL_INC_DIR})

# install configured easdif headers.
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/easdif/easdif_version.hpp DESTINATION ${INSTALL_INC_DIR}/easdif)

GET_DIRECTORY_PROPERTY(easdifDOCFILES DIRECTORY easdif DEFINITION srcDOCFILES)
SET(DOCFILES ${easdifDOCFILES} CACHE INTERNAL "files containing doc sources" FORCE)
MESSAGE("doc files : ${DOCFILES}")

# Documentation generation with Doxygen
ADD_CUSTOM_TARGET(doc COMMAND  doxygen  ${CMAKE_BINARY_DIR}/Doxyfile DEPENDS CMakeLists.txt ${DOCFILES})
ADD_CUSTOM_TARGET(install_doc  
  COMMAND chmod -R  g+w doc/html 
  COMMAND ncftpput -u $ENV{USER} ${DOCSERVER} ${DOCDIR} doc/html/* 
  COMMAND chmod -R  g-w doc/html)
ADD_DEPENDENCIES(install_doc doc)



SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "EaSDIF, the high level API to SDIF data")
SET(CPACK_PACKAGE_VENDOR "IRCAM")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
SET(CPACK_PACKAGE_VERSION_MAJOR "${EASDIF_VERSION_MAJ}")
SET(CPACK_PACKAGE_VERSION_MINOR "${EASDIF_VERSION_MIN}")
SET(CPACK_PACKAGE_VERSION_PATCH "${EASDIF_VERSION_RELEASE}")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME
    "Easdif-${EASDIF_VERSION_MAJ}.${EASDIF_VERSION_MIN}.${EASDIF_VERSION_RELEASE}")
SET(CPACK_PACKAGE_FILE_NAME)
INCLUDE(CPack)
