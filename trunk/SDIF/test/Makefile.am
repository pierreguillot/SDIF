# $Id: Makefile.am,v 1.9 2001-07-12 14:11:48 roebel Exp $
#
# ./test/Makefile.am		September 2000			Patrice Tisserand
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
#
# $Log: not supported by cvs2svn $
# Revision 1.8  2001/05/02 09:34:52  tisseran
# Change License from GNU Public License to GNU Lesser Public License.
#
# Revision 1.7  2000/12/07 13:42:21  roebel
# small reorganization of sdif/Makefile.am
#
# Revision 1.6  2000/12/07 12:59:58  roebel
# Reorganized test suite to be sensitive to wrong endianness
#
# Revision 1.5  2000/10/31 11:46:28  roebel
# Added makefile dependency of all progs from generated library
#
# Revision 1.4  2000/10/30  19:05:49  roebel
# Added support for relative path compilation (VPATH)
# Added support for debugging feature in configure.in and Makefiles
#
# Revision 1.3  2000/10/30  16:30:00  roebel
# Added INCLUDES variable:
#    INCLUDES = -I../include
# to find user level sdif.h
#
# Revision 1.2  2000/10/27  20:04:01  roebel
# autoconf merged back to main trunk
#
# Revision 1.1.2.2  2000/10/26  17:27:40  schwarz
# Added header with copyright and links to all Makefile.am and configure.in.
#
SUBDIRS = files
EXTRA_DIST = test_conv2  cmd_testcheck xpianoRes.m5.FIL.sdif

noinst_PROGRAMS = headerTest NVTTest testcheck testselect
noinst_HEADERS = utils.h
headerTest_SOURCES = test1.c utils.c
headerTest_DEPENDENCIES = ../sdif/@SELLIB@
NVTTest_SOURCES = test2.c utils.c
NVTTest_DEPENDENCIES = ../sdif/@SELLIB@
testcheck_SOURCES = testcheck.c
testcheck_DEPENDENCIES = ../sdif/@SELLIB@
testselect_SOURCES = testselect.c
testselect_DEPENDENCIES = ../sdif/@SELLIB@

LDADD = -l@TESTLIB@
LDFLAGS = -L../sdif
INCLUDES = -I../include -I$(top_srcdir)/include

TESTS_ENVIRONMENT = SDIFTYPES=$(top_srcdir)/data/SdifTypes.STYP
TESTS = test_conv2  headerTest NVTTest cmd_testcheck

