# $Id: Makefile,v 1.5 2000-08-17 14:34:45 schwarz Exp $
#
# $Log: not supported by cvs2svn $
# Revision 1.4  2000/08/09  14:43:48  schwarz
# Put all SDIF types into XML format.  Lots of descriptions still missing.
# Full "about this document" info.
#
# Revision 1.3  2000/08/01  10:15:34  schwarz
# Added clean, new, .DELETE_ON_ERROR.
#
# Revision 1.2  2000/08/01  09:37:00  schwarz
# Targets for both files, use standard Perl modules.
#

ifeq ($(USER), schwarz)
  mynode	= /u/formes/schwarz/soft/perl/XML-Node-0.10
  PERLFLAGS	= -I$(mynode) -MNode
endif

PERL		= perl $(PERLFLAGS)


.DELETE_ON_ERROR:	# remove targets when the command failed


all:		sdiftypes.styp sdiftypes.html
new:		clean all

sdiftypes.styp:	sdiftypes.xml xmltostyp.pl
		$(PERL) xmltostyp.pl $< >$@

sdiftypes.html:	sdiftypes.xml xmltohtml.pl
		$(PERL) xmltohtml.pl $< $@ toc.html
#		 generate in doc?

install:	sdiftypes.styp
#		cp -p ...

clean:
		rm -f sdiftypes.styp sdiftypes.html

show:		sdiftypes.html
		netscape -remote 'reload(file:$(PWD)/$<)'

test:		all
		querysdif -t sdiftypes.styp </dev/null

db:
		DEBUG=1 $(PERL) xmltohtml.pl sdiftypes.xml >sdiftypes.html
		netscape -remote 'reload(file:$(PWD)/sdiftypes.html)'
