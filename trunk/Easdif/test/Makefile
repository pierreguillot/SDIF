FORMESROOT = /u/formes/share

CXX = g++
CXXFLAGS = -g -ggdb -Wall -I../src -I$(FORMESROOT)/include
LDFLAGS = -L../lib -L$(FORMESROOT)/lib/$(SYS) -lEasdif -lsdif -lm


all: virtual classba classbis classbiw classbiww esclass

matrix: matrix.cpp ../lib/libEasdif.a
	$(CXX) matrix.cpp -o $@ $(CXXFLAGS) $(LDFLAGS)

virtual: virtual.cpp ../lib/libEasdif.a
	$(CXX) virtual.cpp -o $@ $(CXXFLAGS) $(LDFLAGS)

classba: classba.cpp ../lib/libEasdif.a
	$(CXX) classba.cpp -o $@ $(CXXFLAGS) $(LDFLAGS)


classbis: classbis.cpp ../lib/libEasdif.a
	$(CXX) classbis.cpp -o $@ $(CXXFLAGS) $(LDFLAGS)


classbiw: classbiw.cpp ../lib/libEasdif.a
	$(CXX) classbiw.cpp -o $@ $(CXXFLAGS) $(LDFLAGS)

classbiww: classbiww.cpp ../lib/libEasdif.a 
	$(CXX) classbiww.cpp -o $@ $(CXXFLAGS) $(LDFLAGS)


esclass: esclass.cpp ../lib/libEasdif.a 
	$(CXX) esclass.cpp -o $@ $(CXXFLAGS) $(LDFLAGS)

clean:
	-rm matrix virtual classba classbis classbiw classbiww cla esclass $(LIB_OBJ) core

