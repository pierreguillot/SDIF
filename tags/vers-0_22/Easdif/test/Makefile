SDIFROOT := ../SDIF

CXX = g++
CXXFLAGS = -g -ggdb -Wall -I.. -I$(SDIFROOT)/include -I$(SDIFROOT)/compile.$(SYS)/include 

LIBDIR  = ../lib.$(SYS)
LDFLAGS = -L$(LIBDIR) -lEasdif   -lm



all: tutorini tutofini #trytry #try_excep tutorial_fin #rent_sel ent_sel#typty #ent_sel #try_excep ent_sele ent_sel entttselentread try_excep #typty ttyp tetyp testyp testnvt tutname tutmat virtual classba classbis classbiw classbiww esclass new_tutorial try_excep new_try tryfile 

ent_sele: ent_sele.cpp $(LIBDIR)/libEasdif.a 
	$(CXX) ent_sele.cpp  -o $@ $(CXXFLAGS) $(LDFLAGS)

entttsel: entttsel.cpp $(LIBDIR)/libEasdif.a 
	$(CXX) entttsel.cpp  -o $@ $(CXXFLAGS) $(LDFLAGS)

ent_sel: ent_sel.cpp $(LIBDIR)/libEasdif.a 
	$(CXX) ent_sel.cpp  -o $@ $(CXXFLAGS) $(LDFLAGS)

try_excep: try_excep.cpp $(LIBDIR)/libEasdif.a 
	$(CXX) try_excep.cpp  -o $@ $(CXXFLAGS) $(LDFLAGS)

entread: entread.cpp $(LIBDIR)/libEasdif.a 
	$(CXX) entread.cpp  -o $@ $(CXXFLAGS) $(LDFLAGS)

typty: typty.cpp $(LIBDIR)/libEasdif.a 
	$(CXX) typty.cpp  -o $@ $(CXXFLAGS) $(LDFLAGS)

rent_sel: rent_sel.cpp $(LIBDIR)/libEasdif.a 
	$(CXX) rent_sel.cpp  -o $@ $(CXXFLAGS) $(LDFLAGS)

tutorial_fin: tutorial_fin.cpp $(LIBDIR)/libEasdif.a 
	$(CXX) tutorial_fin.cpp  -o $@ $(CXXFLAGS) $(LDFLAGS)


trytry: trytry.cpp $(LIBDIR)/libEasdif.a 
	$(CXX) trytry.cpp  -o $@ $(CXXFLAGS) $(LDFLAGS)


tutofini: tutofini.cpp $(LIBDIR)/libEasdif.a 
	$(CXX) tutofini.cpp  -o $@ $(CXXFLAGS) $(LDFLAGS)


tutorini: tutorini.cpp $(LIBDIR)/libEasdif.a 
	$(CXX) tutorini.cpp  -o $@ $(CXXFLAGS) $(LDFLAGS)

clean:
	-rm -f tutorini tutofini trytry try_excep tutorial_fin rent_sel typty entread try_excep ent_sel entttsel ent_sele #matrix virtual classba classbis classbiw classbiww cla esclass new_tutorial try_excep new_try tryfile tutmat tutname testnvt testyp ttyp#$(LIB_OBJ) core










