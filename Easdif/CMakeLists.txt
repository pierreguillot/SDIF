#
# cmake configuration file
#
# This file is part of the IRCAM EaSDIF Library (http://www.ircam.fr/sdif)
#
# Copyright (C) 2006 by IRCAM - Centre Pompidou, Paris, France.
#
# created by Axel Roebel                     7.12.2006
#
# This file computes/replaces some default values:
# CMAKE_BUILD_TYPE     : default value is "release" other selectable value is "debug"
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
#  BUILD_MINGW:BOOL=ON/OFF  if cygwin add -mno-cygwin to compiler commands

PROJECT(EaSDIF)

SET(EASDIF_VERSION_MAJ 1)
SET(EASDIF_VERSION_MIN 3)
SET(EASDIF_VERSION_RELEASE 0)
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

INCLUDE(GET_ARCH)

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

FIND_PROGRAM(HAVEDOT_EXECUTABLE NAMES dot )
IF(HAVEDOT_EXECUTABLE)
  SET(HAVEDOT YES)
ELSE(HAVEDOT_EXECUTABLE)
  SET(HAVEDOT NO)
ENDIF(HAVEDOT_EXECUTABLE)
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

INCLUDE(CheckIncludeFileCXX)
CHECK_INCLUDE_FILE_CXX(sstream HAVE_SSTREAM)
CHECK_INCLUDE_FILE_CXX(strstream HAVE_STRSTREAM)

SET(top_srcdir ${CMAKE_CURRENT_SOURCE_DIR})
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)


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


ADD_DEFINITIONS("-DHAVE_CONFIG_H")


INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/SDIF/sdifconfig ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/SDIF/include ${CMAKE_CURRENT_SOURCE_DIR}  )

ADD_SUBDIRECTORY( easdif ) 
ADD_SUBDIRECTORY( test ) 

ADD_CUSTOM_TARGET(lib)
ADD_DEPENDENCIES(lib Easdif${DEBUGEXT})


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



SET(prefix ${CMAKE_INSTALL_PREFIX})
SET(exec_prefix ${CMAKE_INSTALL_PREFIX})
SET(libdir ${CMAKE_INSTALL_PREFIX}/lib)
SET(includedir ${CMAKE_INSTALL_PREFIX}/include)
SET(EASDIFBASENAME Easdif${DEBUGEXT})
CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/Easdif.pc.in   ${PROJECT_BINARY_DIR}/Easdif${DEBUGEXT}.pc )

OPTION(DOSWIG OFF)
IF(DOSWIG)
IF(CMAKE_GENERATOR MATCHES "^Unix Makefiles")
  ADD_SUBDIRECTORY( swig EXCLUDE_FROM_ALL )
ELSE(CMAKE_GENERATOR MATCHES "^Unix Makefiles")
  ADD_SUBDIRECTORY( swig )
ENDIF(CMAKE_GENERATOR MATCHES "^Unix Makefiles")
ENDIF(DOSWIG)


INSTALL(FILES  ${PROJECT_BINARY_DIR}/Easdif${DEBUGEXT}.pc DESTINATION ${PKGCONFIGPATH} )


GET_DIRECTORY_PROPERTY(easdifDOCFILES DIRECTORY easdif DEFINITION srcDOCFILES)
SET(DOCFILES ${easdifDOCFILES} CACHE INTERNAL "files containing doc sources" FORCE)
#MESSAGE("doc files : ${DOCFILES}")

