# $Id: Makefile.am,v 1.1.2.5 2000-10-26 17:27:39 schwarz Exp $
#
# ./sdif/Makefile.am		September 2000			Patrice Tisserand
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
lib_LIBRARIES = libsdif.a
FORMES_H=host_architecture.h preincluded.h UniversalEnvVar.h host_configure.h XpGuiCalls.h
FORMES_INCLUDE=/u/formes/share/include

noinst_HEADERS = *.h ${FORMES_H}
libsdif_a_dir = $(prefix)/libsdif

libsdif_a_SOURCES =  SdifString.c      SdifHash.c     SdifGlobals.c      SdifSignatureTab.c \
		     SdifError.c       SdifMemory.c   SdifNameValue.c    SdifMatrixType.c \
		     SdifFrameType.c   SdifStreamID.c SdifMatrix.c       SdifFrame.c \
		     SdifPrint.c       SdifTest.c     SdifTimePosition.c SdifFile.c \
		     SdifRWLowLevel.c  SdifFGet.c     SdifFRead.c        SdifFScan.c \
		     SdifFPut.c        SdifFPrint.c   SdifFWrite.c       SdifTextConv.c \
		     SdifConvToText.c  SdifHard_OS.c  SdifErrMess.c      SdifList.c \
		     SdifPreTypes.c    SdifSelect.c   SdifHighLevel.c    SdifCheck.c 
sdiftype = $(datadir)/SdifTypes.STYP
INCLUDES = -D_SdifTypesFileName=\"$(sdiftype)\" -DUSE_CONFIGURE

$(libsdif_a_SOURCES) : % : @IRCAMDEPS@

# EXTRA FOR IRCAM STUFF
ircam_deps : ${FORMES_H}

host_architecture.h:
	cp ${FORMES_INCLUDE}/host_architecture.h .

preincluded.h :
	cp ${FORMES_INCLUDE}/preincluded.h .

UniversalEnvVar.h : 
	cp ${FORMES_INCLUDE}/UniversalEnvVar.h .

host_configure.h: 
	cp ${FORMES_INCLUDE}/host_configure.h .

XpGuiCalls.h:
	cp ${FORMES_INCLUDE}/XpGuiCalls.h .
