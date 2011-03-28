SET(EASDIF_MATLAB_VERSION_MAJ 1)
SET(EASDIF_MATLAB_VERSION_MIN 1)
SET(EASDIF_MATLAB_VERSION_REL 0)

MESSAGE(STATUS "-------------------------------------------------")
MESSAGE(STATUS "Configuring mex wrappers: version ${EASDIF_MATLAB_VERSION_MAJ}.${EASDIF_MATLAB_VERSION_MIN}.${EASDIF_MATLAB_VERSION_REL}" )

FIND_PROGRAM(MEX mex)
FIND_PROGRAM(MKOCT mkoctfile)

IF(NOT MEX AND APPLE)
  FILE(GLOB MATLAB_PATH /Applications/M*)
  IF(MATLAB_PATH)
    FOREACH(DIR ${MATLAB_PATH})
      IF(NOT MEX)
        IF(EXISTS  ${DIR}/bin/mex)
          SET(MEX ${DIR}/bin/mex)
        ENDIF(EXISTS  ${DIR}/bin/mex )
      ENDIF(NOT MEX)
    ENDFOREACH(DIR ${MATLAB_PATH})
  ENDIF(MATLAB_PATH)
ENDIF(NOT MEX AND APPLE)

IF(MEX) 
  MESSAGE(STATUS "matlab mex wrapper :: using mex -> ${MEX}")
  # Attention the line breaks are required for mac ppc
  SET(SOURCE "#include<mex.h> 
void mexFunction (int nlhs, mxArray *plhs [], int nrhs, const mxArray *prhs []){
}")
  CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/SDIF/cmModules/trycompile.c.in  ${CMAKE_CURRENT_BINARY_DIR}/cmake_tmp.cxx)
  EXECUTE_PROCESS(COMMAND ${MEX}  ${CMAKE_CURRENT_BINARY_DIR}/cmake_tmp.cxx -v RESULT_VARIABLE res OUTPUT_VARIABLE out ERROR_VARIABLE err)
  
  IF(res)
    MESSAGE(STATUS "mex command ${MEX} cannot be run, error ${res} '${err}' skipping mex\n    make sure this is not the mex program from the tex distribution.")
  ELSE (res)
    STRING(REGEX REPLACE ".*LDEXTENSION[ \t]*=[ \t]*\([^ \t\n\r]*).*" "\\1" MEXEXT ${out})
    IF( "${out}" MATCHES "-fexception" )
      SET(EXCEPTIONFLAG)
    ELSE( "${out}" MATCHES "-fexception" )
      SET(EXCEPTIONFLAG "-fno-exceptions" )
    ENDIF( "${out}" MATCHES "-fexception" )
    
    
    # GNU C++ Compiler
    IF(CMAKE_COMPILER_IS_GNUCXX)
      SET(SEL_CXXABI_FLAG -fabi-version=1 CACHE STRING "select C++ ABI")
      IF (GCCVERSIONMAJ EQUAL 2)
        ADD_DEFINITIONS( -D_ISOC99_SOURCE)
      ENDIF (GCCVERSIONMAJ EQUAL 2)        
    ENDIF(CMAKE_COMPILER_IS_GNUCXX)
    
    SET(COMPILER "\"CXX=${CMAKE_CXX_COMPILER}\"")
    
    
    SET(CMAKE_SHARED_LIBRARY_PREFIX)
    IF("${ARCH}" STREQUAL "x86_64" )
      SET(CMAKE_SHARED_LIBRARY_SUFFIX mexa64)
    ELSE ("${ARCH}" STREQUAL "x86_64" )
      SET(CMAKE_SHARED_LIBRARY_SUFFIX mexglx)
    ENDIF("${ARCH}" STREQUAL "x86_64" )
    
    IF(${CMAKE_BUILD_TYPE} MATCHES "debug")
      SET(MEXFLAGS -g)
      SET(COMPFLAGS CMAKE_CXX_FLAGS_DEBUG)
      SET(MEXFLAGBUILDTYPE CXXDEBUGFLAGS)
    ELSE(${CMAKE_BUILD_TYPE} MATCHES "debug")
      SET(MEXFLAGS -O)
      SET(COMPFLAGS CMAKE_CXX_FLAGS_RELEASE)
      SET(MEXFLAGBUILDTYPE CXXOPTIMFLAGS)
    ENDIF(${CMAKE_BUILD_TYPE} MATCHES "debug")
    
    STRING(REGEX REPLACE ".*[^L][^D]${MEXFLAGBUILDTYPE}[ \t]*=[ \t]*\([^\n\r]*\).*" "\\1" MEXORIFLAGS ${out})
    STRING(REGEX REPLACE ".*[^L][^D]LDFLAGS[ \t]*=[ \t]*\([^\n\r]*\).*" "\\1" MEXORILDFLAGS ${out})
    STRING(REGEX REPLACE ".*[^L][^D]LDCXXFLAGS[ \t]*=[ \t]*\([^\n\r]*\).*" "\\1" MEXORILDCXXFLAGS ${out})
    
    SET(CMAKE_CXX_FLAGS_RELEASE "CXXOPTIMFLAGS=${MEXORIFLAGS} ${CMAKE_CXX_ADD_FLAGS_RELEASE} ${CMAKE_SHARED_LIBRARY_CXX_FLAGS}")
    SET(CMAKE_CXX_FLAGS_DEBUG   "CXXDEBUGFLAGS=${MEXORIFLAGS} -O0 -fno-inline -fno-default-inline  ${CMAKE_CXX_ADD_FLAGS_DEBUG} ${CMAKE_SHARED_LIBRARY_CXX_FLAGS}")
    SET(CMAKE_CXX_LINK_FLAGS    "LDFLAGS=$(EXTRALDFLAGS)  ${MATMTL_LFLAGS} -shared -lstdc++" ${COMPILER})
    IF(DEBUG)
      MESSAGE(STATUS "CMAKE_SHARED_LIBRARY_C_FLAGS ${CMAKE_SHARED_LIBRARY_C_FLAGS}")
      MESSAGE(STATUS "CMAKE_SHARED_LIBRARY_CXX_FLAGS ${CMAKE_SHARED_LIBRARY_CXX_FLAGS}")
      MESSAGE(STATUS "ORI: ${MEXORIFLAGS}")
    ENDIF(DEBUG)
    SET(CMAKE_SHARED_LIBRARY_C_FLAGS)
    SET(CMAKE_SHARED_LIBRARY_CXX_FLAGS)


    SET(ADDCFLAGS "-DEASDIF_MATLAB_VERSION=${EASDIF_MATLAB_VERSION_MAJ}.${EASDIF_MATLAB_VERSION_MIN}.${EASDIF_MATLAB_VERSION_REL} -DEASDIF_IS_STATIC")

    IF(GCC_HAS_VISIBILITY)
      SET(ADDCFLAGS "${ADDCFLAGS} -DGCC_HAS_VISIBILITY -fvisibility=hidden")
    ENDIF(GCC_HAS_VISIBILITY)

    ADD_CUSTOM_TARGET(mex)      
    SET(INSTALL_MEX_FILES )
    MACRO(MEXCOMPILE_FILES )
      FOREACH(_current_FILE ${ARGN})
        GET_FILENAME_COMPONENT(_tmp_FILE ${_current_FILE} ABSOLUTE)
        GET_FILENAME_COMPONENT(_abs_PATH ${_tmp_FILE} PATH)
        GET_FILENAME_COMPONENT(_basename ${_tmp_FILE} NAME_WE)
      
        SET(_o_FILE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${_basename}${DEBUGEXT}${MEXEXT})
        #          MESSAGE(STATUS "_current_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${_current_FILE} _o_FILE ${_o_FILE}")
        SET(INSTALL_MEX_FILES ${INSTALL_MEX_FILES} ${_o_FILE})
        ADD_CUSTOM_COMMAND(OUTPUT ${_o_FILE} 
          COMMAND echo "Processing: ${_current_FILE}"
          COMMAND
          ${MEX} ${MEXFLAGS} -v ${CMAKE_CURRENT_SOURCE_DIR}/${_current_FILE} "CXX=${CMAKE_CXX_COMPILER}" "${${COMPFLAGS}} ${ADDCFLAGS}"  "LDFLAGS=${MEXORILDFLAGS}" "LDCXXFLAGS=${MEXORILDCXXFLAGS}" -I${CMAKE_CURRENT_BINARY_DIR}/../SDIF/sdifconfig -I${CMAKE_CURRENT_BINARY_DIR}/.. -I${CMAKE_CURRENT_SOURCE_DIR}/../SDIF/include -I${CMAKE_CURRENT_SOURCE_DIR}/.. -output ${_o_FILE} -L${CMAKE_ARCHIVE_OUTPUT_DIRECTORY} -lEasdif_static_pic${DEBUGEXT}
          DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${_current_FILE}  ${DEP_INCLUDES} Easdif_static_pic${DEBUGEXT}) 
        
        ADD_CUSTOM_TARGET(${_basename}${DEBUGEXT}  DEPENDS  ${_o_FILE})      
        #ADD_DEPENDENCIES(${_basename}  ${_o_FILE} )
        ADD_DEPENDENCIES(mex  ${_basename}${DEBUGEXT} )
      ENDFOREACH(_current_FILE)
    ENDMACRO(MEXCOMPILE_FILES )
    
    SET(SOURCES Fsdif_read_handler.cpp Fsdif_write_handler.cpp)
    
    MEXCOMPILE_FILES(${SOURCES})
    
    SET(MFILES ${CMAKE_CURRENT_SOURCE_DIR}/Fsdifclose.m 
      ${CMAKE_CURRENT_SOURCE_DIR}/Fsdifopen.m 
      ${CMAKE_CURRENT_SOURCE_DIR}/Fsdifwrite.m 
      ${CMAKE_CURRENT_SOURCE_DIR}/Fsdifread.m 
      ${CMAKE_CURRENT_SOURCE_DIR}/Fsdifexist.m 
      ${CMAKE_CURRENT_SOURCE_DIR}/Fsdifclearall.m 
      ${CMAKE_CURRENT_SOURCE_DIR}/Fsdifrewind.m 
      ${CMAKE_CURRENT_SOURCE_DIR}/Fsdifloadfile.m)
    
    INSTALL(FILES ${INSTALL_MEX_FILES} ${MFILES} DESTINATION ${INSTALL_LIB_DIR}/matlab/ )
  ENDIF(res)

