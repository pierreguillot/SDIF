include ../Makefile.preamble

LDFLAGS = -L$(SDIF_LIB_DIR)
LIBS = -l$(LIBNAME)


CFLAGS = $(CAFLAGS) $(DOFLAGS) -D_SDIF_VERS="$(VERSION)" -I. -I$(INCLUDE_DIR) -I$(SDIF_SRC_DIR)


include Makefile.src

ODIRS  = $(SYSTEM)_$(SYSVERS)
OBJS = $(SRC:%.c=$(ODIRS)/%.o)


PROGNAME := sdif
PROGDIRS := ./bin/$(SYSTEM)_$(SYSVERS)
PROG := $(PROGDIRS)/$(PROGNAME)

all: dirs prog

dirs: $(PROGDIRS:%=%-dir)  $(ODIRS:%=%-dir)

%-dir: 
	@if ( ! -e $* ) mkdir -p $*

prog: $(PROG) $(SDIF_LIB)
	cp File.sdif.txt.r $(PROGDIRS)
	cp $(PRJ_DIR)/lib$(LIBNAME)/SdifTypes.STYP $(PROGDIRS)
	@echo $(PROGNAME)

$(PROG): $(OBJS)
	$(CC) $(LDFLAGS) $(LFLAGS) $(OBJS) $(LIBS) -o $(PROG)

$(ODIRS)/%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $< 

clean:
	rm -f $(PROG)
	rm -f $(OBJS)
	rm -f *~
	rm -f #*
	rm -f $(PROGDIRS)/core

test:
	cd $(PROGDIRS); $(PROGNAME)
