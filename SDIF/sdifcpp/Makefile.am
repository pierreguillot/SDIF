# $Id: Makefile.am,v 1.3 2003-07-21 13:41:35 tisseran Exp $
#
# ./sdifcpp/Makefile.am		March 2003		Axel Roebel
#
# Source file for GNU automake, generates Makefile.in.
#
#
# This file is part of the IRCAM SDIF Library (http://www.ircam.fr/sdif)
#
# Copyright (C) 1998-2003 by IRCAM - Centre Pompidou, Paris, France.
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
# 
# See file COPYING for further informations on licensing terms.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#
# For any information regarding this and other IRCAM software, please
# send email to:  sdif@ircam.fr


# $Log: not supported by cvs2svn $
# Revision 1.2  2003/03/18 14:56:52  roebel
# Use libtool to built conveniance library.
#
# Revision 1.1  2003/03/07 17:34:09  roebel
# cplusplus compilation is moved in its own directory.
#



EXTRA_LTLIBRARIES = libsdif-cpp.la 
lib_LTLIBRARIES = @SELLIBCPP@

FORMES_H=host_architecture.h  preincluded.h UniversalEnvVar.h  XpGuiCalls.h
FORMES_INCLUDE=/u/formes/share/include

SDIF_H = ../include/sdif_version.h $(top_srcdir)/include/sdif.h \
	$(addprefix $(top_srcdir)/sdif/,SdifCheck.h 	SdifErrMess.h	\
	SdifConvToText.h 	\
	SdifError.h 	SdifFGet.h 	SdifFPrint.h 		\
	SdifFPut.h 	SdifFRead.h	SdifFScan.h 		\
	SdifFWrite.h 	SdifFile.h	SdifTimePosition.h 	\
	SdifFrame.h 	SdifFrameType.h SdifGlobals.h 		\
	SdifHard_OS.h 	SdifHash.h	SdifHighLevel.h 	\
	SdifList.h 	SdifMatrix.h 	SdifMatrixType.h	\
	SdifMemory.h	SdifNameValue.h	SdifPreTypes.h 		\
	SdifPrint.h 	SdifSelect.h	SdifRWLowLevel.h	\
	SdifStreamID.h	SdifString.h 	SdifSignatureTab.h	\
	SdifTest.h 	SdifTextConv.h 	sdiftypes.h	        \
	project_preinclude.h )	../sdif/config.h \
	$(top_srcdir)/sdif/SdifVersion.h 

noinst_HEADERS = $(SDIF_H) $(addprefix $(top_srcdir)/sdif/,${FORMES_H})

libsdif_cpp_la_SOURCES = \
	SdifHash.cpp       SdifGlobals.cpp      SdifSignatureTab.cpp   \
	SdifMemory.cpp     SdifNameValue.cpp    SdifMatrixType.cpp     \
	SdifStreamID.cpp   SdifMatrix.cpp       SdifFrame.cpp          \
	SdifTest.cpp       SdifTimePosition.cpp SdifFile.cpp           \
	SdifFGet.cpp       SdifFRead.cpp        SdifFScan.cpp          \
	SdifFPrint.cpp     SdifFWrite.cpp       SdifTextConv.cpp       \
	SdifHard_OS.cpp    SdifErrMess.cpp      SdifList.cpp           \
	SdifSelect.cpp     SdifHighLevel.cpp    SdifCheck.cpp          \
	SdifString.cpp     SdifError.cpp        SdifFrameType.cpp      \
	SdifPrint.cpp      SdifRWLowLevel.cpp   SdifFPut.cpp           \
	SdifConvToText.cpp SdifPreTypes.cpp

libsdif_cpp_la_LDFLAGS=-no-undefined -release @VERSION@ -rpath $(libdir)

CLEANFILES = $(libsdif_cpp_la_SOURCES) SdifVersion.h
sdiftype = $(datadir)/SdifTypes.STYP

INCLUDES = -I../include -I$(top_srcdir)/include -I../sdif -I$(top_srcdir)/sdif -D_SdifTypesFileName=\"$(sdiftype)\" -DUSE_CONFIGURE


$(libsdif_cpp_la_SOURCES) : $(FORMES_H:%=$(top_srcdir)/sdif/%)

$(libsdif_cpp_la_SOURCES) : %.cpp : ../sdif/%.c 
	cp  $< $@

BUILT_SOURCES = $(libsdif_cpp_la_SOURCES) 

# dont use c compiler implicite rule which would override use of c++ 
%.o : %.c

#
# EXTRA rules and dependencies to generate the checksum file SdifVersion.h
#

SdifFile.o : SdifVersion.h

# Versionsrc := $(libsdif_a_SOURCES) $(subst config.h,,$(subst $(top_srcdir)/sdif/SdifVersion.h,,$(noinst_HEADERS) )) 
Versionsrc := $(libsdif_cpp_la_SOURCES) $(subst ../sdif/config.h,,$(subst $(top_srcdir)/sdif/SdifVersion.h,,$(noinst_HEADERS) )) 


SdifVersion.h : $(Versionsrc)
	@echo  "Updating  Library CKSUM"
	@echo "/* Automatically generated File do not edit */" >$@
	@echo '#define AUTOCKSUM "$$SDIFChecksum: '$(shell CMD_ENV=xpg4;export CMD_ENV;cksum $^ | cut -f1 -d' '|cksum |cut -f1 -d' ')' $$ IRCAM $$SDIFdate: "__DATE__" $$"'>>$@


#
# EXTRA FOR IRCAM STUFF
#

if ISATIRCAM
ircam_cmp = $(FORMES_INCLUDE)/$(notdir $(1))
else
ircam_cmp = 
endif

$(FORMES_H:%=$(top_srcdir)/sdif/%) : $(top_srcdir)/sdif/% : $(call ircam_cmp,%)
	cp $< $@

