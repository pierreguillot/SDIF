# $Id: Makefile,v 1.10 2003-04-18 17:45:23 schwarz Exp $
#
# $Log: not supported by cvs2svn $
# Revision 1.9  2003/04/18 16:00:22  schwarz
# Generate separate large module SDIF.pm with all
# low-level sdif lib wrappers.
#
# Revision 1.8  2003/04/18 15:41:18  schwarz
# Don't include all definitions from sdif.h, only the ones needed by
# easdif, defined in sdifdefine.i.  -> smaller wrapper size:
# .cxx	 880k -> 130k
# .o	1000k -> 210k
# .pm	 123k -> 8k
#
# Revision 1.7  2003/04/18 14:31:27  schwarz
# Added typemap for string to SdifSignature conversion (input arg):
# Setting/writing data works --> wrapper complete, but rudimentary.
#
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


PERL5_INCLUDE	= /usr/lib/perl5/5.6.1/i386-linux/CORE/
INCL		= -I/usr/local/include -I$(PERL5_INCLUDE) \
		  -I.. -I../SDIF/include -I../SDIF/sdif
EASDIF_LIB_PATH	= ../easdif/.libs
# dbflag = -g


modules		= eaSDIF SDIF
incfiles	= sdifdefines.i sdiftypemap.i
easdifincbase	= entity frame matrix namevaluetable
easdifincfiles	= $(easdifincbase:%=../easdif/sdif%.h)
sdifincfiles	= ../SDIF/include/sdif.h

define swig
	swig -c++ -perl5 -shadow -o $@ $<
endef


all:	eaSDIF.so SDIF.so

eaSDIF_wrap.cxx:	easdif.i $(easdifincfiles) $(incfiles)
			$(swig)

SDIF_wrap.cxx:		sdif.i   $(sdifincfiles)   $(incfiles)
			$(swig)


%_wrap.o: %_wrap.cxx
	g++ -c $(dbflag) -fpic -Dexplicit= -fno-strict-aliasing $(INCL) $<

%.so: 	%_wrap.o
	g++ -shared $(EASDIF_LIB_PATH)/libEasdif.so $< -o $@

test:	eaSDIF.pm eaSDIF.so SDIF.so
	LD_LIBRARY_PATH=$(EASDIF_LIB_PATH):$(LD_LIBRARY_PATH) \
	perl -I. test.pl

clean:
	-rm -f eaSDIF_wrap.* SDIF_wrap.* eaSDIF.pm SDIF.pm eaSDIF.so SDIF.so