ENDIF(MEX)


IF(MKOCT) 

  MESSAGE(STATUS "octave mex wrapper :: using mkoctfile -> ${MKOCT}")
  EXECUTE_PROCESS(COMMAND ${MKOCT} -v RESULT_VARIABLE res OUTPUT_VARIABLE out ERROR_VARIABLE err)

  IF(res)
    MESSAGE(STATUS "mkoctfile command ${MKOCT} cannot be run, error '${err}' skipping oct")
  ELSE (res)

    SET(OCTEXT .mex)
    
    ADD_CUSTOM_TARGET(oct)      
    SET(INSTALL_OCT_FILES )
    MACRO(OCTCOMPILE_FILES )
      FOREACH(_current_FILE ${ARGN})
        GET_FILENAME_COMPONENT(_tmp_FILE ${_current_FILE} ABSOLUTE)
        GET_FILENAME_COMPONENT(_abs_PATH ${_tmp_FILE} PATH)
        GET_FILENAME_COMPONENT(_basename ${_tmp_FILE} NAME_WE)
        
        SET(_o_FILE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${_basename}${DEBUGEXT}${OCTEXT})
        #          MESSAGE(STATUS "_current_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${_current_FILE} _o_FILE ${_o_FILE}")
        SET(INSTALL_OCT_FILES ${INSTALL_OCT_FILES} ${_o_FILE})
        ADD_CUSTOM_COMMAND(OUTPUT ${_o_FILE} 
          COMMAND echo "Processing: ${_current_FILE}"
          COMMAND
          ${MKOCT} --mex -v ${CMAKE_CURRENT_SOURCE_DIR}/${_current_FILE}  -I${CMAKE_CURRENT_BINARY_DIR}/../SDIF/sdifconfig -I${CMAKE_CURRENT_BINARY_DIR}/.. -I${CMAKE_CURRENT_SOURCE_DIR}/../SDIF/include -I${CMAKE_CURRENT_SOURCE_DIR}/.. --output ${_o_FILE} -L${CMAKE_ARCHIVE_OUTPUT_DIRECTORY} -lEasdif_static_pic${DEBUGEXT}
          DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${_current_FILE}  ${DEP_INCLUDES} Easdif_static_pic${DEBUGEXT}) 
        
        ADD_CUSTOM_TARGET(${_basename}_oct${DEBUGEXT}  DEPENDS  ${_o_FILE})      
        #ADD_DEPENDENCIES(${_basename}  ${_o_FILE} )
        ADD_DEPENDENCIES(oct  ${_basename}_oct${DEBUGEXT} )
      ENDFOREACH(_current_FILE)
    ENDMACRO(OCTCOMPILE_FILES)
    
    SET(SOURCES Fsdif_read_handler.cpp Fsdif_write_handler.cpp)
    
    OCTCOMPILE_FILES(${SOURCES})
    
    SET(MFILES_OCT ${CMAKE_CURRENT_SOURCE_DIR}/Fsdifclose.m 
      ${CMAKE_CURRENT_SOURCE_DIR}/Fsdifopen.m 
      ${CMAKE_CURRENT_SOURCE_DIR}/Fsdifwrite.m 
      ${CMAKE_CURRENT_SOURCE_DIR}/Fsdifread.m 
      ${CMAKE_CURRENT_SOURCE_DIR}/Fsdifexist.m 
      ${CMAKE_CURRENT_SOURCE_DIR}/Fsdifloadfile.m)
    
    INSTALL(FILES ${INSTALL_OCT_FILES} ${MFILES_OCT} DESTINATION ${INSTALL_LIB_DIR}/octave/ )
  ENDIF(res)  
ENDIF(MKOCT)
