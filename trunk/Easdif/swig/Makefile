# $Id: Makefile,v 1.2 2003-04-17 09:54:12 schwarz Exp $
#
# $Log: not supported by cvs2svn $
# Revision 1.1  2003/04/17 09:51:10  schwarz
# First working swig generated perl extension!
# Classes re-declared in easdif.i, simple method without shadow classes.
# Doesn't do much except counting frames.


PERL5_INCLUDE = /usr/lib/perl5/5.6.1/i386-linux/CORE/

INCL = -I/usr/local/include -I$(PERL5_INCLUDE) -I.. -I../SDIF/include -I../SDIF/sdif

wrap = easdif_wrap.cxx

all:	SDIF.so

$(wrap):	easdif.i Makefile
	swig -c++ -perl5 $<
#	swig -c++ -perl5 -shadow $<

easdif_wrap.o:	$(wrap)
	g++ -c -fpic -Dexplicit=  $(wrap)  -fno-strict-aliasing $(INCL)
#	g++ $(INCL) -DHAVE_CONFIG_H -DUSE_CONFIGURE -g2 $< -o $@

SDIF.so:	easdif_wrap.o
	g++ -shared ../easdif/.libs/libEasdif.so easdif_wrap.o -o SDIF.so

test:
	LD_LIBRARY_PATH=../easdif/.libs:$(LD_LIBRARY_PATH) perl -I. ./test.pl

clean: 
	rm $(wrap)
