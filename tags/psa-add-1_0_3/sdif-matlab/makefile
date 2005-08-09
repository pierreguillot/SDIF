# $Id: makefile,v 1.37 2005-07-13 13:04:21 roebel Exp $
#
# makefile	26. January 2000	Diemo Schwarz
#
# formesutils matlab library
#
# $Log: not supported by cvs2svn $
# Revision 1.36  2004/07/20 16:07:52  roebel
# Version 0.4.0
#
# Revision 1.35  2004/07/20 14:48:39  roebel
# Added new target issdiffile.mexglx
#
# Revision 1.34  2004/03/30 18:10:44  roebel
# Improved dist creation when not all binaries are available.
#
# Revision 1.33  2004/02/19 10:56:32  schwarz
# install loadsdifbpf
#
# Revision 1.32  2003/09/15 17:29:12  schwarz
# Me too says it's version 0.3!
#
# Revision 1.31  2003/04/22 16:08:54  roebel
# Added possibility for writing local type declaration to
# sdif file.
#
# Revision 1.30  2001/07/23 11:07:26  roebel
# Installation removes files first!
#
# Revision 1.29  2001/05/29 13:27:20  roebel
# Version 0.2.1b
#
# Revision 1.28  2001/05/29 09:55:38  roebel
# Version 0.2.1
#
# Revision 1.27  2001/05/28 16:33:45  roebel
# Version 0.2.0.
#
# Revision 1.26  2001/05/14 17:35:07  roebel
# Small typo fixed!
#
# Revision 1.25  2001/05/14 17:34:00  roebel
# Previous checkin comment was a typo in fact version is 0.1.9!
#
# Revision 1.24  2001/05/14 17:32:30  roebel
# Version changed to 1.9
#
# Revision 1.23  2001/05/04 13:05:35  roebel
# DIST_DIR changed to new directory layout of download dir
#
# Revision 1.22  2001/05/03 13:03:20  roebel
# New version recompiled with proper SDifTypes.STYP
#
# Revision 1.21  2001/04/20 14:48:41  roebel
# Added missing files data/testlm*.out to source distribution.
#
# Revision 1.20  2001/04/20 14:33:33  roebel
# Makefile totally rewritten: Source distribution added,
# automatic extension determination, support for matlab 5 and 6,
# improved tests, compilation environment is changed by means of
# 5 variables, only.
#
# Revision 1.19  2001/02/07 16:00:13  roebel
# Version 0.1.7b with README.first
#
# Revision 1.18  2001/02/07 15:56:42  roebel
# Added README.first to distribution
#
# Revision 1.17  2001/02/07 15:54:13  roebel
# Added support for matlab 6
#
# Revision 1.16  2000/12/19 16:44:11  roebel
# Fixed Bug in loadsdif - crash when last matrix was not selected
# Moved test file sequence4seg1.energy.sdif into repository
# Corrected sgi mex extension to mexsg such that sgi is now in
# distribution
#
# Revision 1.15  2000/09/28 19:17:55  schwarz
# Use new /u/formes/share directories.
#
# Revision 1.14  2000/08/27  14:32:20  schwarz
# Added data directory for reference data.
# Version number, dist uses sdif-matlab-readme, no querysdif.
#
# Revision 1.13  2000/08/27  14:24:12  schwarz
# Clarified empty matrix issue:  The doc was wrong!
# Updated doc and loadsdiffile and loadsdifflat now use eof flag right
# and don't stop on empty matrices.
#
# Revision 1.12  2000/08/23  15:03:20  schwarz
# Read/write test testrw with intrusion/selection of other data.
#
# Revision 1.11  2000/08/08  18:07:30  schwarz
# Added README with release notes to distribution.
#
# Revision 1.10  2000/08/08  17:47:48  schwarz
# Make distribution with all mex binaries.
#
# Revision 1.9  2000/08/08  17:26:59  schwarz
# Fixed 'last matrix not returned' bug.
# More test cases t0, t1.
#
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

# Please edit this section to match
# Your mex compiler script (comes with matlab and is usually
# MATLABROOT/bin/mex you may use mex for matlab 5 or matlab 6
# if you specify multiple commands you may compile
# for matlab 5 and 6 with one make call
MEXCMD    = mex

# The path to sdif.h 
SDIF_INCL  = /u/formes/share/include

# The path of the sdif library 
SDIF_LIB  = /u/formes/share/lib/$(SYS)/libsdif.a

# Special flags to link mex files, if you do not have a proper
# libc5 development environment installed on linux you may
# try:  LINKFLAGS = "LDFLAGS=-shared -nodefaultlibs"
LINKFLAGS = 

