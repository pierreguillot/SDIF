# $Id: Makefile.am,v 1.18 2003-04-09 12:28:25 roebel Exp $
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
# Revision 1.17  2003/04/06 16:28:57  roebel
# Added to CVS
#
# Revision 1.16  2003/03/07 17:55:01  roebel
# Added switch for doc compilation.
#
# Revision 1.15  2003/03/07 17:31:37  roebel
# cplusplus compilation is moved in its own directory.
#
# Revision 1.14  2002/10/31 16:35:36  schwarz
# Added sdif.vcproj
# More info for dist.
#
# Revision 1.13  2002/10/30 17:25:33  schwarz
# Ouf, finally proper distribution building: a src.tar.gz and a binary
# rpm (thanks to Patrice, but use with caution).
# publish-ftp doesn't actually work...
#
# Revision 1.12  2002/10/30 16:09:20  schwarz
# Better src dependency.
# No doc included in dist (wants to make old cocoon doc...)
#
# Revision 1.11  2002/10/30 15:54:01  schwarz
# Added necessary files to dist.
# Added src distribution target.
# Updated publish method (not tested).
#
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

if COND_COMPILE_DOC
DOC_SRC_DIR = doc
else
DOC_SRC_DIR =
endif 

SUBDIRS = sdif include tools data test sdifcpp $(DOC_SRC_DIR)
EXTRA_DIST = README.MacOSX

sdifincdir = @top_srcdir@/include

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = sdif.pc

grep:
	grep $a sdif/*.[ch] /dev/null

doc: 
	cd doc; $(MAKE) -f Makefile.doc

ircam_deps:
	@cd sdif; $(MAKE) ircam_deps

test:	check



#
# DISTRIBUTION
#

# more files to include in dist
EXTRA_DIST	  = sdif.vcproj

# base name of distribution (override automake generated variable to make source distribution!)
distdir		  = $(PACKAGE)-$(VERSION)-src
PUBLISH_FILE      = $(distdir).tar.gz
PUBLISH_RPM	  = $(PACKAGE)-$(VERSION)-1.i386.rpm

# make a binary rpm (experimental)
rpm:		$(rpmdir) sdif.spec dist
		cp $(PUBLISH_FILE) $(rpmdir)/SOURCES
		cp sdif.spec       $(rpmdir)/SPECS
		cd $(rpmdir)/SPECS; rpm -ba sdif.spec
		cp $(rpmdir)/RPMS/i386/$(PUBLISH_RPM) $(top_builddir)

# this must be in your .rpmmacros file: %_topdir /tmp/<your username>-rpm
rpmdir = /tmp/$(USER)-rpm
$(rpmdir):
		cp -rf /usr/src/redhat/ $@


#
# PUBLISH (on server)
#

# use this to check if the distribution works, tag the cvs project, 
# make a source distribution and rpm, and copy it to the web server
publish:	distcheck cvstag dist rpm publish-ftp

# more files to publish (on server)
EXTRA_PUBLISH     = README COPYING AUTHORS INSTALL ChangeLog #  doc

# call this as "make PUBLISH_HOST=yourwebhost publish" to publish a new version
# this might work differently for other places
# todo: configure PUBLISH_HOST
PUBLISH_DIR       = sdif/download
PUBLISH_FILES     = $(PUBLISH_FILE) $(PUBLISH_RPM) $(EXTRA_PUBLISH)

publish-ftp:	# (experimental)
		cat <<END_OF_FTP_COMMANDS \
$$< \
$$< \
cd $(PUBLISH_DIR) \
mput $(PUBLISH_FILES) \
END_OF_FTP_COMMANDS \
		| ftp $(PUBLISH_HOST)



# tag the cvsrepository with the current version 
# (to exactly reconstitute a published version)
CVSTAG	      = $(shell echo $(PACKAGE)_lib_version_$(VERSION) | tr . _)
cvstag:
		cvs tag -F $(CVSTAG)


.PHONY:		grep doc ircam_deps test publish publish-ftp rpm cvstag
