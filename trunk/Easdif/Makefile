
DOCDIR = /usr/local/www/iii/analyse_synthese/documentation/easdif
DOCSERVER = maelzel


.PHONY: lib test all doc clean install LINK

all: lib test



LINK:
	if [ ! -L easdif ];then ln -s src easdif;fi

lib: LINK SDIF/sdif/libsdif.a
	make -C src


SDIF/sdif/libsdif.a: 
	cd SDIF; if [ ! -f ./configure ]; then autoconf;fi ; ./configure --enable-cpp; make 

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
	make -C SDIF distclean
	make -C src clean
	make -C test clean

doc:
	doxygen

install_doc: doc
	chmod -R  g+w doc/html
	rsync -avpz --delete doc/html/* $(DOCSERVER):$(DOCDIR)
	chmod -R  g+w doc/html