# install mex files and supporting m-files INSTALL_FILES into INSTALL_DIR
INSTALL_DIR	= /u/formes/share/lib/matlab


# It should not be necessary to edit below this line
#===================================================

ARCH = $(shell arch)

ifeq ($(wildcard setup.ircam),setup.ircam)
include setup.ircam
endif

#
# VERSION
#

V_MAJOR 	= 0
V_MINOR 	= 4
V_RELEASE	= 0
VERSION		= $(V_MAJOR).$(V_MINOR).$(V_RELEASE)
CVSTAG		= sdif_matlab_v$(V_MAJOR)_$(V_MINOR)_$(V_RELEASE)


ALLMEXSUF := mexsg mexglx mexlx mexaxp

.PHONY:		t1.sdif test testl

#
# targets
#
all:			allmex
new:			clean all

# find matlab interpreters that belong to the given mex commands
MATLABCMDS = $(subst mex,matlab,$(MEXCMD))

# determine suffix for current mex script
mexsuf.inc.$(ARCH): 
	@echo determining mex suffixes for your system
	@rm -f testmexext.mex*
	@for mexcmd in $(MEXCMD); do  $$mexcmd testmexext.c; done 
	@echo -n "MEXSUF := "  > mexsuf.inc.$(ARCH)
	@for mexout in testmexext.mex*; do echo -n `expr $$mexout : ".*[.]\(.*\)$$"` " " ; done >>   mexsuf.inc.$(ARCH);
	@echo >> mexsuf.inc.$(ARCH)
	@rm -f testmexext.mex*
	@echo suffix list is 
	@cat 	mexsuf.inc.$(ARCH)

-include mexsuf.inc.$(ARCH)


#
# FILES
#
mexbase		= loadsdif writesdif issdiffile
loadsdif_mex    = $(foreach suf,$(MEXSUF),loadsdif.$(suf))
writesdif_mex    = $(foreach suf,$(MEXSUF),writesdif.$(suf))
issdif_mex    = $(foreach suf,$(MEXSUF),issdiffile.$(suf))

loadsdif_src	= loadsdif-subs.c
writesdif_src	= writesdif-subs.c
cfiles		= $(mexbase:%=%.c) $(loadsdif_src) $(writesdif_src) 
hfiles		= $(mexbase:%=%.h)
mexfiles	= $(loadsdif_mex)  $(writesdif_mex) $(issdif_mex) 

mfiles		= loadsdif.m loadsdiffile.m loadallsdiffile.m loadsdifflat.m loadsdifbpf.m \
		  writesdif.m sdifexist.m 
