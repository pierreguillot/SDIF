# $Id: makefile,v 1.3 2000-05-12 14:05:03 schwarz Exp $
#
# makefile	26. January 2000	Diemo Schwarz
#
# formesutils matlab library
#
# $Log: not supported by cvs2svn $
# Revision 1.2  2000/05/11  12:38:22  schwarz
# Compiles mex on Linux!  (Paths and compiler had to be found.)
# db target.
#
# Revision 1.1  2000/05/04  13:24:08  schwarz
# Matlab mex extension and support functions to load SDIF files.
#


#
# settings
#

ARCH 		:= $(shell arch)
# we don't care for IRIX64
SYSNAME		:= $(subst IRIX64,IRIX,$(shell uname -s))
SYSVERS		:= $(shell uname -r)
SYS		:= $(SYSNAME)_$(SYSVERS)
USER 		:= $(shell whoami)

# mex suffix per arch
mex-alpha	 = mexaxp
mex-i686	 = mexlx
mexsuf     	 = $(mex-$(ARCH))

# mex compiler
#mex	   	 = /usr/LOCAL/matlab5/bin/mex
mex	   	 = mex

# mex includes
matlabincl-alpha = /usr/LOCAL/matlab-5.2/extern/include
matlabincl-i686  = /usr/local/matlab/extern/include


ifeq ($(DEBUG),1)
  VERS		 = -debug
  VERSION	 = -debug
else
  VERS		 = 
  VERSION	 = -release
endif

# under IMPORT_ROOT, imported lib and include directories are used
ifeq ($(USER),schwarz)
  IMPORT_ROOT	 = $(HOME)
else 
  IMPORT_ROOT	 = /u/formes
endif

GLOBAL_ROOT	 = /u/formes
GLOBAL_INCL 	 = $(IMPORT_ROOT)/include 
GLOBAL_LIB	 = $(IMPORT_ROOT)/lib/$(ARCH)

SDIF_LIB_DIR     = $(GLOBAL_LIB)/$(SYS)$(VERSION)
SDIF_LIB_BASE    = sdif$(VERS)
SDIF_LIB_PATH    = $(SDIF_LIB_DIR)/lib$(SDIF_LIB_BASE).a

PM_LIB_DIR	 = $(GLOBAL_LIB)
PM_LIB_BASE	 = Pm
PM_LIB_PATH	 = $(PM_LIB_DIR)/lib$(PM_LIB_BASE).a

pm_sdif		 = $(PM_LIB_PATH) $(SDIF_LIB_PATH)
INCL	   	 = -I$(GLOBAL_INCL) -I/usr/include -I/usr/local/include/
CFLAGS	   	 = -g -ansi -Wall -DTEST $(INCL)
LFLAGS		 = 
MEXFLAGS-alpha	 = -g
MEXFLAGS-i686	 = -g CC=gcc LD=gcc

OBJ_DIR	   	 = obj/$(ARCH)
OUT_DIRS	 = $(OBJ_DIR)

# install mex files and supporting m-files INSTALL_FILES into INSTALL_DIR
INSTALL_DIR	 = /u/formes/lib/matlab
INSTALL_FILES	 = $(mexfiles) loadsdiffile.m loadsdifflat.m


#
# rules
#

.SUFFIXES:	.c .$(mexsuf)

.c.$(mexsuf):
	$(mex) $(MEXFLAGS-$(ARCH)) $(INCL) $(LFLAGS) $($(@F)_mexflags) $^

$(OBJ_DIR)/%.o:	%.c
	gcc -c $(CFLAGS) -I$(matlabincl-$(ARCH)) $< -o $@


#
# files
#

base		= loadsdif
loadsdif_src	= loadsdif-subs.c
loadsdif_obj	= $(loadsdif_src:%.c=$(OBJ_DIR)/%.o)
cfiles		= $(base:%=%.c) $(loadsdif_src)
mexfiles	= $(notdir $(base:%=%.$(mexsuf)))
allsrc		= $(cfiles)


#
# targets
#

all:			$(OUT_DIRS) $(mexfiles)
loadsdif:		loadsdif.$(mexsuf)

# dependencies
loadsdif.$(mexsuf):	loadsdif.c $(loadsdif_obj) $(pm_sdif)
loadsdif.c:		loadsdif.h 
$(loadsdif_obj):	$(loadsdif_src) loadsdif.h

# external builds
sdif:
		$(MAKE) -C $(HOME)/src/SDIF
db:
		$(MAKE) DEBUG=1

test:		$(mexfiles)
		echo testl | matlab -nosplash

install:	all
		cp $(INSTALL_FILES) $(INSTALL_DIR)

clean mexclean cl:
		-rm -f $(mexfiles)
ls:	   
		@echo $(mexfiles)
ll:	   
		@echo $(CFLAGS)
		@echo $(INCL)
		@echo $(INCLUDE)
		@echo $(udidepend)

# report missing symbols in loadsdif lib
mis missing:	$(mexfiles)
		@echo; echo [missing objects in loadsdif]
		nm loadsdif.$(mexsuf) | grep '| U | 0*$$'

tags:		TAGS
TAGS:		$(allsrc)
		etags $(allsrc)

$(OUT_DIRS):
		mkdir -p $@
