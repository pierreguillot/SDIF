# $Id: Makefile,v 1.7 2003-04-18 14:31:27 schwarz Exp $
#
# $Log: not supported by cvs2svn $
# Revision 1.6  2003/04/17 12:20:39  schwarz
# Output (return value) mapping for SdifSignature works!
#
# Revision 1.5  2003/04/17 11:15:01  schwarz
# Swigging sdif.h works (with some warnings), but that's not yet it...
#
# Revision 1.4  2003/04/17 11:03:52  schwarz
# Swigged all easdif classes!
# Now going on to map sdif basic types.
#
# Revision 1.3  2003/04/17 10:05:48  schwarz
# The same with shadow classes (= proper memory management and methods).
#
# Revision 1.2  2003/04/17 09:54:12  schwarz
# Test target works when path to easdif shared lib is given.
#
# Revision 1.1  2003/04/17 09:51:10  schwarz
# First working swig generated perl extension!
# Classes re-declared in easdif.i, simple method without shadow classes.
# Doesn't do much except counting frames.


PERL5_INCLUDE = /usr/lib/perl5/5.6.1/i386-linux/CORE/

INCL = -I/usr/local/include -I$(PERL5_INCLUDE) -I.. -I../SDIF/include -I../SDIF/sdif

sdifincbase  = entity frame matrix namevaluetable
sdifincfiles = $(sdifincbase:%=../easdif/sdif%.h) ../SDIF/include/sdif.h

wrap = easdif_wrap.cxx
# dbflag = -g

all:			SDIF.pm SDIF.so

SDIF.pm $(wrap):	easdif.i $(sdifincfiles) sdifsignature.i
	swig -c++ -perl5 -shadow $<
#	swig -c++ -perl5 $<

easdif_wrap.o:		$(wrap)
	g++ -c $(dbflag) -fpic -Dexplicit= $(wrap) -fno-strict-aliasing $(INCL)

SDIF.so:		easdif_wrap.o
	g++ -shared ../easdif/.libs/libEasdif.so easdif_wrap.o -o SDIF.so

test:			SDIF.pm SDIF.so 
	LD_LIBRARY_PATH=../easdif/.libs:$(LD_LIBRARY_PATH) \
	perl -I. test.pl

clean: 
	rm $(wrap)
