FORMESROOT = /u/formes/share

CXX = g++
CXXFLAGS = -Wall -I. -I$(FORMESROOT)/include

ifeq ($(DEBUG), 1)
	CXXFLAGS += -g -ggdb
endif

LIB_SRC = sdifframe.cpp sdifmatrix.cpp
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