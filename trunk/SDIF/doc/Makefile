# $Id: Makefile,v 2.0 1998-11-29 11:20:51 virolle Exp $
#
# makefile	4. November 1998	Diemo Schwarz
#
# Makefile for automatic documentation.
#
# $Log: not supported by cvs2svn $
# Revision 1.1  1998/11/10  14:56:40  schwarz
# Added makefile to generate cocoon documentation.
#


#
# cocoon
#

outdir		= html

cocoon:		$(outdir) # cpheaders
		cocoon -x sdif.cocoon

# put header files in separate dirs to yield separate libs

# headerbase	= 
# hdir		= headers

# HEADERS	= $(headerbase:%=$(PROJ_ROOT)/include/%.h)
# HEADIRS		= $(headerbase:%=$(hdir)/%)

cpheaders:	$(hdir) $(HEADIRS)
		for h in $(headerbase); do \
			cp $(PROJ_ROOT)/include/$$h.h $(hdir)/$$h; \
		done

$(outdir) $(hdir) $(HEADIRS):	
		mkdir $@

