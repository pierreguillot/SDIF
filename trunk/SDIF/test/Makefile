
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
# $Id: Makefile,v 3.2 2000-09-27 10:18:00 schwarz Exp $
#
#               Copyright (c) 1998 by IRCAM - Centre Pompidou
#                          All rights reserved.
#
#  For any information regarding this and other IRCAM software, please
#  send email to:
#                            manager@ircam.fr
#
	rm -f #*
	rm -f core

install: $(PROG)
	cp  $(PROG) $(HOME)/bin/$(ARCH)/$(ODIRS)
test:
	cd $(PROGDIRS) ;


# crash: sdifextract matt/fft.sdif::@0-1|wc
# crash: cd matt; sdifextract venti.sdif::@0-1 xventi.sdif
