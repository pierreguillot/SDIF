#
# cmake configuration file
#
# This file is part of the IRCAM SDIF Library (http://www.ircam.fr/sdif)
#
# Copyright (C) 2006 by IRCAM - Centre Pompidou, Paris, France.
#
# created by Axel Roebel                     7.12.2006
#

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../sdif)

ADD_EXECUTABLE(querysdif${DEBUGEXT} querysdif.c)
TARGET_LINK_LIBRARIES(querysdif${DEBUGEXT} sdif${DEBUGEXT} m)

ADD_EXECUTABLE(sdifextract${DEBUGEXT} sdifextract.c)
TARGET_LINK_LIBRARIES(sdifextract${DEBUGEXT} sdif${DEBUGEXT} m)

ADD_EXECUTABLE(sdiftotext${DEBUGEXT} sdiftotext.c)
TARGET_LINK_LIBRARIES(sdiftotext${DEBUGEXT} sdif${DEBUGEXT} m)

ADD_EXECUTABLE(tosdif${DEBUGEXT} tosdif.c)
TARGET_LINK_LIBRARIES(tosdif${DEBUGEXT} sdif${DEBUGEXT} m)

INSTALL(TARGETS querysdif${DEBUGEXT} sdifextract${DEBUGEXT} sdiftotext${DEBUGEXT} tosdif${DEBUGEXT} DESTINATION ${INSTALL_BIN_DIR} CONFIGURATIONS RELEASE )
