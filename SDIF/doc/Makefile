# $Id: Makefile,v 2.2 1999-01-06 14:50:13 schwarz Exp $
#
#               Copyright (c) 1998 by IRCAM - Centre Pompidou
#                          All rights reserved.
#
#  For any information regarding this and other IRCAM software, please
#  send email to:
#                            manager@ircam.fr
#
#
# Makefile	4. November 1998	Diemo Schwarz
#
# Makefile for automatic documentation.
#
# $Log: not supported by cvs2svn $
# Revision 2.1  1998/12/21  18:26:48  schwarz
# Inserted copyright message.
#
# Revision 2.0  1998/11/29  11:20:51  virolle
# english documentation for SDIF norm.
#
# Revision 1.1  1998/11/10  14:56:40  schwarz
# Added makefile to generate cocoon documentation.
#

HERE		= $(subst /a/,/net/,$(PWD))
ABS_ROOT	= $(HERE)/..
XROOT		= $(HERE)/$*	# $* is what matches % in a pattern rule

COCOON		= cocoon
PREPROC		= gcc -E -P -ansi -x c 

makedependency	= # Makefile # when changing preprocessor options
headerdir	= $(ABS_ROOT)/sdif
headerfiles	= $(wildcard $(headerdir)/*.h)	# same as $(shell ls .../*.h)


#
# cocoon
#

outdirs		= intern extern

cocoon:		ext index-extern.html int index-intern.html 
abc:		index-intern.html index-extern.html

show:		showintern
show%:		%
		netscape -remote 'openURL(file:$(HERE)/$*/SDIF.html)'

int: 		intern/SDIF.lnk
ext: 		extern/SDIF.lnk


intern-defs	= "-DSENTINELS=nosentinels"  "-DMEMBERSENTINEL=" \
		  "-DCUSTOMIZE=preformatted"

extern-defs	= "-DSENTINELS="  "-DMEMBERSENTINEL=membersentinel /*DOC:" \
		  "-DCUSTOMIZE="


%/SDIF.lnk:	sdif.%.cocoon
		rm -f $*/*
		$(COCOON) -s -x $<   # -s: leave SDIF.lnk 

# generate cocoon config file for one big web page with all defs
sdif.%.cocoon:	sdif.src $(headerfiles) $(makedependency)
		$(PREPROC) $< -o $@  $($*-defs) \
			-DWEBROOT=$(XROOT) -DURLROOT=$(XROOT) \
			"-DLIBRARY=library SDIF $(ABS_ROOT)/sdif/ ."


# alphabetical list of all declarations

index-%.html:	%/SDIF.lnk
		sort $< | perl -n -e 's|\\#|$(XROOT)/SDIF.html#|; split "%"; print "$$_[2]<br>\n"' >$@


boldit:		intern/SDIF.lnk
		perl -n -e 'split "%"; print "%$$_[1]%<b>$$_[1]</b>%\n"' $< >$@


$(outdirs):
		mkdir $@

