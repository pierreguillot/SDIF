# $Id: makefile,v 1.9 2000-08-08 17:26:59 schwarz Exp $
#
# makefile	26. January 2000	Diemo Schwarz
#
# formesutils matlab library
#
# $Log: not supported by cvs2svn $
# Revision 1.8  2000/08/04  14:42:31  schwarz
# Added reset of file variable, prevents crash on double-close.
# Version number is written in NVTs, and is used for distribution,
# defined in makefile (major.minor.release).
# Types file now included in distribution and documentation.
#
# Revision 1.7  2000/07/27  18:24:49  schwarz
# Install really all necessary files.
#
# Revision 1.6  2000/07/24  12:33:33  schwarz
# Install all necessary files.
#
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


#
# VERSION
#

V_MAJOR 	= 0
V_MINOR 	= 1
V_RELEASE	= 4
VERSION		= $(V_MAJOR).$(V_MINOR).$(V_RELEASE)
CVSTAG		= sdif_matlab_v$(V_MAJOR)_$(V_MINOR)_$(V_RELEASE)

#
# PATHS and SETTINGS
#

# under IMPORT_ROOT, imported lib and include directories are used
ifeq ($(USER),tisseran)
  IMPORT_ROOT	 = $(HOME)
endif

CFLAGS	   	 = -g -Wall -DTEST '-DVERSION="$(VERSION)"'
LFLAGS		 = 
CC		 = gcc

OBJ_DIR	   	 = obj/$(ARCH)
OUT_DIRS	 = $(OBJ_DIR) $(INSTALL_DIR)
VPATH		 = $(OBJ_DIR)


#
# FILES
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
# INSTALL and DISTRIBUTION
#

# install mex files and supporting m-files INSTALL_FILES into INSTALL_DIR
INSTALL_DIR	= /u/formes/lib/matlab
CORE_FILES	= $(mexfiles) sdifexist.m \
		  loadsdif.m loadsdiffile.m loadsdifflat.m \
		  writesdif.m
INSTALL_FILES	= $(CORE_FILES) mex.makedefs
INSTALLED_FILES = $(INSTALL_FILES:%=$(INSTALL_DIR)/%)
DISTFILES	= $(CORE_FILES) -C $(SDIF_TYPES_DIR) $(SDIF_TYPES_FILE) \
		  -C $(subst /lib/,/bin/,$(SDIF_LIB_DIR)) querysdif #sdifextract
DISTBASE	= sdif-matlab-$(VERSION)


#
# RULES
#

$(OBJ_DIR)/%.o:	%.c
		$(MEX-COMPILE)

.PHONY:		t1.sdif test testl


#
# targets
#

all:			$(OBJ_DIR) $(mexfiles)
new:			clean all
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

test:		$(mexfiles) t0 testl t1.sdif

t0:
		echo testrw    | matlab -nosplash
testl:
		echo testl     | matlab -nosplash
t1t:	
		setenv  SDIFTYPES testtypes.styp; $(MAKE) t1.sdif
t1.sdif:
		echo "testwrite('$@'), loadsdiffile('$@')"  |  matlab -nosplash
		querysdif $@
		sdiftotext -i $@

install:	all $(INSTALL_DIR)
		cp -fp $(INSTALL_FILES) $(INSTALL_DIR)
		chmod g-w $(INSTALLED_FILES) # assure ownership-change
dist:		all
		tar cvfz $(DISTBASE).tar.gz $(DISTFILES)
		@echo now do: cvs tag $(CVSTAG)
publish:	dist
		rsh maelzel cp -p $(PWD)/$(DISTBASE).tar.gz /usr/local/www/ircam/equipes/analyse-synthese/sdif/download

clean mexclean cl:
		-rm -f $(mexfiles) $(loadsdif_obj) $(writesdif_obj) 

ls:	   
		@echo $(mexfiles)
ll:	   
		@echo $(CFLAGS)
		@echo $(INCL)
		@echo $(INCLUDE)

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
