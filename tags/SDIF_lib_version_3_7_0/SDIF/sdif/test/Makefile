# $Id: Makefile,v 1.4 2002-05-24 19:38:07 ftissera Exp $
#
# SDIF/sdif/test/Makefile
#
# $Log: not supported by cvs2svn $
# Revision 1.3  2000/05/04 14:57:29  schwarz
# test check for SdifCheckFrames funcs.
#
# Revision 1.2  1999/09/28  13:15:11  schwarz
# Test case
#
# Revision 1.1  1999/08/31  10:03:37  schwarz
# Added test code for module SdifSelect which parses an access specification to a
# chosen part of SDIF data.  Can be added to a file name.
#

# Version modifiee de SDIF
MODIF_SDIF_LIBROOT = /u/formes/ftissera/src/SDIF

CC = gcc
INC_DIR = -I$(MODIF_SDIF_LIBROOT)/include
CFLAGS = -g -ggdb -Wall $(INC_DIR)
LD_DIR = -L$(MODIF_SDIF_LIBROOT)/sdif
LDFLAGS = $(LD_DIR) -lsdif -lm

essai_err: essai_err.c $(MODIF_SDIF_LIBROOT)/include/sdif.h
	$(CC) essai_err.c -o $@ $(CFLAGS) $(LDFLAGS)

clean:
	-rm essai_err core *~






