lib_LIBRARIES = libsdif.a
noinst_HEADERS = *.h
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


#libsdif_a_LIBADD = @LIBOBJS@ @ALLOCA@