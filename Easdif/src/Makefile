FORMESROOT = /u/formes/share
FORMROOT = /u/formes/ftissera/src/Easdif/SDIF

CXX = g++
#CXXFLAGS = -Wall -I. -I$(FORMESROOT)/include

#CXXFLAGS = -Wall -I./sdif -I$(FORMROOT)/include
#CXXFLAGS = -Wall -I. -I$(FORMROOT)/include
CXXFLAGS = -Wall -I. -I$(FORMROOT)/include -I$(FORMROOT)/sdif

ifeq ($(DEBUG), 1)
	CXXFLAGS += -g -ggdb
endif

LIB_SRC = sdifframe.cpp sdifmatrix.cpp sdifentity.cpp sdifnamevaluetable.cpp#sdifstream.cpp sdifdocument.cpp
OBJ_DIR = .
LIB_OBJ = $(LIB_SRC:%.cpp=$(OBJ_DIR)/%.o)
LIBNAME = libEasdif.a

all: $(LIBNAME)

$(OBJ_DIR)/%.o: %.cpp
	$(CXX) $< -c -o $@ $(CXXFLAGS)

$(LIBNAME): $(LIB_OBJ)
	ar cru $(LIBNAME) $(LIB_OBJ)
	-cp -f $(LIBNAME) ../lib

clean:
	-rm ../lib/$(LIBNAME) $(LIBNAME) $(LIB_OBJ) core