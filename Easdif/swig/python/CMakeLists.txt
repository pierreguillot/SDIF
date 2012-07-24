FIND_PROGRAM(PYTHON python)
MESSAGE(STATUS "python executable  ${PYTHON}")
IF(PYTHON)
  EXECUTE_PROCESS(COMMAND ${PYTHON}-config  --ldflags OUTPUT_VARIABLE out ERROR_VARIABLE err OUTPUT_STRIP_TRAILING_WHITESPACE)
  SET(PYTHON_LIBRARY ${out})
  EXECUTE_PROCESS(COMMAND ${PYTHON}-config  --includes OUTPUT_VARIABLE out ERROR_VARIABLE err OUTPUT_STRIP_TRAILING_WHITESPACE)
  STRING(REGEX REPLACE "[ \t]+" ";" PYTHON_INCLUDE_PATH ${out})
  MESSAGE(STATUS "Python includes <${PYTHON_INCLUDE_PATH}>")
  LIST(REMOVE_DUPLICATES PYTHON_INCLUDE_PATH)
  IF(DEBUG)
    MESSAGE(STATUS "Python library ${PYTHON_LIBRARY}")
    MESSAGE(STATUS "Python includes <${PYTHON_INCLUDE_PATH}>")
  ENDIF(DEBUG)
  FOREACH(PP ${PYTHON_INCLUDE_PATH})
    IF(DEBUG)
      MESSAGE("pp ${PP}")
    ENDIF(DEBUG)
    STRING(REGEX REPLACE "-I" "" PPP ${PP})
    INCLUDE_DIRECTORIES("${PPP}")
  ENDFOREACH(PP ${PYTHON_INCLUDE_PATH})

  EXECUTE_PROCESS(COMMAND ${PYTHON}  --version OUTPUT_VARIABLE out ERROR_VARIABLE err OUTPUT_STRIP_TRAILING_WHITESPACE)
  STRING(REGEX REPLACE "[a-zA-Z0-9 \t]* \([0-9]+[.][0-9]+\).*"  "\\1" PYTHON_VERSION ${err})
  MESSAGE(STATUS "PYTHON_VERSION ${PYTHON_VERSION}")
  SET(BUILD_WITH_INSTALL_RPATH ON)
  
  SET(INTERFACE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/easdif-python.i)
  
  SET_SOURCE_FILES_PROPERTIES(${INTERFACE_SRC} PROPERTIES CPLUSPLUS ON)
  
  SET(CMAKE_SWIG_FLAGS "-I${CMAKE_CURRENT_BINARY_DIR}" "-I${CMAKE_CURRENT_SOURCE_DIR}")
  SWIG_ADD_MODULE(eaSDIF python  ${INTERFACE_SRC}     )
  #  SWIG_LINK_LIBRARIES(eaSDIF  Easdif_static_pic${DEBUGEXT}  ${PYTHON_LIBRARY})
  # PYTHON LIBRARY seems not to be required because it is in fact part of python executable
# linking with libpython.a fails for some compilers on linux 64 bit due to the fact that the librray is not compiled with -fPIC flag.
  # as well on mac os x linking with _pic library fails.
  #  no idea why. Keep non pic version for now. 
  IF(APPLE)
    SWIG_LINK_LIBRARIES(eaSDIF  Easdif_static_pic${DEBUGEXT}  ${PYTHON_LIBRARY})
  ELSE(APPLE)
    SWIG_LINK_LIBRARIES(eaSDIF  Easdif_static_pic${DEBUGEXT})
  ENDIF(APPLE)
  ADD_CUSTOM_TARGET(pythonswig)
  ADD_DEPENDENCIES(pythonswig ${SWIG_MODULE_eaSDIF_REAL_NAME})
  CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/test.py.in ${CMAKE_CURRENT_BINARY_DIR}/test.py)

  CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/example.py ${CMAKE_CURRENT_BINARY_DIR}/example.py)
  
  CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in ${CMAKE_CURRENT_BINARY_DIR}/setup.py)


  IF(APPLE)
    SET(LOADPATH DYLD_LIBRARY_PATH)
  ELSE (APPLE)
    IF(WIN32)
      SET(LOADPATH PATH)
    ELSE(WIN32)
      IF ( "${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
        SET(LOADPATH LD_LIBRARY_PATH)
      ENDIF( "${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
      IF ( "${CMAKE_SYSTEM_NAME}" MATCHES "FreeBSD")
        SET(LOADPATH DYLD_LIBRARY_PATH)
      ENDIF( "${CMAKE_SYSTEM_NAME}" MATCHES "FreeBSD")
    ENDIF(WIN32)
  ENDIF(APPLE)
  
  ADD_CUSTOM_TARGET(checkswigpython
    COMMAND echo Testing python swig interface
    COMMAND ${LOADPATH}=$(CMAKE_BINARY_DIR)/lib ${PYTHON} ${CMAKE_CURRENT_BINARY_DIR}/test.py)
  ADD_DEPENDENCIES(checkswigpython pythonswig)
  ADD_DEPENDENCIES(checkswig checkswigpython)

  ADD_CUSTOM_TARGET(install_python_module_locally
    COMMENT install python swig interface into userbase
    COMMAND ${PYTHON} setup.py install --user
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
  ADD_DEPENDENCIES(install_python_module_locally pythonswig)
  ADD_CUSTOM_TARGET(install_python_module_globally    
    COMMENT install python swig interface into userbase
    COMMAND ${PYTHON} setup.py install
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

  ADD_DEPENDENCIES(install_python_module_globally pythonswig)
ENDIF(PYTHON) 

