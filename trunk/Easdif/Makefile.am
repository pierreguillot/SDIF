# $Id: Makefile.am,v 1.2 2003-03-04 00:50:01 roebel Exp $
#
# ./Makefile.am		Feb. 2003     Axel Roebel
#
# Source file for GNU automake, generates Makefile.in.
#


# $Log: not supported by cvs2svn $
# Revision 1.1  2003/03/03 10:03:24  roebel
# Adapted for autoconf/automake...
#

SUBDIRS = SDIF easdif  test
DIST_SUBDIRS = easdif  test

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = Easdif.pc

grep:
	grep $a easdif/*.cpp easdif/*.h /dev/null

#
# DISTRIBUTION
#

# base name of distribution (override automake generated variable to make source distribution!)
distdir		  = $(PACKAGE)-$(VERSION)-src
PUBLISH_FILE      = $(distdir).tar.gz

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


.PHONY:		grep doc ircam_deps test publish publish-ftp  cvstag
