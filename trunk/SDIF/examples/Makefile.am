# $Id: Makefile.am,v 1.3 2009-01-07 11:40:04 diemo Exp $
#
# examples/Makefile.am             December 2008          Diemo Schwarz
#
# build and distribute example source code
#
# $Log: not supported by cvs2svn $
# Revision 1.2  2009/01/05 16:00:58  diemo
# trying to add source code to dist, not working yet
#

INCLUDES  = -I../include -I$(top_srcdir)/include -I$(top_srcdir)/sdif
CFLAGS   = @CFLAGS@ '-DVERSION="$(VERSION)"'

# this is for automake
EXTRA_DIST = list-types.c list-nvt.c

all:		list-types list-nvt

list-types: 	list-types.c
		gcc -I../include -g  -o list-types list-types.c -lsdif

list-nvt: 	list-nvt.c
		gcc -I../include -g  -o $@ $^ -lsdif
