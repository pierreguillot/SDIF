lib_LIBRARIES = libsdif.a
FORMES_INCLUDE=/u/formes/include
noinst_HEADERS = *.h ${FORMES_H}
libsdif_a_dir = $(prefix)/libsdif
libsdif_a_SOURCES =  SdifString.c      SdifHash.c     SdifGlobals.c      SdifSignatureTab.c \
		     SdifError.c       SdifMemory.c   SdifNameValue.c    SdifMatrixType.c \
		     SdifFrameType.c   SdifStreamID.c SdifMatrix.c       SdifFrame.c \
		     SdifPrint.c       SdifTest.c     SdifTimePosition.c SdifFile.c \
		     SdifRWLowLevel.c  SdifFGet.c     SdifFRead.c        SdifFScan.c \
		     SdifFPut.c        SdifFPrint.c   SdifFWrite.c       SdifTextConv.c \
		     SdifConvToText.c  SdifHard_OS.c  SdifErrMess.c      SdifList.c \
		     SdifPreTypes.c    SdifSelect.c   SdifHighLevel.c    SdifCheck.c 
sdiftype = $(datadir)/SdifTypes.STYP
INCLUDES = -D_SdifTypesFileName=\"$(sdiftype)\" -DUSE_CONFIGURE

# EXTRA FOR IRCAM STUFF
ircam_deps : ${FORMES_H}

host_architecture.h:
	cp ${FORMES_INCLUDE}/host_architecture.h .

preincluded.h :
	cp ${FORMES_INCLUDE}/preincluded.h .

UniversalEnvVar.h : 
	cp ${FORMES_INCLUDE}/UniversalEnvVar.h .

host_configure.h: 
	cp ${FORMES_INCLUDE}/host_configure.h .

XpGuiCalls.h:
	cp ${FORMES_INCLUDE}/XpGuiCalls.h .
