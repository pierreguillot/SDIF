# $Id: Makefile.am,v 1.9 2004-07-26 10:28:16 roebel Exp $
#
# ./Easdif/test/Makefile.am	Feb. 2003 Axel Roebel
#
# Source file for GNU automake, generates Makefile.in.
#
#  This file is part of the IRCAM EASDIF Library (http://www.ircam.fr/sdif)
# 
#  Copyright (C) 2002-2003 by IRCAM-Centre Georges Pompidou, Paris, France.
# 
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public License
#  as published by the Free Software Foundation; either version 2
#  of the License, or (at your option) any later version.
# 
#  See file COPYING for further informations on licensing terms.
# 
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Lesser General Public License for more details.
# 
#  You should have received a copy of the GNU Lesser General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
# 
# 
#  The EASDIF library provides an Easy Api for IRCAMs SDIF library. 
# 

# $Log: not supported by cvs2svn $
# Revision 1.8  2004/03/19 01:03:26  roebel
# Finalized support for --enable-ddebug
#
# Revision 1.7  2003/07/10 17:19:43  roebel
# Finished gcc3_2 suffix support for test programs
#
# Revision 1.6  2003/07/09 21:19:55  roebel
# Provide configure switch to add gccversion to library name to distinguish
# gcc3.2 from gcc2.96 compiled libraries.
#
# Revision 1.5  2003/04/08 23:53:02  roebel
# Dont install test programs
#
# Revision 1.4  2003/04/06 16:30:53  roebel
# Added license info
#
# Revision 1.3  2003/03/07 17:38:41  roebel
# Updated for correct handling of distribution and new layout of c++ compilation in SDIF.
#
# Revision 1.2  2003/03/04 00:56:12  roebel
# Fixed path problems for configuration not inside top_srcdir
#
# Revision 1.1  2003/03/03 10:04:25  roebel
# Adapted for autoconf/automake...
#

EXTRA_PROGRAMS= easdifRmNVT easdifRmNVT-debug
bin_PROGRAMS = @SELPROG@
noinst_PROGRAMS =  tutorini tutofini

if GCCSUFFIX
GCCSUFFIX = _gcc3_2
else
GCCSUFFIX =
endif 

EASDIR=../easdif
easdifRmNVT_SOURCES = easdifRmNVT.cpp
easdifRmNVT_LDADD = -lEasdif$(GCCSUFFIX) -lm
easdifRmNVT_LDFLAGS = -L$(EASDIR)
easdifRmNVT_DEPENDENCIES=$(EASDIR)/@SELLIB@

easdifRmNVT_debug_SOURCES = easdifRmNVT.cpp
easdifRmNVT_debug_LDADD = -lEasdif-debug -lm
easdifRmNVT_debug_LDFLAGS = -L$(EASDIR)
easdifRmNVT_debug_DEPENDENCIES=$(EASDIR)/@SELLIB@

tutorini_SOURCES = tutorini.cpp
tutorini_LDADD = -lEasdif$(GCCSUFFIX) -lm
tutorini_LDFLAGS = -L$(EASDIR)
tutorini_DEPENDENCIES=$(EASDIR)/@SELLIB@

tutofini_SOURCES = tutofini.cpp
tutofini_LDADD = -lEasdif$(GCCSUFFIX) -lm
tutofini_LDFLAGS = -L$(EASDIR)
tutofini_DEPENDENCIES=$(EASDIR)/@SELLIB@


CXXFLAGS = @CXXFLAGS@ -I$(top_srcdir) -I..  -I../SDIF/include -I$(top_srcdir)/SDIF/include 












