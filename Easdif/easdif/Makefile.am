# $Id: Makefile.am,v 1.5 2003-03-18 15:37:11 roebel Exp $
#
# ./Easdif/src/Makefile.am	Feb. 2003 Axel Roebel
#
# Source file for GNU automake, generates Makefile.in.
#

# $Log: not supported by cvs2svn $
# Revision 1.4  2003/03/07 17:38:41  roebel
# Updated for correct handling of distribution and new layout of c++ compilation in SDIF.
#
# Revision 1.3  2003/03/05 15:33:13  roebel
# Fixed installation of include files.
#
# Revision 1.2  2003/03/04 00:55:02  roebel
# Removed version determination
#
# Revision 1.1  2003/03/03 19:00:16  roebel
# Moved src directory to new name easdif
#
# Revision 1.1  2003/03/03 10:03:50  roebel
# Adapted for autoconf/automake...
#

# EXTRA_LIBRARIES to provide knowledge about possible substitutions
# in lib_LTLIBRARIES
EXTRA_LTLIBRARIES = libEasdif.la 

# lib_LIBRARIES = @SELLIB@
lib_LTLIBRARIES = @SELLIB@

EXTRA_DIST = easdif_version.h.in

EASDIF_H = easdif_version.h easdif.h  sdifentity.h \
	sdifexception.h sdifframe.h sdifinit.h sdifmatrix.h sdifmatrixdata.h \
	sdifmatrixdatainterface.h sdifnamevaluetable.h

easdifincdir = $(prefix)/include/easdif
easdifinc_HEADERS = $(EASDIF_H)

libEasdif_la_SOURCES =  sdifentity.cpp sdifexception.cpp sdifframe.cpp \
			sdifinit.cpp sdifmatrix.cpp sdifnamevaluetable.cpp

libEasdif_la_LIBADD  = @SDIFCPPLIB@
libEasdif_la_LDFLAGS = -no-undefined  -rpath $(libdir)


INCLUDES = -I$(top_srcdir) -I../SDIF/include -I$(top_srcdir)/SDIF/include -I$(top_srcdir)/SDIF/sdif   -DUSE_CONFIGURE


#
# EXTRA rules and dependencies to generate easdif_version.h
#
#$(top_srcdir)/src/easdif_version.h: $(top_srcdir)/Doxyfile
#	echo "/* Version automatically generated from Makefile/DoxyFile" >$@.h
#	echo " * do not edit */" >>$@.h
#	echo "#ifndef EASDIF_VERSION_MAJ"	 >>$@.h
#	echo "#define" EASDIF_VERSION_MAJ $(strip $(shell grep ^PROJECT_NUMBER  $< | cut -d"=" -f2| cut -d. -f1)) >>$@.h
#	echo "#define" EASDIF_VERSION_MIN $(strip $(shell grep ^PROJECT_NUMBER  $< | cut -d"=" -f2| cut -d. -f2)) >>$@.h
#	echo "#endif "	 >>$@.h


