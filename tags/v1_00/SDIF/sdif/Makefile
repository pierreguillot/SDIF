# Makefile only for .o files of sdif library

include ../Makefile.preamble



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
	rm -f *~
	rm -f #*

clean:
	rm -f $(SDIF_OBJS)
	rm -f *~
	rm -f #*
	rm -f *.i

include ../Makefile.postamble