allsrc		= $(cfiles) $(hfiles) testmexext.c
srcmfiles       = $(mfiles) tt.m testl.m sdifexist.m testreadwrite.m testrw.m \
testwrite.m
datadir         = data/*sdif data/*.sdif.ascii data/testl*.out

allmex:		$(mexfiles)

#
# INSTALL and DISTRIBUTION
#

INSTALL_FILES	= $(mexfiles) $(mfiles) 
INSTALLED_FILES = $(INSTALL_FILES:%=$(INSTALL_DIR)/%)

DIST_DIR	= /usr/local/www/ircam/equipes/analyse-synthese/sdif/download/sdif-matlab
DIST_FILES	= $(foreach s, $(ALLMEXSUF), $(mexbase:%=%.$s)) \
		  sdif-matlab-readme README.first $(mfiles) \
		  -C $(SDIF_TYPES_DIR) $(SDIF_TYPES_FILE) \
#		  -C $(subst /lib/,/bin/,$(SDIF_LIB_DIR)) querysdif
SRC_DIST_FILES	=  $(allsrc) $(srcmfiles) $(datadir) \
		  sdif-matlab-readme README.first INSTALL makefile \
		  testtypes.styp -C $(SDIF_TYPES_DIR) $(SDIF_TYPES_FILE) 
DIST_HOST	= infirme
DIST_BASE	= sdif-matlab-$(VERSION)
DIST_COPYFILES  = $(DIST_BASE).tar.gz $(DIST_BASE)-src.tar.gz sdif-matlab-readme

# Compiler includes
#
INCL	 = -I$(SDIF_INCL) -I/usr/include 


# compile -- will produce mexfiles with proper extension
# for all mex commands given
$(loadsdif_mex) : loadsdif.c  loadsdif-subs.c  $(SDIF_LIB) loadsdif.h 	
	@for mexcmd in $(MEXCMD); do \
	   echo Compiling:; \
	   echo $$mexcmd -O '-DVERS=\"$(VERSION)\"' $(INCL) $(LINKFLAGS) loadsdif.c  loadsdif-subs.c $(SDIF_LIB); \
	   $$mexcmd -O '-DVERS=\"$(VERSION)\"' $(INCL) $(LINKFLAGS) loadsdif.c  loadsdif-subs.c $(SDIF_LIB);\
	done	


$(writesdif_mex) : writesdif.c writesdif-subs.c $(SDIF_LIB) writesdif.h 
	@for mexcmd in $(MEXCMD); do \
	   echo Compiling:; \
	   echo $$mexcmd  -O '-DVERS=\"$(VERSION)\"' $(INCL) $(LINKFLAGS) writesdif.c writesdif-subs.c $(SDIF_LIB);\
	   $$mexcmd  -O '-DVERS=\"$(VERSION)\"' $(INCL) $(LINKFLAGS) writesdif.c writesdif-subs.c $(SDIF_LIB);\
	done	

$(issdif_mex) : issdiffile.c  $(SDIF_LIB) 
	@for mexcmd in $(MEXCMD); do \
	   echo Compiling:; \
	   echo $$mexcmd  -O '-DVERS=\"$(VERSION)\"' $(INCL) $(LINKFLAGS)  issdiffile.c $(SDIF_LIB);\
	   $$mexcmd  -O '-DVERS=\"$(VERSION)\"' $(INCL) $(LINKFLAGS) issdiffile.c $(SDIF_LIB);\
	done	


# external builds
sdif:
		$(MAKE) -C $(HOME)/src/SDIF

test:		$(mexfiles) testrw testl t1.sdif

# unsetting the display variable prevents the start of java environment
# for matlab 6
testrw:	  gen
	@rm -f error.found 
	@unset DISPLAY;\
	for cmd in $(MATLABCMDS); do\
		echo "testrw;quit"  |  $$cmd -nosplash; \
	        if [ -f error.found ];then \
	           echo error running testrw with $$cmd ;\
		   rm -f error.found; \
		   exit 1; \
		fi; \
	done	
testl:	gen
	@unset DISPLAY;\
	for cmd in $(MATLABCMDS); do\
		echo "testl;quit" | $$cmd -nosplash |  sed -e "1,5 s/.*//" >gen/testl.out; \
	        if [ "`diff data/testlm5.out gen/testl.out`" != "" -a "`diff data/testlm6.out gen/testl.out`" != "" ]; then \
		  echo error running testl with $$cmd; \
		  exit 1; \
		else \
		  echo "OK running testl with $$cmd !!";\
		fi;\
	done

t1.sdif:  gen
	@unset DISPLAY;\
	SDIFTYPES=testtypes.styp;export SDIFTYPES;\
	for cmd in $(MATLABCMDS); do\
		echo "testwrite('gen/$@'), loadsdiffile('gen/$@');quit" | $$cmd -nosplash >/dev/null;\
		querysdif gen/$@ | fgrep -v Version >gen/t1.sdif.out; \
		sdiftotext -i gen/$@ | fgrep -v Version >>gen/t1.sdif.out;\
	        if [ "`diff data/t1.sdif.ascii gen/t1.sdif.out`" != "" ]; then \
		  echo error running testwrite/loadsdiffile with $$cmd; \
		  exit 1; \
		else \
		  echo "OK running t1.sdif with $$cmd !!"; \
		fi;\
	done

install:	all $(INSTALL_DIR)
	rm -f $(INSTALLED_FILES)
	cp -f $(INSTALL_FILES) $(INSTALL_DIR)
	chmod g+w $(INSTALLED_FILES) # assure ownership-change

instdef installdef installdefs:
	cp -f mex.makedefs $(INSTALL_DIR)

dist:		all
	-tar cvfz $(DIST_BASE).tar.gz $(DIST_FILES)
	tar cvfz $(DIST_BASE)-src.tar.gz $(SRC_DIST_FILES)
	@echo now do: cvs tag $(CVSTAG)
publish:	dist
	ssh $(DIST_HOST) cp -f $(DIST_COPYFILES:%=$(PWD)/%) $(DIST_DIR)

clean: mexclean
	-rm -f mexsuf.inc.$(ARCH) gen/*

mexclean:
	-rm -f $(mexfiles)

ls:	   
	@echo $(mexfiles)
ll:	   
	@echo $(MATLABCMDS)
	@echo $(INCL)

# report missing symbols in loadsdif lib
mis missing:	$(mexfiles)
	for mexf in $(mexfiles); do\
	  echo; echo [missing objects in $$mexf];\
		nm $$mexf | grep ' U ';\
	done

gen: 
	mkdir gen

$(INSTALL_DIR): 
	mkdir $(INSTALL_DIR)

tags:		TAGS
TAGS:		$(allsrc)
	etags $(allsrc)
