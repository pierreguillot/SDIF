# $Id: makefile,v 1.6 2000-07-24 12:33:33 schwarz Exp $
#
# makefile	26. January 2000	Diemo Schwarz
#
# formesutils matlab library
#
# $Log: not supported by cvs2svn $
# Revision 1.5  2000/07/19  16:32:45  schwarz
# Factored out commonly used make-definitions for mex compilation,
# put into mex.makedefs make-include file.
#
# Revision 1.4  2000/05/12  16:14:12  tisseran
# Mexfile to write sdif files in matlab.
# TODO: add possibility to use several file at same time.
#       add test on arguments
#
# Revision 1.3  2000/05/12  14:05:03  schwarz
# Target install: installs INSTALL_FILES into INSTALL_DIR,
# currently /u/formes/lib/matlab.
# Target test: runs matlab with testl.m.
#
# Revision 1.2  2000/05/11  12:38:22  schwarz
# Compiles mex on Linux!  (Paths and compiler had to be found.)
# db target.
#
# Revision 1.1  2000/05/04  13:24:08  schwarz
# Matlab mex extension and support functions to load SDIF files.
#

# include mex definitions from the right places
MAKEFLAGS = -I . -I /u/formes/lib/matlab/
include mex.makedefs

# under IMPORT_ROOT, imported lib and include directories are used
ifeq ($(USER),tisseran)
  IMPORT_ROOT	 = $(HOME)
endif

CFLAGS	   	 = -g -ansi -Wall -DTEST
LFLAGS		 = 
CC		 = gcc

OBJ_DIR	   	 = obj/$(ARCH)
OUT_DIRS	 = $(OBJ_DIR)
VPATH		 = $(OBJ_DIR)

# install mex files and supporting m-files INSTALL_FILES into INSTALL_DIR
INSTALL_DIR	 = /u/formes/lib/matlab
INSTALL_FILES	 = $(mexfiles) mex.makedefs \
		   loadsdiffile.m loadsdifflat.m writesdif.m sdifexist.m
INSTALLED_FILES  = $(INSTALL_FILES:%=$(INSTALL_DIR)/%)

#
# files
#

base		= loadsdif writesdif
loadsdif_src	= loadsdif-subs.c
writesdif_src	= writesdif-subs.c
loadsdif_obj	= $(loadsdif_src:%.c=$(OBJ_DIR)/%.o)
writesdif_obj	= $(writesdif_src:%.c=$(OBJ_DIR)/%.o)
cfiles		= $(base:%=%.c) $(loadsdif_src) $(writesdif_src) 
mexfiles	= $(notdir $(base:%=%.$(MEXSUF)))
allsrc		= $(cfiles)


#
# rules
#

$(OBJ_DIR)/%.o:	%.c
		$(MEX-COMPILE)

.PHONY:		t1.sdif test testl


#
# targets
#

all:			$(OUT_DIRS) $(mexfiles)
loadsdif:		loadsdif.$(MEXSUF)
writesdif:		writesdif.$(MEXSUF) 

# dependencies
loadsdif.$(MEXSUF):	loadsdif.c  $(loadsdif_obj)  $(PM_LIB) $(SDIF_LIB)
loadsdif.c:		loadsdif.h 
$(loadsdif_obj):	$(loadsdif_src)  loadsdif.h

writesdif.$(MEXSUF):	writesdif.c $(writesdif_obj) $(PM_LIB) $(SDIF_LIB)
writesdif.c:		writesdif.h 
$(writesdif_obj):	$(writesdif_src) writesdif.h

# external builds
sdif:
		$(MAKE) -C $(HOME)/src/SDIF
db:
		$(MAKE) DEBUG=1

test:		$(mexfiles) testl t1.sdif

testl:
		echo testl | matlab -nosplash
t1.sdif:
		(echo "writesdif('$@')"; \
		 echo "writesdif(0,1,'1NVT','1NVT','name\tvalue\n')"; \
		 echo "writesdif(1,1,'1ABC','IABC',[1],'1ABC',[1 2; 5 6])"; \
		 echo "writesdif('close')" )  |  matlab -nosplash
		querysdif $@
		sdiftotext -i $@

install:	all
		cp -fp $(INSTALL_FILES) $(INSTALL_DIR)
		chmod g-w $(INSTALLED_FILES) # assure ownership-change

clean mexclean cl:
		-rm -f $(mexfiles) $(loadsdif_obj) $(writesdif_obj) 

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
		nm loadsdif.$(MEXSUF) | grep '| U | 0*$$'
		echo [missing objects in writesdif]
		nm writesdif.$(MEXSUF) | grep '| U | 0*$$'

$(OUT_DIRS):
	mkdir -p $@

tags:		TAGS
TAGS:		$(allsrc)
		etags $(allsrc)
