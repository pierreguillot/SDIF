# set defaults for install prefix and build type
INCLUDE(REPLACE_DEFAULT_VALUE_MACRO)
REPLACE_DEFAULT_VALUE(CMAKE_BUILD_TYPE  STRING release 0 "set build type, debug or release" )
REPLACE_DEFAULT_VALUE(CMAKE_INSTALL_PREFIX PATH /u/formes/share 0 "install prefix" )
MESSAGE("Configure for mode = ${CMAKE_BUILD_TYPE}")
