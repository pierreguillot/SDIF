# $Id: Makefile,v 3.1 1999-03-14 10:56:28 virolle Exp $
#
#               Copyright (c) 1998 by IRCAM - Centre Pompidou
#                          All rights reserved.
#
#  For any information regarding this and other IRCAM software, please
#  send email to:
#                            manager@ircam.fr
#
#
# Makefile only for .o files of sdif library
#
# $Log: not supported by cvs2svn $
# Revision 2.2  1998/12/21  18:26:56  schwarz
# Inserted copyright message.
#
# Revision 2.1  1998/12/03  18:10:47  schwarz
# Corrected and simplified generation of output directories,
# added missing header files to list to be installed.
#

PRJ_DIR := $(shell pwd)/..


include ../Makefile.preamble

CFLAGS := $(CAFLAGS) $(DOFLAGS) $(D_SDIF_FLAG) -I$(SDIF_SRC_DIR) -I$(INCLUDE_DIR)
OUTDIRS = $(SDIF_OBJ_DIR)


#
#	targets
#



all: objects

objects: $(OUTDIRS) $(SDIF_OBJS)
	@echo objects

semiclean:
	-rm -f *~
	-rm -f #*

clean:
	-rm -f $(SDIF_OBJS)
	-rm -f *~
	-rm -f #*
	-rm -f *.i

include ../Makefile.postamble
