
PYTHON_SITE_PKG=`python2.2 -c 'import distutils.sysconfig; print distutils.sysconfig.get_python_lib()'`


PYTHON=python2.2
PYTHON_INCLUDE=`$(PYTHON) -c 'import distutils.sysconfig; print distutils.sysconfig.get_python_inc()'`

INCL		= -I/usr/local/include -I$(PYTHON_INCLUDE) \
		  -I@SDIF_INC_DIR@ -I$../SDIF/sdif -I@EASDIF_INC_DIR@

EASDIF_LIB_PATH	= @EASDIF_LIB_PATH@
# dbflag = -g


modules		= eaSDIF SDIF
incfiles	= ../easdif-common.i ../easdif-defines.i sdiftypemap-python.i
easdifincbase	= entity frame matrix namevaluetable
easdifincfiles	= $(easdifincbase:%=@EASDIF_INC_DIR@/easdif/sdif%.h)
sdifincfiles	= @SDIF_INC_DIR@/sdif.h

define swig
	swig -c++ -python -shadow -o $@ $<
endef


all:	_eaSDIF.so #_SDIF.so

eaSDIF_wrap.cxx:	easdif-python.i $(easdifincfiles) $(incfiles)
			$(swig)

SDIF_wrap.cxx:		sdif.i   $(sdifincfiles)   $(incfiles)
			$(swig)


_eaSDIF.so: eaSDIF.so
	mv eaSDIF.so _eaSDIF.so

_SDIF.so: SDIF.so
	mv SDIF.so _SDIF.so


%_wrap.o: %_wrap.cxx
	g++ -c $(dbflag) -fpic -Dexplicit= -fno-strict-aliasing $(INCL) $<

%.so: 	%_wrap.o
	g++ -shared $(EASDIF_LIB_PATH)/libEasdif.so $< -o $@

test:	eaSDIF.py _eaSDIF.so _SDIF.so
	LD_LIBRARY_PATH=$(EASDIF_LIB_PATH):$(LD_LIBRARY_PATH) \
	$(PYTHON) test.py

clean:
	-rm -f eaSDIF_wrap.* SDIF_wrap.* eaSDIF.py SDIF.py eaSDIF.pyc SDIF.pyc _eaSDIF.so _SDIF.so
