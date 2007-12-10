IF(APPLE)
  
  OPTION(DOUNIVERSAL "configure for cross compilation producing universal" OFF )
  IF(CMAKE_OSX_SYSROOT AND DOUNIVERSAL)
    IF(_CMAKE_OSX_MACHINE)
      IF("${_CMAKE_OSX_MACHINE}" STREQUAL "ppc")
        SET(CMAKE_OSX_ARCHITECTURES "ppc;i386" CACHE STRING "Build architectures for OSX" FORCE)
      ELSE("${_CMAKE_OSX_MACHINE}" STREQUAL "ppc")
        SET(CMAKE_OSX_ARCHITECTURES "i386;ppc" CACHE STRING "Build architectures for OSX" FORCE)
      ENDIF("${_CMAKE_OSX_MACHINE}" STREQUAL "ppc")
    ELSE(_CMAKE_OSX_MACHINE)
      SET( CMAKE_OSX_ARCHITECTURES  "ppc;i386" CACHE STRING "Build architectures for OSX" FORCE)
    ENDIF(_CMAKE_OSX_MACHINE)
  ELSE(CMAKE_OSX_SYSROOT AND DOUNIVERSAL)
    IF(DOUNIVERSAL)
      MESSAGE(FATAL_ERROR "Universal binaries can only created if the SDK MacOSX10.4u.sdk is installed")
    ENDIF(DOUNIVERSAL)
    IF ("${ARCH}" MATCHES "^p.*pc.*")
      FIND_PATH (SDK MacOSX10.3.9.sdk /Developer/SDKs DOC "Path to 10.3.9 SDK") 
      SET(MACOSX_DEPLOYMENT_TARGET 10.3 CACHE STRING "min macosx version to run on")
      IF (SDK)
        ADD_DEFINITIONS (-isysroot ${SDK}/MacOSX10.3.9.sdk)
      ENDIF (SDK)
    ELSE ("${ARCH}" MATCHES "^p.*pc.*" )
      FIND_PATH (SDK MacOSX10.4u.sdk /Developer/SDKs DOC "Path to 10.4 SDK") 
      SET(MACOSX_DEPLOYMENT_TARGET 10.4 CACHE STRING "min macosx version to run on")
      IF(SDK)
        ADD_DEFINITIONS (-isysroot ${SDK}/MacOSX10.4u.sdk )
      ENDIF (SDK)    
    ENDIF("${ARCH}" MATCHES "^p.*pc.*" )
  ENDIF(CMAKE_OSX_SYSROOT AND DOUNIVERSAL)
  # not needed because Easdif/SDIF does only use BSD
  #IF(MACOSX_DEPLOYMENT_TARGET)
  #  IF(GGCVERSIONMAJ GREATER 3)
  #    ADD_DEFINITIONS (-mmacosx-version-min=${MACOSX_DEPLOYMENT_TARGET} )  
  #  ENDIF(GGCVERSIONMAJ GREATER 3)
  #ENDIF(MACOSX_DEPLOYMENT_TARGET)
ENDIF(APPLE)
