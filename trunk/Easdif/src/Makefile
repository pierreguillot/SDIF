FORMESROOT = /u/formes/share
SDIFROOT = ../SDIF
CXX = g++
CXXFLAGS = -Wall -I.. -I$(SDIFROOT)/include -I$(SDIFROOT)/sdif

ifeq ($(DEBUG), 1)
	CXXFLAGS += -g -ggdb
endif

LIB_SRC = sdifframe.cpp sdifmatrix.cpp sdifentity.cpp sdifnamevaluetable.cpp sdifinit.cpp sdifexception.cpp
OBJ_DIR = ../obj.$(SYS)
LIB_DIR = ../lib.$(SYS)
LIB_OBJ = $(LIB_SRC:%.cpp=$(OBJ_DIR)/%.o)
LIBNAME = libEasdif.a
TARGETLIB = $(LIB_DIR)/$(LIBNAME)

.PHONY: DIRS

all: DIRS $(LIBNAME)

DIRS:
	@mkdir -p $(OBJ_DIR)  $(LIB_DIR) 


$(OBJ_DIR)/%.o: %.cpp
	$(CXX) $< -c -o $@ $(CXXFLAGS)

$(LIBNAME): $(LIB_OBJ)
	ar cru $(TARGETLIB) $(LIB_OBJ)
	cd $(OBJ_DIR);list=`ar -xv $(SDIFROOT)/sdif/libsdif.a|cut -d" " -f3`;\
	ar  -r $(TARGETLIB) $$list; rm -f $$list

clean:
	-rm -f $(TARGETLIB)  $(LIB_OBJ) core