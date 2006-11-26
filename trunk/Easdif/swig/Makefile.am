# $Id: Makefile.am,v 1.6 2006-11-26 16:51:29 roebel Exp $
#
# eaSDIF - swig/Makefile.am
#
# Main makefile for swig wrapper generation for eaSDIF C++ library.
#
# $Log: not supported by cvs2svn $
# Revision 1.5  2006/11/26 16:12:18  roebel
# Disable java subdir if not java compiler is not available.
#
# Revision 1.4  2003/06/04 20:40:43  schwarz
# Added targets to make just one language.
# Moved java to back because it doesn't work at the moment.
#


if MAKE_JAVA
JAVADIR = java
else
JAVADIR =
endif

SUBDIRS = python $(JAVA_DIR) perl5 
DIST_SUBDIRS = python java perl5  m4


$(SUBDIRS):
	$(MAKE) -C $@

# make only one language
perl:
	$(MAKE) -C perl5

