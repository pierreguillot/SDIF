# $Id: Makefile.am,v 1.4 2003-04-06 16:30:53 roebel Exp $
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
# Revision 1.3  2003/03/07 17:38:41  roebel
# Updated for correct handling of distribution and new layout of c++ compilation in SDIF.
#
# Revision 1.2  2003/03/04 00:56:12  roebel
# Fixed path problems for configuration not inside top_srcdir
#
# Revision 1.1  2003/03/03 10:04:25  roebel
# Adapted for autoconf/automake...
#

EXTRA_PROGRAMS=
bin_PROGRAMS = easdifRmNVT tutorini tutofini

EASDIR=../easdif
easdifRmNVT_SOURCES = easdifRmNVT.cpp
easdifRmNVT_LDADD = -lEasdif -lm
easdifRmNVT_LDFLAGS = -L$(EASDIR)
easdifRmNVT_DEPENDENCIES=$(EASDIR)/libEasdif.la

tutorini_SOURCES = tutorini.cpp
tutorini_LDADD = -lEasdif -lm
tutorini_LDFLAGS = -L$(EASDIR)
tutorini_DEPENDENCIES=$(EASDIR)/libEasdif.la

tutofini_SOURCES = tutofini.cpp
tutofini_LDADD = -lEasdif -lm
tutofini_LDFLAGS = -L$(EASDIR)
tutofini_DEPENDENCIES=$(EASDIR)/libEasdif.la


CXXFLAGS = -g -I$(top_srcdir) -I..  -I../SDIF/include -I$(top_srcdir)/SDIF/include 












