
DOCDIR = /usr/local/www/iii/analyse_synthese/documentation/easdif
DOCSERVER = maelzel


.PHONY: lib test all doc clean install LINK

all: lib test

VERSMAJOR=$(strip $(shell grep ^PROJECT_NUMBER  Doxyfile | cut -d"=" -f2| cut -d. -f1))
VERSMINOR=$(strip $(shell grep ^PROJECT_NUMBER  Doxyfile | cut -d"=" -f2| cut -d. -f2))

LINK:
	if [ ! -L easdif ];then ln -s src easdif;fi

lib: LINK SDIF/compile.$(SYS)/sdif/.libs/libsdif-cpp.a src/easdif_version.h
	make -C src


vers:
	@echo Easdif version is:  $(VERSMAJOR).$(VERSMINOR)

src/easdif_version.h: Doxyfile
	make -C src VERSIONMAJ=$(VERSMAJOR) VERSIONMIN=$(VERSMINOR) easdif_version

SDIF/compile.$(SYS)/sdif/.libs/libsdif-cpp.a: 
	cd SDIF; if [ ! -f ./configure ]; then autoconf;fi ; mkdir -p compile.$(SYS); cd compile.$(SYS);../configure --enable-cpp `if [ -d /u/formes/share/include ]; then echo --prefix=/u/formes/share;fi`; make 

test:
	make -C test


install: install_lib 


install_lib: lib
	@mkdir -p $(FORMESROOT)/include/easdif
	rm -f $(FORMESROOT)/include/easdif/*.h
	cp src/*.h  $(FORMESROOT)/include/easdif
	chmod -R g+w $(FORMESROOT)/include/easdif
	rm -f $(FORMESROOT)/lib/$(SYS)/libEasdif.a
	cp lib.$(SYS)/libEasdif.a $(FORMESROOT)/lib/$(SYS)/libEasdif.a
	chmod g+w $(FORMESROOT)/lib/$(SYS)/libEasdif.a

clean:
	rm -rf SDIF/compile.$(SYS)
	make -C src clean
	make -C test clean

doc:
	doxygen

install_doc: doc
	chmod -R  g+w doc/html
	rsync -avpz --delete doc/html/* $(DOCSERVER):$(DOCDIR)
	chmod -R  g+w doc/html

cvstags: lib
	cvs tag -F vers-$(VERSMAJOR)_$(VERSMINOR)
