# $Id: Makefile.am,v 1.5 2000-10-31 11:46:32 roebel Exp $
#
# ./tools/Makefile.am		September 2000			Patrice Tisserand
#
# Source file for GNU automake, generates Makefile.in.
#
#
# This file is part of the IRCAM SDIF Library (http://www.ircam.fr/sdif)
#
# Copyright (C) 1998, 1999, 2000 by IRCAM - Centre Pompidou, Paris, France.
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
# 
# See file COPYING for further informations on licensing terms.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#
# For any information regarding this and other IRCAM software, please
# send email to:  sdif@ircam.fr
#
# $Log: not supported by cvs2svn $
# Revision 1.4  2000/10/30  19:05:54  roebel
# Added support for relative path compilation (VPATH)
# Added support for debugging feature in configure.in and Makefiles
#
# Revision 1.3  2000/10/30  14:44:02  roebel
# Moved all tool sources into central tools directory and added config.h to sources
#
# Revision 1.2  2000/10/27  20:04:17  roebel
# autoconf merged back to main trunk
#
# Revision 1.1.2.2  2000/10/26  17:27:47  schwarz
# Added header with copyright and links to all Makefile.am and configure.in.
#

EXTRA_PROGRAMS=querysdif-debug sdifextract-debug sdiftotext-debug tosdif-debug querysdif sdifextract sdiftotext tosdif
bin_PROGRAMS = @PROGLIST@

querysdif_debug_SOURCES=querysdif.c
sdifextract_debug_SOURCES=sdifextract.c
sdiftotext_debug_SOURCES=sdiftotext.c
tosdif_debug_SOURCES=tosdif.c

querysdif_debug_LDADD = -lsdif-debug
querysdif_debug_LDFLAGS = -L../sdif
querysdif_debug_DEPENDENCIES=../sdif/libsdif-debug.a
sdifextract_debug_LDADD = -lsdif-debug
sdifextract_debug_LDFLAGS = -L../sdif
sdifextract_debug_DEPENDENCIES=../sdif/libsdif-debug.a
sdiftotext_debug_LDADD = -lsdif-debug
sdiftotext_debug_LDFLAGS = -L../sdif
sdiftotext_debug_DEPENDENCIES=../sdif/libsdif-debug.a
tosdif_debug_LDADD = -lsdif-debug
tosdif_debug_LDFLAGS = -L../sdif
tosdif_debug_DEPENDENCIES=../sdif/libsdif-debug.a

querysdif_LDADD = -lsdif
querysdif_LDFLAGS = -L../sdif
querysdif_DEPENDENCIES=../sdif/libsdif.a
sdifextract_LDADD = -lsdif
sdifextract_LDFLAGS = -L../sdif
sdifextract_DEPENDENCIES=../sdif/libsdif.a
sdiftotext_LDADD = -lsdif
sdiftotext_LDFLAGS = -L../sdif
sdiftotext_DEPENDENCIES=../sdif/libsdif.a
tosdif_LDADD = -lsdif
tosdif_LDFLAGS = -L../sdif
tosdif_DEPENDENCIES=../sdif/libsdif.a

INCLUDES = -I$(top_srcdir)/include

