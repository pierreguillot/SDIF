# $Id: Makefile,v 1.3 1998-11-10 15:36:15 schwarz Exp $
#
# make general	
#
# $Log: not supported by cvs2svn $


PRJ_DIR := $(shell pwd)

include Makefile.preamble


#
# compile
#

all: M_lib M_tosdif M_sdiftotext M_sdifextract


M_lib:
	@cd libsdif ; $(MAKE)

M_tosdif:
	@cd tosdif ; $(MAKE)

M_sdiftotext:
	@cd sdiftotext ; $(MAKE)

M_sdifextract:
	@cd sdifextract ; $(MAKE)


#
# clean
#

clean_lib:
	@cd libsdif ; $(MAKE)  clean

clean_tosdif:
	@cd tosdif; $(MAKE) clean

clean_sdiftotext:
	@cd sdiftotext; $(MAKE) clean

clean_sdifextract:
	@cd sdifextract; $(MAKE) clean

clean: clean_lib clean_tosdif clean_sdiftotext clean_sdifextract



#
# install
#

install_lib:
	@cd libsdif ; $(MAKE) install

install_tosdif:
	@cd tosdif; $(MAKE) install

install_sdiftotext:
	@cd sdiftotext; $(MAKE) install

install_sdifextract:
	@cd sdifextract; $(MAKE) install

install: install_lib install_tosdif install_sdiftotext install_sdifextract



#
# uninstall
#

uninstall_lib:
	@cd libsdif ; $(MAKE) uninstall

uninstall_tosdif:
	@cd tosdif; $(MAKE) uninstall

uninstall_sdiftotext:
	@cd sdiftotext; $(MAKE) uninstall

uninstall_sdifextract:
	@cd sdifextract; $(MAKE) uninstall

uninstall: uninstall_lib uninstall_tosdif uninstall_sdiftotext \
	   uninstall_sdifextract
