#!/bin/sh
# $Id: autogen.sh,v 1.3 2003-04-03 12:39:45 schwarz Exp $
#
# $Log: not supported by cvs2svn $

rm -f aclocal.m4 config.* install-sh libtool ltconfig ltmain.sh missing mkinstalldirs
set -x

#aclocal -I m4
aclocal
# On Mac OS X, libtoolize is glibtoolize
libtoolize --force --automake --copy || glibtoolize --force --automake --copy
autoconf
automake --foreign --add-missing --copy
(
    cd SDIF  &&  ./autogen.sh
)
