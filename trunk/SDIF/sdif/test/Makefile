# $Id: Makefile,v 1.1 1999-08-31 10:03:37 schwarz Exp $
#
# SDIF/sdif/test/Makefile
#
# $Log: not supported by cvs2svn $

PRJ_DIR := $(shell pwd)/../..
include $(PRJ_DIR)/Makefile.preamble

ifeq ($(ARCH),alpha)
CC	  = gcc
CAFLAGS	  = 
LIBS 	  = -l$(LIBNAME)
else
LIBS 	  = -l$(LIBNAME) -lgen
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


TARGETS	= testselect


all:	$(TARGETS)


testselect:	testselect.c $(SDIF_OBJ_DIR)/SdifSelect.o


clean:
	$(RM) $(TARGETS)
