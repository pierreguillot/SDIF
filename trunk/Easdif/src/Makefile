FORMESROOT = /u/formes/share
SDIFROOT = ../SDIF
CXX = g++
CXXFLAGS = -Wall -I.. -I$(SDIFROOT)/include -I$(SDIFROOT)/sdif -I$(SDIFROOT)/compile.$(SYS)/include 

ifeq ($(DEBUG), 1)
	CXXFLAGS += -g -ggdb
endif

HEADER = sdifexception.h easdif.h
LIB_SRC = sdifframe.cpp sdifmatrix.cpp sdifentity.cpp sdifnamevaluetable.cpp sdifinit.cpp sdifexception.cpp
OBJ_DIR = ../obj.$(SYS)
LIB_DIR = ../lib.$(SYS)
LIB_OBJ = $(LIB_SRC:%.cpp=$(OBJ_DIR)/%.o)
LIBNAME = libEasdif.a
TARGETLIB = $(LIB_DIR)/$(LIBNAME)

.PHONY: DIRS easdif_version

all: DIRS $(LIBNAME)

DIRS:
	@mkdir -p $(OBJ_DIR)  $(LIB_DIR) 

easdif_version:
	echo "/* Version automatically generated from Makefile/DoxyFile" >$@.h
	echo " * do not edit */" >>$@.h
	echo "#ifndef EASDIF_VERSION_MAJ"	 >>$@.h
	echo "#define" EASDIF_VERSION_MAJ $(VERSIONMAJ) >>$@.h
	echo "#define" EASDIF_VERSION_MIN $(VERSIONMIN) >>$@.h
	echo "#endif "	 >>$@.h


$(OBJ_DIR)/%.o: %.cpp $(HEADER)
	$(CXX) $< -c -o $@ $(CXXFLAGS)

$(LIBNAME): $(LIB_OBJ)
	ar cru $(TARGETLIB) $(LIB_OBJ)
	cd $(OBJ_DIR);list=`ar -xv $(SDIFROOT)/compile.$(SYS)/sdif/libsdif-cpp.a|cut -d" " -f3`;\
	ar  -r $(TARGETLIB) $$list; rm -f $$list

clean:
	-rm -f $(TARGETLIB)  $(LIB_OBJ) core




