# $Id: Makefile.am,v 1.1 2003-03-03 19:00:16 roebel Exp $
#
# ./Easdif/src/Makefile.am	Feb. 2003 Axel Roebel
#
# Source file for GNU automake, generates Makefile.in.
#

# $Log: not supported by cvs2svn $
# Revision 1.1  2003/03/03 10:03:50  roebel
# Adapted for autoconf/automake...
#

# EXTRA_LIBRARIES to provide knowledge about possible substitutions
# in lib_LTLIBRARIES
EXTRA_LTLIBRARIES = libEasdif.la 

# lib_LIBRARIES = @SELLIB@
lib_LTLIBRARIES = @SELLIB@


EASDIF_H = easdif_version.h easdif.h  sdifentity.h \
	sdifexception.h sdifframe.h sdifinit.h sdifmatrix.h sdifmatrixdata.h \
	sdifmatrixdatainterface.h sdifnamevaluetable.h


noinst_HEADERS = easdifintro.h

libEasdif_la_SOURCES =  sdifentity.cpp sdifexception.cpp sdifframe.cpp \
			sdifinit.cpp sdifmatrix.cpp sdifnamevaluetable.cpp

libEasdif_la_LIBADD  = @SDIFOBJECTS@

libEasdif_la_LDFLAGS = -no-undefined  -rpath $(libdir)

INCLUDES = -I$(top_srcdir) -I$(top_srcdir)/SDIF/include -I$(top_srcdir)/SDIF/sdif   -DUSE_CONFIGURE


#
# EXTRA rules and dependencies to generate easdif_version.h
#
$(top_srcdir)/src/easdif_version.h: $(top_srcdir)/Doxyfile
	echo "/* Version automatically generated from Makefile/DoxyFile" >$@.h
	echo " * do not edit */" >>$@.h
	echo "#ifndef EASDIF_VERSION_MAJ"	 >>$@.h
	echo "#define" EASDIF_VERSION_MAJ $(strip $(shell grep ^PROJECT_NUMBER  $< | cut -d"=" -f2| cut -d. -f1)) >>$@.h
	echo "#define" EASDIF_VERSION_MIN $(strip $(shell grep ^PROJECT_NUMBER  $< | cut -d"=" -f2| cut -d. -f2)) >>$@.h
	echo "#endif "	 >>$@.h


