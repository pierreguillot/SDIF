
SET(noinst_HEADERS  easdifintro.hpp easdif_config.hpp)
SET(HEADERS  easdif.h easdif_version.hpp sdifentity.hpp sdifexception.hpp sdif_frame.hpp sdifinit.hpp sdifmatrixdata.hpp sdifmatrixdatainterface.hpp sdif_matrix.hpp sdifnamevaluetable.hpp)

SET(SOURCES sdifentity.cpp  sdifexception.cpp sdif_matrix.cpp sdif_frame.cpp sdifnamevaluetable.cpp sdifinit.cpp)
SET(SDIF_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../SDIF/sdif/SdifCheck.c  
  ${CMAKE_CURRENT_SOURCE_DIR}/../SDIF/sdif/SdifConvToText.c  
  ${CMAKE_CURRENT_SOURCE_DIR}/../SDIF/sdif/SdifErrMess.c 
  ${CMAKE_CURRENT_SOURCE_DIR}/../SDIF/sdif/SdifFGet.c          
  ${CMAKE_CURRENT_SOURCE_DIR}/../SDIF/sdif/SdifFile.c        
  ${CMAKE_CURRENT_SOURCE_DIR}/../SDIF/sdif/SdifFPrint.c
  ${CMAKE_CURRENT_SOURCE_DIR}/../SDIF/sdif/SdifFPut.c          
  ${CMAKE_CURRENT_SOURCE_DIR}/../SDIF/sdif/SdifFrame.c       
  ${CMAKE_CURRENT_SOURCE_DIR}/../SDIF/sdif/SdifFrameType.c
  ${CMAKE_CURRENT_SOURCE_DIR}/../SDIF/sdif/SdifFRead.c         
  ${CMAKE_CURRENT_SOURCE_DIR}/../SDIF/sdif/SdifFScan.c       
  ${CMAKE_CURRENT_SOURCE_DIR}/../SDIF/sdif/SdifFWrite.c
  ${CMAKE_CURRENT_SOURCE_DIR}/../SDIF/sdif/SdifGlobals.c       
  ${CMAKE_CURRENT_SOURCE_DIR}/../SDIF/sdif/SdifHard_OS.c     
  ${CMAKE_CURRENT_SOURCE_DIR}/../SDIF/sdif/SdifHash.c
  ${CMAKE_CURRENT_SOURCE_DIR}/../SDIF/sdif/SdifHighLevel.c     
  ${CMAKE_CURRENT_SOURCE_DIR}/../SDIF/sdif/SdifList.c        
  ${CMAKE_CURRENT_SOURCE_DIR}/../SDIF/sdif/SdifMatrix.c
  ${CMAKE_CURRENT_SOURCE_DIR}/../SDIF/sdif/SdifMatrixType.c    
  ${CMAKE_CURRENT_SOURCE_DIR}/../SDIF/sdif/SdifNameValue.c   
  ${CMAKE_CURRENT_SOURCE_DIR}/../SDIF/sdif/SdifPreTypes.c
  ${CMAKE_CURRENT_SOURCE_DIR}/../SDIF/sdif/SdifPrint.c         
  ${CMAKE_CURRENT_SOURCE_DIR}/../SDIF/sdif/SdifRWLowLevel.c  
  ${CMAKE_CURRENT_SOURCE_DIR}/../SDIF/sdif/SdifSelect.c
  ${CMAKE_CURRENT_SOURCE_DIR}/../SDIF/sdif/SdifSignatureTab.c  
  ${CMAKE_CURRENT_SOURCE_DIR}/../SDIF/sdif/SdifStreamID.c    
  ${CMAKE_CURRENT_SOURCE_DIR}/../SDIF/sdif/SdifString.c
  ${CMAKE_CURRENT_SOURCE_DIR}/../SDIF/sdif/SdifTest.c          
  ${CMAKE_CURRENT_SOURCE_DIR}/../SDIF/sdif/SdifTextConv.c    
  ${CMAKE_CURRENT_SOURCE_DIR}/../SDIF/sdif/SdifTimePosition.c
)


ADD_LIBRARY(Easdif_static${DEBUGEXT} STATIC  ${SOURCES} ${SDIF_SOURCES}  )
ADD_LIBRARY(Easdif${DEBUGEXT} SHARED ${SOURCES} ${SDIF_SOURCES} )

SET_TARGET_PROPERTIES(Easdif${DEBUGEXT} PROPERTIES VERSION ${EASDIF_VERSION} LINKER_LANGUAGE CXX)

SET_TARGET_PROPERTIES(Easdif_static${DEBUGEXT} PROPERTIES VERSION ${EASDIF_VERSION} POST_INSTALL_SCRIPT ${CMAKE_BINARY_DIR}/easdif/RenameInstalledStaticLib.cmake )
INSTALL_TARGETS(${INSTALL_LIB_DIR} Easdif${DEBUGEXT} Easdif_static${DEBUGEXT} )
INSTALL_FILES(${INSTALL_INC_DIR} FILES ${HEADERS} )

SET(BEFORE_RENAME "Easdif_static${DEBUGEXT}")
SET(AFTER_RENAME "Easdif${DEBUGEXT}")
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/../cmModules/RenameInstalledStaticLib.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/RenameInstalledStaticLib.cmake @ONLY)
INCLUDE(STORE_FILES_LIST)
STORE_FILES_LIST(DOCFILES "${SOURCES} ${HEADERS} ${noinst_HEADERS}")