# Documentation generation with Doxygen
ADD_CUSTOM_TARGET(doc COMMAND  doxygen  ${CMAKE_BINARY_DIR}/Doxyfile DEPENDS CMakeLists.txt;${DOCFILES})
IF(DEFINED DOCSERVER)
  FIND_PROGRAM(NCFTPPUT_EXECUTABLE ncftpput)
  IF(NCFTPPUT_EXECUTABLE)
    ADD_CUSTOM_TARGET(install_doc  
      COMMAND chmod -R  g+w doc/html 
      COMMAND ${NCFTPPUT_EXECUTABLE} -u $ENV{USER} ${DOCSERVER} ${DOCDIR} doc/html/* 
      COMMAND chmod -R  g-w doc/html)
    ADD_DEPENDENCIES(install_doc doc)
  ELSE(NCFTPPUT_EXECUTABLE)
    MESSAGE("++++++++++++++++++++++++")
    MESSAGE(STATUS "ncftpput not found! no install_doc target generated \n++++++++++++++++++++++++")
  ENDIF(NCFTPPUT_EXECUTABLE)
ENDIF(DEFINED DOCSERVER)

SET(DOCTAR Easdif-${EASDIF_VERSION}.doc.tgz)

ADD_CUSTOM_TARGET(pubdoc
COMMAND	cp -r doc   Easdif-${EASDIF_VERSION}.doc
COMMAND	tar -cvzf ${DOCTAR} Easdif-${EASDIF_VERSION}.doc
COMMAND	rm -rf Easdif-${EASDIF_VERSION}.doc)
ADD_DEPENDENCIES(pubdoc doc)



GET_FILENAME_COMPONENT(normalize_path ${CMAKE_CURRENT_SOURCE_DIR}/SDIF ABSOLUTE)
GET_DIRECTORY_PROPERTY(sdif_version DIRECTORY ${normalize_path} DEFINITION  SDIF_VERSION)

FILE(GLOB_RECURSE configure_dependencies_am ${CMAKE_CURRENT_SOURCE_DIR}/Makefile.am) 
FILE(GLOB_RECURSE configure_dependencies_sh ${CMAKE_CURRENT_SOURCE_DIR}/autogen.sh) 
FILE(GLOB_RECURSE configure_dependencies_in ${CMAKE_CURRENT_SOURCE_DIR}/configure.in) 
ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/configure COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/autogen.sh WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DEPENDS ${configure_dependencies_in} ${configure_dependencies_am} ${configure_dependencies_sh} )

ADD_CUSTOM_TARGET(dist cpack --config ${CMAKE_CURRENT_BINARY_DIR}/CPackSourceConfig.cmake DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/configure )

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "EaSDIF/SDIF libraries and tools for handling SDIF files")
SET(CPACK_PACKAGE_VENDOR "IRCAM")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
SET(CPACK_PACKAGE_VERSION_MAJOR "${EASDIF_VERSION_MAJ}")
SET(CPACK_PACKAGE_VERSION_MINOR "${EASDIF_VERSION_MIN}")
SET(CPACK_PACKAGE_VERSION_PATCH "${EASDIF_VERSION_RELEASE}")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME
    "Easdif-${EASDIF_VERSION}-SDIF-${sdif_version}-src")
SET(CPACK_PACKAGE_FILE_NAME)
SET(CPACK_GENERATOR "TGZ")
SET(CPACK_SOURCE_GENERATOR "TGZ")

SET(CPACK_SOURCE_IGNORE_FILES "/CVS/" "/\\\\.svn/" "\\\\.swp$" "\\\\.#" "/#" "~$"  "/build.*/" "/autom4te.cache/"   "/SDIF/others/" "/all.ircam.sh$" "/all.install$" "/SDIF/sdif/ReadFromMemory.txt$"  "/test/fall.sdif"   "/Makefile$" "sdif.sln$" "/SDIF/test/prof/" "/SDIF/test/.*src$"   "/SDIF/test/.*dsp$" "/SDIF/test/SdifTypes.STYP" "/macosxbuild/[^MS]$"  "/macosxbuild/.*plist$" "/macosxbuild/.*MacOSX.binary-distrib$" "/macosxbuild/S.*/[^Mp]" "/macosxbuild/SDIF.xcodeproj/" ".cvsignore$" "/test/ent.*cpp$" "/test/t[tryoe].*cpp$" "/SDIF/doc/pics/.*fig$" "/SDIF/doc/intern/"  "/SDIF/doc/standard/" "/SDIF/doc/index-sf.html" "/SDIF/doc/Makefile.doc" "/SDIF/doc/sdif.src" "/SDIF/doc/.*txt" "/SDIF/types/" "/SDIF/tools/main.nib/" "/SDIF/tools/.*sdef" "/SDIF/tools/.*cp$" "/SDIF/tools/.*h$" "/SDIF/tools/sdifusage.*$" "/SDIF/tools/.*vcproj$" "/SDIF/doc/download/" "compileandinstall$" "/sdifcpp/.*cpp" "/sdifcpp/.*h$" "/test/Ne*\\\\.sdif$"   "/test/tut[mn].*\\\\.cpp$"   "/test/es.*cpp$" "/test/[cmvnrs].*cpp$" )
#MESSAGE("CPACK_SOURCE_IGNORE_FILES  ${CPACK_SOURCE_IGNORE_FILES}")
INCLUDE(CPack)


MESSAGE("===================================================================")
MESSAGE("Option summary:")
MESSAGE("install prefix (CMAKE_INSTALL_PREFIX)         : ${CMAKE_INSTALL_PREFIX}")
MESSAGE("enable swig support (DOSWIG)                  : ${DOSWIG}")
MESSAGE("build static Easdif lib (EASDIF_BUILD_STATIC) : ${EASDIF_BUILD_STATIC}")
MESSAGE("build static sdif lib (SDIF_BUILD_STATIC)     : ${SDIF_BUILD_STATIC}")
MESSAGE("enable thread support (USE_THREADS)           : ${USE_THREADS}")
IF(APPLE)
  MESSAGE("enable universal (DOUNIVERSAL)                : ${DOUNIVERSAL}" )
  IF(CMAKE_OSX_SYSROOT)
    IF(DOUNIVERSAL)
      MESSAGE("Architectures                                 : ${CMAKE_OSX_ARCHITECTURES} ")
    ENDIF(DOUNIVERSAL)
  ENDIF(CMAKE_OSX_SYSROOT)
ENDIF(APPLE)

IF(CYGWIN)
  MESSAGE("Use mingw (BUILD_MINGW)                       : ${BUILD_MINGW}")
ENDIF(CYGWIN)

MESSAGE("Easdif installation summary:")
MESSAGE("bin            : ${CMAKE_INSTALL_PREFIX}/${INSTALL_BIN_DIR}")
MESSAGE("lib            : ${CMAKE_INSTALL_PREFIX}/${INSTALL_LIB_DIR}")
MESSAGE("includes       : ${CMAKE_INSTALL_PREFIX}/${INSTALL_INC_DIR}")
MESSAGE("pkgconfig      : ${PKGCONFIGPATH}")
MESSAGE("===================================================================")
