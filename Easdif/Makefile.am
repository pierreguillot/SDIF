# $Id: Makefile.am,v 1.8 2003-04-09 11:55:29 roebel Exp $
#
# ./Makefile.am		Feb. 2003     Axel Roebel
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
# Revision 1.7  2003/04/08 23:22:34  roebel
# Added determination of SDIF version
#
# Revision 1.6  2003/04/08 18:27:36  roebel
# Added doc tar to published files.
# Changed name of published tar to include SDIF with version.
# Configured PUBLISH_HOST
# Rename conflicting files that have common names in SDIF and Easdif
# to coexist in download directory.
#
# Revision 1.5  2003/04/06 16:47:51  roebel
# Added copyright notice.
#
# Revision 1.4  2003/04/03 13:15:40  schwarz
# Added doc and test targets.
#
# Revision 1.3  2003/03/07 17:38:41  roebel
# Updated for correct handling of distribution and new layout of c++ compilation in SDIF.
#
# Revision 1.2  2003/03/04 00:50:01  roebel
# Changed src difrectory to easdif, fixed distribution of SDIF
#
# Revision 1.1  2003/03/03 10:03:24  roebel
# Adapted for autoconf/automake...
#

SUBDIRS =  SDIF easdif  test 
EXTRA_DIST = Doxyfile.in README.shared README.doc

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = Easdif.pc

doc:
	doxygen Doxyfile

test:	check

grep:
	grep $a easdif/*.cpp easdif/*.h /dev/null


DOCTAR = $(PACKAGE)-$(VERSION).doc.tgz

pubdoc: doc
	tar -cvzf $(DOCTAR) doc 

#
# DISTRIBUTION
#

# base name of distribution (override automake generated variable to make source distribution!)
SDIFVERSION=$(shell grep "\#define VERSION" SDIF/sdif/config.h | cut -d\" -f2)

distdir		  = $(PACKAGE)-$(VERSION)-SDIF-$(SDIFVERSION)-src
PUBLISH_FILE      = $(distdir).tar.gz

#
# PUBLISH (on server)
#

# use this to check if the distribution works, tag the cvs project, 
# make a source distribution and rpm, and copy it to the web server
publish:	distcheck cvstag dist pubdoc publish-ftp 

# more files to publish (on server)
EXTRA_PUBLISH     = $(DOCTAR)

# call this as "make PUBLISH_HOST=yourwebhost publish" to publish a new version
# this might work differently for other places
# todo: configure PUBLISH_HOST
PUBLISH_HOST      = www.ircam.fr
PUBLISH_DIR       = sdif/download/
PUBLISH_FILES     = $(PUBLISH_FILE) $(PUBLISH_RPM) $(EXTRA_PUBLISH)
PUBLISH_FILES_MAP = COPYING INSTALL README AUTHORS ChangeLog
publish-ftp:	# (experimental)
		cat <<END_OF_FTP_COMMANDS \
$$< \
$$< \
cd $(PUBLISH_DIR) \
mput $(PUBLISH_FILES) \
nmap $1 $1-Easdif \
mput $(PUBLISH_FILES_MAP) \
END_OF_FTP_COMMANDS \
		| ftp $(PUBLISH_HOST)



# tag the cvsrepository with the current version 
# (to exactly reconstitute a published version)
CVSTAG	      = $(shell echo $(PACKAGE)_lib_version_$(VERSION) | tr . _)
cvstag:
		cvs tag -F $(CVSTAG)


.PHONY:		grep doc ircam_deps test publish pubdoc publish-ftp  cvstag

