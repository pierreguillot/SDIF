# $Id: Makefile,v 3.4 1999-10-07 15:22:52 schwarz Exp $
#
#               Copyright (c) 1998 by IRCAM - Centre Pompidou
#                          All rights reserved.
#
#  For any information regarding this and other IRCAM software, please
#  send email to:
#                            manager@ircam.fr
#
#
# make general	
#
# $Log: not supported by cvs2svn $
# Revision 3.3  1999/04/30  13:46:53  schwarz
# Fixed little mistake in install target.
#
# Revision 3.2  1999/04/21  14:17:42  schwarz
# Introduced list 'SUBS' of subdirectories, for which automatically
# all the subtargets M_, clean_, (un)install_ are generated.
#
# Revision 2.1  1998/12/21  18:26:39  schwarz
# Inserted copyright message.
#
# Revision 2.0  1998/11/29  11:28:29  virolle
# Added Visual C++ projects.
#
# Revision 1.3  1998/11/10  15:36:15  schwarz
# Added sdifextract.
#


PRJ_DIR := $(shell pwd)

include Makefile.preamble


SUBS		= libsdif tosdif sdiftotext sdifextract querysdif

.PHONEY:	$(SUBS)

M_all	 	= $(SUBS:%=M_%)
clean_all 	= $(SUBS:%=clean_%)
install_all	= $(SUBS:%=install_%)
uninstall_all	= $(SUBS:%=uninstall_%)


#
# compile
#

all: 	$(M_all)


M_%:
	@cd $*; $(MAKE) 
M_lib:
	@cd libsdif; $(MAKE)
$(SUBS) lib:
	@cd $@; $(MAKE)

debug db:
	$(MAKE) DEBUG=1 all
release:
	$(MAKE) DEBUG=0 all


#
# clean
#

clean_%:
	@cd $*; $(MAKE) clean
clean_lib:
	@cd libsdif; $(MAKE) clean

clean:  $(clean_all)



#
# install
#


install_%:
	@cd $*; $(MAKE) install
install_lib instlib:
	@cd libsdif ; $(MAKE) install

install:  $(install_all)



#
# uninstall
#


uninstall_%:
	@cd $*; $(MAKE) uninstall
uninstall_lib:
	@cd libsdif; $(MAKE) uninstall

uninstall:  $(uninstall_all)



#
# support
#

tags:
	etags sdif/*.[ch]
