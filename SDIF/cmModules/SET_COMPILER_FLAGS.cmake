# Set default compiler flags for all build types
#

# these are default values, they can be overriden before including
# SET_COMPILER_FLAGS or on the command line


IF(NOT COMPILER_FLAGS_INIT)
  INCLUDE(GET_ARCH)
  IF(DEBUG)
    MESSAGE(STATUS "Set compiler flags ")
    MESSAGE("ARCH ${ARCH}")
    MESSAGE("CMAKE_SYSTEM_PROCESSOR ${CMAKE_SYSTEM_PROCESSOR}")
    MESSAGE("CMAKE_HOST_SYSTEM_PROCESSOR ${CMAKE_HOST_SYSTEM_PROCESSOR}")
  ENDIF(DEBUG)
  INCLUDE(ADD_COMPILER_FLAGS)
  ADD_COMPILER_FLAGS(${EXTRA_CFLAGS})
  # set mode dependend compiler flags
  ADD_BT_COMPILER_FLAGS(DEBUG -DCHECK -DDEBUG)
  # unfortunately this does work only in mastersvp
  #  because  all libraries need to be compiled
  #  with the STL debug flag as well.
  # ADD_BT_COMPILER_FLAGS(DEBUG  -D_GLIBCXX_DEBUG )
  ADD_BT_COMPILER_FLAGS(RELEASE -DNDEBUG)
  ADD_BT_COMPILER_FLAGS(PROFILE -DPROFILE)
  ADD_BT_COMPILER_FLAGS(RELWITHDEBINFO -DRELWITHDEBINFO)
  # due to a bug in cmake 2.4.3 GNUCXX is not set in cygwin (and mingw?)
  IF(CMAKE_COMPILER_IS_GNUCC)
    # GET_ARCH should map either CMAKE_OSX_ARCHITECTURES or CMAKE_SYSTEM_PROCESSOR to the variable ARCH
    # for apple the architecture flags are generated accordingly by default
    IF(NOT APPLE)
      IF("${ARCH}" STREQUAL "i386" OR "${ARCH}" STREQUAL "i686")
        SET(ARCH_IS_32BIT 1)
      ELSE("${ARCH}" STREQUAL "i386" OR "${ARCH}" STREQUAL "i686")
        SET(ARCH_IS_32BIT 0)
      ENDIF("${ARCH}" STREQUAL "i386" OR "${ARCH}" STREQUAL "i686")
      IF(${ARCH_IS_32BIT} AND "${CMAKE_HOST_SYSTEM_PROCESSOR}" STREQUAL "x86_64")
        ADD_COMPILER_FLAGS(-m32)
	ADD_LINK_FLAGS_ALL_TARGETS(-m32)
      ENDIF(${ARCH_IS_32BIT} AND "${CMAKE_HOST_SYSTEM_PROCESSOR}" STREQUAL "x86_64")
    ENDIF(NOT APPLE)
    IF(NOT WIN32)
      INCLUDE( CheckCXXCompilerFlag)      
      CHECK_CXX_COMPILER_FLAG("-fvisibility=hidden" GCC_HAS_VISIBILITY)
      IF(GCC_HAS_VISIBILITY)
        ADD_COMPILER_FLAGS(-DGCC_HAS_VISIBILITY -fvisibility=hidden)
      ENDIF(GCC_HAS_VISIBILITY)
    ENDIF(NOT WIN32)
    INCLUDE(GET_GCC_VERSION)
    ADD_BT_COMPILER_FLAGS(DEBUG -Wall -Wno-switch -Wno-unused-function -O0 -fno-inline )
    ADD_BT_COMPILER_CXXFLAGS(DEBUG -fno-default-inline)
    ADD_BT_COMPILER_FLAGS(RELEASE -funroll-loops -Wall -Wno-switch -Wno-unused-function)
    ADD_BT_COMPILER_FLAGS(RELWITHDEBINFO -funroll-loops  -Wall -Wno-switch -Wno-unused-function -fno-inline)
    ADD_BT_COMPILER_CXXFLAGS(RELWITHDEBINFO -fno-default-inline)
    IF (GCCVERSIONMAJ EQUAL 3)
      IF (GCCVERSIONMIN EQUAL 2)
        # 3.2 is horribly slow in inline-ing for large source files
        ADD_BT_COMPILER_FLAGS(RELEASE -finline-limit=1000 )
      ELSE (GCCVERSIONMIN EQUAL 2)
        ADD_BT_COMPILER_FLAGS(RELEASE -finline-limit=5000 )
      ENDIF (GCCVERSIONMIN EQUAL 2)
    ELSE(GCCVERSIONMAJ EQUAL 3)
      IF (GCCVERSIONMAJ EQUAL 4)
        ADD_BT_COMPILER_FLAGS(RELEASE -finline-limit=5000 --param large-function-insns=5000 --param large-function-growth=500 --param inline-unit-growth=100)
        ADD_BT_COMPILER_FLAGS(RELWITHDEBINFO -finline-limit=5000 --param large-function-insns=5000 --param large-function-growth=500 --param inline-unit-growth=100)
        #ADD_BT_COMPILER_FLAGS(RELEASE -finline-limit=5000 --param large-function-insns=5000 --param large-function-growth=400)
        #ADD_BT_COMPILER_FLAGS(RELWITHDEBINFO -finline-limit=5000 --param large-function-insns=5000 --param large-function-growth=400)
        IF (GCCVERSIONMIN EQUAL 1)
          ADD_BT_COMPILER_FLAGS(RELEASE --param salias-max-implicit-fields=50)
          ADD_BT_COMPILER_FLAGS(RELWITHDEBINFO --param salias-max-implicit-fields=50)
        ENDIF (GCCVERSIONMIN EQUAL 1)
        IF (GCCVERSIONMIN GREATER 2)
          ADD_BT_COMPILER_FLAGS(RELEASE -Wstrict-overflow=0)
          ADD_BT_COMPILER_FLAGS(RELWITHDEBINFO -Wstrict-overflow=0)
        ENDIF (GCCVERSIONMIN GREATER 2)
        IF(${ARCH}  STREQUAL "x86_64")
          ADD_COMPILER_FLAGS(-fno-strict-aliasing)
        ELSE(${ARCH}  STREQUAL "x86_64")
          ADD_COMPILER_FLAGS(-fstrict-aliasing)
        ENDIF(${ARCH}  STREQUAL "x86_64")
      ENDIF(GCCVERSIONMAJ EQUAL 4)
    ENDIF (GCCVERSIONMAJ EQUAL 3)    
  ENDIF(CMAKE_COMPILER_IS_GNUCC)

  IF (APPLE)
     IF("${ARCH}" MATCHES ^p.*)
    #   ADD_ARCH_FLAGS(ppc970 -mcpu=970 -mpowerpc64 -mpowerpc-gpopt -mtune=970  -falign-loops=32 -falign-jumps=32 )
    #   ADD_ARCH_FLAGS(ppc7450 -mcpu=7450 -mpowerpc -mtune=7450 )
    #   ADD_ARCH_FLAGS(ppc -mpowerpc )
    #   ADD_ARCH_FLAGS(powerpc -mpowerpc )
     ELSE("${ARCH}" MATCHES ^p.*)
      ADD_COMPILER_FLAGS( -maccumulate-outgoing-args  )
    ENDIF("${ARCH}" MATCHES ^p.*)
  ENDIF(APPLE)

  IF ( "${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
    MESSAGE(STATUS "Configuring Linux")
    ADD_COMPILER_FLAGS(-maccumulate-outgoing-args  ) 
  ENDIF ("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
  IF (WIN32)
    IF(CMAKE_COMPILER_IS_CYGWIN OR CMAKE_COMPILER_IS_MINGW)
      IF(CMAKE_COMPILER_IS_CYGWIN)
        IF(NOT DEFINED BUILD_MINGW)
          OPTION(BUILD_MINGW "Build MingW Version" ON)
        ENDIF(NOT DEFINED BUILD_MINGW)
      ELSE(CMAKE_COMPILER_IS_CYGWIN)
        SET(BUILD_MINGW OFF)
      ENDIF(CMAKE_COMPILER_IS_CYGWIN)
      IF(BUILD_MINGW)
        ADD_COMPILER_FLAGS(-mno-cygwin )
        #SET(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -mno-cygwin")
        #SET(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -mno-cygwin")
        LINK_LIBRARIES(-mno-cygwin)
        LIST(APPEND CMAKE_REQUIRED_DEFINITIONS -mno-cygwin)
        LIST(APPEND CMAKE_REQUIRED_FLAGS -mno-cygwin)
      ENDIF(BUILD_MINGW)
      ADD_COMPILER_FLAGS(-maccumulate-outgoing-args  ) 
    ENDIF(CMAKE_COMPILER_IS_CYGWIN OR CMAKE_COMPILER_IS_MINGW)
    IF(MSVC)
      # for visual c++ 2005
      ADD_COMPILER_FLAGS(-D_CRT_SECURE_NO_DEPRECATE=1 -D_SCL_SECURE_NO_WARNINGS=1 -D_SECURE_SCL=0)
      
      ADD_BT_COMPILER_FLAGS(RELEASE /Oi /Ot /Oy /GS-)
      ADD_BT_COMPILER_FLAGS(RELWITHDEBINFO /Oi /Ot /Oy /GS-)
    ENDIF(MSVC)
    IF(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
      # link statically
      ADD_LINK_FLAGS_ALL_TARGETS(/INCREMENTAL:no /MANIFEST)
      # link static run time for executables
      # 
      #      ADD_BT_LINK_FLAGS(EXE RELEASE /MT)
      #      ADD_BT_LINK_FLAGS(EXE RELWITHDEBINFO /MT)

      ADD_COMPILER_FLAGS(-D_CRT_SECURE_NO_DEPRECATE=1 -D_SCL_SECURE_NO_WARNINGS=1 -D_SECURE_SCL=0 )

 
      # use statically linked runtime in all cases
      IF(NOT FORCE_DYNAMIC_MS_RUNTIME)
        #RELEASE
        STRING(REPLACE "/MD" "/MT" TT_STBF "${CMAKE_C_FLAGS_RELEASE}"  )
        SET(CMAKE_C_FLAGS_RELEASE "${TT_STBF}")
        STRING(REPLACE "/MD" "/MT" TT_STBF "${CMAKE_CXX_FLAGS_RELEASE}"  )
        SET(CMAKE_CXX_FLAGS_RELEASE "${TT_STBF}")

        #RELWITHDEBINFO
        STRING(REPLACE "/MD" "MT" TT_STBF "${CMAKE_C_FLAGS_RELWITHDEBINFO}"  )
        SET(CMAKE_C_FLAGS_RELWITHDEBINFO "${TT_STBF}")
        STRING(REPLACE "/MD" "/MT" TT_STBF "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}"  )
        SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${TT_STBF}")

        #DEBUG
        STRING(REPLACE "/MD" "/MT" TT_STBF "${CMAKE_CXX_FLAGS_DEBUG}"  )
        SET(CMAKE_CXX_FLAGS_DEBUG "${TT_STBF}")
        STRING(REPLACE "/MD" "/MT" TT_STBF "${CMAKE_C_FLAGS_DEBUG}"  )
        SET(CMAKE_C_FLAGS_DEBUG "${TT_STBF}")
      ENDIF(NOT FORCE_DYNAMIC_MS_RUNTIME)
      IF(CMAKE_SYSTEM_PROCESSOR STREQUAL x86_64 )
        ADD_BT_COMPILER_FLAGS(RELWITHDEBINFO /O3 /Oi /Ot  /GS- /Qinline-factor=10)      
        ADD_BT_COMPILER_FLAGS(RELEASE        /O3 /Oi /Ot  /GS- /Qinline-factor=10)
      ELSE(CMAKE_SYSTEM_PROCESSOR STREQUAL x86_64 )
        ADD_BT_COMPILER_FLAGS(RELWITHDEBINFO /O3 /Oi /Ot /Oy /GS- /Qinline-factor=10)      
        ADD_BT_COMPILER_FLAGS(RELEASE        /O3 /Oi /Ot /Oy /GS- /Qinline-factor=10)
      ENDIF(CMAKE_SYSTEM_PROCESSOR STREQUAL x86_64 )

      # remove /O2 to avoid confusion with /O3 that is set here above
      STRING(REPLACE "/O2" "" TT_STBF "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}"  )
      SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${TT_STBF}")
      STRING(REPLACE "/O2" "" TT_STBF "${CMAKE_C_FLAGS_RELWITHDEBINFO}"  )
      SET(CMAKE_C_FLAGS_RELWITHDEBINFO "${TT_STBF}")
      STRING(REPLACE "/O2" "" TT_STBF "${CMAKE_CXX_FLAGS_RELEASE}"  )
      SET(CMAKE_CXX_FLAGS_RELEASE "${TT_STBF}")
      STRING(REPLACE "/O2" "" TT_STBF "${CMAKE_C_FLAGS_RELEASE}"  )
      SET(CMAKE_C_FLAGS_RELEASE "${TT_STBF}")

      # force no inlining for debug version
      ADD_BT_LINK_FLAGS(DEBUG /Qip-no-inlining /debug:all)
      ADD_BT_COMPILER_FLAGS(DEBUG /Qip-no-inlining /debug:all)

    ENDIF(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")

  ENDIF(WIN32)

  SET(COMPILER_FLAGS_INIT 1)

ENDIF(NOT COMPILER_FLAGS_INIT)

IF(DEBUG)
  PRINT_COMPILER_FLAGS()
ENDIF(DEBUG)
