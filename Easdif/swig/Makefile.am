# $Id: Makefile.am,v 1.4 2003-06-04 20:40:43 schwarz Exp $
#
# eaSDIF - swig/Makefile.am
#
# Main makefile for swig wrapper generation for eaSDIF C++ library.
#
# $Log: not supported by cvs2svn $

SUBDIRS = perl5 python java 

# make only one language
$(SUBDIRS):
	$(MAKE) -C $@

perl:
	$(MAKE) -C perl5
