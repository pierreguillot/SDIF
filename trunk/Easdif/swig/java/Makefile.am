

TOP_SRCDIR=../..
TOP_BUILDDIR=$(TOP_SRCDIR)

PYTHON_INCLUDE=`$(PYTHON) -c 'import distutils.sysconfig; print distutils.sysconfig.get_python_inc()'`
JAVA_INCLUDE=@JAVA_INCLUDE@

INCL		= -I/usr/local/include $(JAVA_INCLUDE) \
		  -I@SDIF_INC_DIR@ -I@SDIF_DEFINC_DIR@  -I@EASDIF_INC_DIR@ -I@EASDIF_DEFINC_DIR@
EASDIF_LIB_PATH	= @EASDIF_LIB_PATH@
# dbflag = -g


modules		= eaSDIF SDIF
incfiles	= ../easdif-defines.i sdiftypemap-java.i
easdifincbase	= entity frame matrix namevaluetable
easdifincfiles	= $(easdifincbase:%=@EASDIF_DEFINC_DIR@/easdif/sdif%.h)
sdifincfiles	= @SDIF_DEFINC_DIR@/sdif.h

define swig
	swig -c++ -java -o $@ $<
endef


all:	libeaSDIF.so #libSDIF.so

eaSDIF_wrap.cxx:	easdif-java.i $(easdifincfiles) $(incfiles)
			$(swig)

SDIF_wrap.cxx:		sdif.i   $(sdifincfiles)   $(incfiles)
			$(swig)


libeaSDIF.so: eaSDIF.so
	mv eaSDIF.so libeaSDIF.so

libSDIF.so: SDIF.so
	mv SDIF.so libSDIF.so


%_wrap.o: %_wrap.cxx
	g++ -c $(dbflag) $(INCL) $<

%.so: 	%_wrap.o
	g++ -shared $(EASDIF_LIB_PATH)/libEasdif.so $< -o $@

test:	libeaSDIF.so 
	javac *.java
	LD_LIBRARY_PATH=$(EASDIF_LIB_PATH):$(LD_LIBRARY_PATH):. \
	java main

clean:
	-mv main.java main.java.new
	-rm -f eaSDIF_wrap.* SDIF_wrap.* *.java *.class libeaSDIF.so
	-mv main.java.new main.java