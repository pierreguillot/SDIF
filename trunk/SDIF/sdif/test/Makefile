# $Id: Makefile,v 1.3 2000-05-04 14:57:29 schwarz Exp $
#
# SDIF/sdif/test/Makefile
#
# $Log: not supported by cvs2svn $
# Revision 1.2  1999/09/28  13:15:11  schwarz
# Test case
#
# Revision 1.1  1999/08/31  10:03:37  schwarz
# Added test code for module SdifSelect which parses an access specification to a
# chosen part of SDIF data.  Can be added to a file name.
#

PRJ_DIR := $(shell pwd)/../..
include $(PRJ_DIR)/Makefile.preamble

LIBS 	  = -l$(LIBNAME)

ifeq ($(ARCH),alpha)
CC	  = gcc
CAFLAGS	  = 
endif

ifeq ($(ARCH),sgi)
LIBS 	 += -lgen
endif

INCFLAGS := -I. -I$(INCLUDE_DIR) -I$(SDIF_SRC_DIR)
CFLAGS	 := -g $(CAFLAGS) $(DFLAGS) $(DOFLAGS)  $(INCFLAGS)
LDFLAGS   = -L$(SDIF_LIB_DIR)


%: 	%.c
	$(CC) $^ $(CFLAGS) $(LDFLAGS) $(LFLAGS) $(OBJS) $(LIBS) -o $@

$(SDIF_OBJ_DIR)/%.o:	$(SDIF_SRC_DIR)/%.c
	@echo "----- making $@"
	@echo "      from   $<"
	$(MAKE) -C $(SDIF_SRC_DIR)


TARGETS	= testcheck testselect


all:	$(TARGETS)
db:
	$(MAKE) all DEBUG=1

testselect:	testselect.c $(SDIF_OBJ_DIR)/SdifSelect.o
testcheck:	testcheck.c  $(SDIF_LIB_DIR)/lib$(LIBNAME).a

$(SDIF_LIB_DIR)/lib$(LIBNAME).a:	$(SDIF_SRC:%=../%)
		$(MAKE) -C $(PRJ_DIR) lib

clean:
	$(RM) $(TARGETS)


# test cases:
#	testselect asdf::aaaa,bbb@2+-.-3

# querysdif ../../test/sdiffiles2000/pianoRes.m5.FIL.sdif
# ./testcheck ../../test/sdiffiles2000/pianoRes.m5.FIL.sdif 1REB
# Found 1REB frame, index 0.
# ./testcheck ../../test/sdiffiles2000/pianoRes.m5.FIL.sdif 1NOI 1REB
# Found 1REB frame, index 1.
# ./testcheck ../../test/sdiffiles2000/pianoRes.m5.FIL.sdif 1HRM 1HRM 1HRM
# Found no frame, index -1.

