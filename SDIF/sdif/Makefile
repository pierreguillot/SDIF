# $Id: Makefile,v 2.1 1998-12-03 18:10:47 schwarz Exp $
#
# Makefile only for .o files of sdif library
#
# $Log: not supported by cvs2svn $

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
