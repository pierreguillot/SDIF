# $Id: Makefile,v 1.2 2000-08-01 09:37:00 schwarz Exp $
#
# $Log: not supported by cvs2svn $

ifeq ($(USER), schwarz)
  mynode	= /u/formes/schwarz/soft/perl/XML-Node-0.10
  PERLFLAGS	= -I$(mynode) -MNode
endif

PERL		= perl $(PERLFLAGS)


all:		sdiftypes.styp sdiftypes.html

sdiftypes.styp:	sdiftypes.xml xmltostyp.pl
		$(PERL) xmltostyp.pl $< >$@

sdiftypes.html:	sdiftypes.xml xmltohtml.pl
		$(PERL) xmltohtml.pl $< >$@
#		 generate in doc?

install:	sdiftypes.styp
#		cp -p ...

show:
		$(PERL) xmltohtml.pl sdiftypes.xml >sdiftypes.html
		netscape -remote 'reload(file:$(PWD)/sdiftypes.html)'
test:		show
		cat sdiftypes.html

db:
		DEBUG=1 $(PERL) xmltohtml.pl sdiftypes.xml >sdiftypes.html
		netscape -remote 'reload(file:$(PWD)/sdiftypes.html)'
