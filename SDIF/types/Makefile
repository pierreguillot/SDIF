
mynode		= /u/formes/schwarz/soft/perl/XML-Node-0.10
PERLFLAGS	= -I$(mynode) -MNode
PERL		= perl $(PERLFLAGS)

test:
	$(PERL) xmltohtml.pl sdiftypes.xml >sdiftypes.html
	netscape -remote 'reload(file:$(PWD)/sdiftypes.html)'
	cat sdiftypes.html


db:
	DEBUG=1 $(PERL) xmltohtml.pl sdiftypes.xml >sdiftypes.html
	netscape -remote 'reload(file:$(PWD)/sdiftypes.html)'


sdiftypes.styp:	sdiftypes.xml
		xmltohtml.pl $< >$@
