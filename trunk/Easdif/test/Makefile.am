# $Id: Makefile.am,v 1.1 2003-03-03 10:04:25 roebel Exp $
#
# ./Easdif/test/Makefile.am	Feb. 2003 Axel Roebel
#
# Source file for GNU automake, generates Makefile.in.
#

# $Log: not supported by cvs2svn $

EXTRA_PROGRAMS=
bin_PROGRAMS = easdifRmNVT tutorini tutofini

easdifRmNVT_SOURCES = easdifRmNVT.cpp
easdifRmNVT_LDADD = -lEasdif -lm
easdifRmNVT_LDFLAGS = -L$(top_srcdir)/src
easdifRmNVT_DEPENDENCIES=$(top_srcdir)/src/libEasdif.la

tutorini_SOURCES = tutorini.cpp
tutorini_LDADD = -lEasdif -lm
tutorini_LDFLAGS = -L$(top_srcdir)/src
tutorini_DEPENDENCIES=$(top_srcdir)/src/libEasdif.la

tutofini_SOURCES = tutofini.cpp
tutofini_LDADD = -lEasdif -lm
tutofini_LDFLAGS = -L$(top_srcdir)/src
tutofini_DEPENDENCIES=$(top_srcdir)/src/libEasdif.la


CXXFLAGS = -g -I$(top_srcdir)  -I$(top_srcdir)/SDIF/include 












