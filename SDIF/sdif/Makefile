# Makefile only for .o files of sdif library

PRJ_DIR := $(shell pwd)/..


include ../Makefile.preamble

CFLAGS := $(CAFLAGS) $(DOFLAGS) $(D_SDIF_FLAG) -I$(SDIF_SRC_DIR) -I$(INCLUDE_DIR)


#
#	targets
#



all: dirs objects

dirs: $(SDIF_OBJ_DIR:%=%-dir)

%-dir: 
	@if ( ! -e $* ) mkdir -p $*

objects: $(SDIF_OBJS)
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
