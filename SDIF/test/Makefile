
include ../Makefile.preamble

VERSION = $(SYSTEM)_$(SYSVERS)_1.00.alpha


LIBNAME = sdif
LIB_DIR = ../lib$(LIBNAME)/$(SYSTEM)_$(SYSVERS)
LDFLAGS = -L$(LIB_DIR)
INCLUDE_DIR_SRC_LIB = ../$(LIBNAME)
LIBS = -l$(LIBNAME)


CFLAGS = $(CAFLAGS) $(DOFLAGS) -D_SDIF_VERSION_="$(VERSION)" -I. -I$(INCLUDE_DIR) -I$(INCLUDE_DIR_SRC_LIB)


include Makefile.src

ODIRS  = $(SYSTEM)_$(SYSVERS)


OBJS = $(SRC:%.c=$(ODIRS)/%.o)


PROGNAME = test
PROGDIRS = ./bin/$(SYSTEM)_$(SYSVERS)
PROG = $(PROGDIRS)/$(PROGNAME)

all: dirs prog

dirs: $(PROGDIRS:%=%-dir)  $(ODIRS:%=%-dir)

%-dir: 
	@if ( ! -e $* ) mkdir -p $*

prog: $(PROG)
	cp  ../lib$(LIBNAME)/SdifTypes.STYP ./
	@echo $(PROGNAME)

$(PROG): $(OBJS) ../lib$(LIBNAME)/$(SYSTEM)_$(SYSVERS)/lib$(LIBNAME).a
	$(CC) $(LDFLAGS) $(LFLAGS) $(OBJS) $(LIBS) -o $(PROG)

$(ODIRS)/%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $< 

clean:
	rm -f $(PROG)
	rm -f $(OBJS)
	rm -f *~
	rm -f #*
	rm -f core

install: $(PROG)
	cp  $(PROG) $(HOME)/bin/$(ARCH)/$(ODIRS)
test:
	cd $(PROGDIRS) ;
