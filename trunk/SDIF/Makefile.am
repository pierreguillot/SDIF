# $Id: Makefile.am,v 1.7 2001-04-26 14:51:59 tisseran Exp $
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
#
SUBDIRS = sdif include tools data test

EXTRA_DIST = \
	README COPYING AUTHORS INSTALL doc

sdifincdir = @top_srcdir@/include

grep:
	grep $a sdif/*.[ch] /dev/null

doc: 
	cd doc; $(MAKE) -f Makefile.doc

ircam_deps:
	@cd sdif; $(MAKE) ircam_deps

.PHONY	:	test

test	:	check

# tag the cvsrepository with the current version 
# (to exactly reconstitute a published version)
CVSTAG	      = $(shell echo $(PACKAGE)_lib_version_$(VERSION) | tr . _)
cvstag:
		cvs tag -F $(CVSTAG)

# call this as "make DISTHOST=yourwebhost publish" to publish a new version
# we publish by copying to DISTDIR, logged on DISTHOST
# this might work differently for other places
DISTDIR       = /usr/local/www/ircam/equipes/analyse-synthese/sdif/download
DISTFILE      = $(distdir).tar.gz
publish:	cvstag dist
		rsh $(DISTHOST) cp $(PWD)/$(DISTFILE) $(DISTDIR) \;\
				chmod g+w $(DISTDIR)/$(DISTFILE)
