# $Id: Makefile.am,v 1.2 2003-03-04 00:56:12 roebel Exp $
#
# ./Easdif/test/Makefile.am	Feb. 2003 Axel Roebel
#
# Source file for GNU automake, generates Makefile.in.
#

# $Log: not supported by cvs2svn $
# Revision 1.1  2003/03/03 10:04:25  roebel
# Adapted for autoconf/automake...
#

EXTRA_PROGRAMS=
bin_PROGRAMS = easdifRmNVT tutorini tutofini

EASDIR=../easdif
easdifRmNVT_SOURCES = easdifRmNVT.cpp
easdifRmNVT_LDADD = -lEasdif -lm
easdifRmNVT_LDFLAGS = -L$(EASDIR)
easdifRmNVT_DEPENDENCIES=$(EASDIR)/libEasdif.la

tutorini_SOURCES = tutorini.cpp
tutorini_LDADD = -lEasdif -lm
tutorini_LDFLAGS = -L$(EASDIR)
tutorini_DEPENDENCIES=$(EASDIR)/libEasdif.la

tutofini_SOURCES = tutofini.cpp
tutofini_LDADD = -lEasdif -lm
tutofini_LDFLAGS = -L$(EASDIR)
tutofini_DEPENDENCIES=$(EASDIR)/libEasdif.la


CXXFLAGS = -g -I$(top_srcdir)  -I$(top_srcdir)/SDIF/include 












