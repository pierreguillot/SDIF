# $Id: Makefile.am,v 1.11 2002-10-30 15:54:01 schwarz Exp $
#
# ./Makefile.am		September 2000			Patrice Tisserand
#
# Source file for GNU automake, generates Makefile.in.
#
#
# This file is part of the IRCAM SDIF Library (http://www.ircam.fr/sdif)
#
# Copyright (C) 1998, 1999, 2000 by IRCAM - Centre Pompidou, Paris, France.
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
# Revision 1.10  2002/08/29 16:07:09  tisseran
# Add support of pkg-config for SDIF library.
# After installation, now you can use:
# pkg-config --cflags --libs sdif
# to have correct compilation flags
#
# Revision 1.9  2001/05/04 14:07:26  tisseran
# Liitle fix:
# - Change a c++ commentary in a c one (sdif.h line:1532)
# - Change publihs rules of main Makefile, now name of asrc archive on www.ircam.fr/sdif/download
# is SDIF-(version)-src.tar.gz
#
# Revision 1.8  2001/05/02 09:34:36  tisseran
# Change License from GNU Public License to GNU Lesser Public License.
#
# Revision 1.7  2001/04/26 14:51:59  tisseran
# ARRGGHHH how can I remove all this stupid log@!@!@@!
#
# Revision 1.6  2001/04/26 14:47:01  tisseran
# Correct a stupid error in previous log (arrggghh DON'T USE C COMMENTARY in cvs log).
# Correct Makefile.am, change cvstag rules:
# cvstags:
# 	cvs -F tags $(CVSTAGS)
# Add some explication in file ChangeLog
#
# Revision 1.5  2001/04/20 17:55:07  schwarz
# Added publish target that sets the cvs tag, generates the distribution
# and copies it to the download directory on the web host.
# Call as "make DISTHOST=yourwebhost publish"
#
# Revision 1.4  2001/04/20 17:35:45  schwarz
# Oops, make doesn't like a rule and a variable of the same name.
#
# Revision 1.3  2001/04/20 17:33:35  schwarz
# Added cvstag target and publish target to publish a new version.
# Removed all Makefile.in from CVS.
#
# Revision 1.2  2000/10/27 20:02:42  roebel
# autoconf merged back to main trunk
#
# Revision 1.1.2.4  2000/10/26  17:27:16  schwarz
# Added header with copyright and links to all Makefile.am and configure.in.


SUBDIRS = sdif include tools data test

sdifincdir = @top_srcdir@/include

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = sdif.pc

grep:
	grep $a sdif/*.[ch] /dev/null

doc: 
	cd doc; $(MAKE) -f Makefile.doc

ircam_deps:
	@cd sdif; $(MAKE) ircam_deps

test	:	check

# tag the cvsrepository with the current version 
# (to exactly reconstitute a published version)
CVSTAG	      = $(shell echo $(PACKAGE)_lib_version_$(VERSION) | tr . _)
cvstag:
		cvs tag -F $(CVSTAG)

# call this as "make DISTHOST=yourwebhost publish" to publish a new version
# this might work differently for other places
# todo: configure DISTHOST
DISTDIR       = sdif/download
DISTFILE      = $(distdir)-src.tar.gz
EXTRA_DIST    = README COPYING AUTHORS INSTALL ChangeLog doc
DISTFILES     = $(DISTFILE) $(EXTRA_DIST)

# use this to tag the cvs project, make a source distribution, and
# copy it to the web server
publish:	cvstag srcdist
		cat <<END_OF_FTP_COMMANDS \
$$< \
$$< \
cd $(DISTDIR) \
mput $(DISTFILES) \
END_OF_FTP_COMMANDS \
		| ftp $(DISTHOST)

# use this to make a source distribution!
srcdist:	dist
		mv $(distdir).tar.gz $(DISTFILE)


.PHONY:		grep doc ircam_deps test publish srcdist cvstag
