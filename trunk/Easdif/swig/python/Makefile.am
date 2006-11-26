
# PYTHON_SITE_PKG=`python2.2 -c 'import distutils.sysconfig; print distutils.sysconfig.get_python_lib()'`


# PYTHON=python
# PYTHON_INCLUDE=`$(PYTHON) -c 'import distutils.sysconfig; print distutils.sysconfig.get_python_inc()'`

INCL		= -I/usr/local/include -I$(PYTHON_INCLUDE) \
		  -I@SDIF_INC_DIR@ -I@SDIF_DEFINC_DIR@  -I@EASDIF_INC_DIR@ -I@EASDIF_DEFINC_DIR@ -I$(top_srcdir)/../SDIF/include -I$(top_srcdir)/..

EASDIF_LIB_PATH	= @EASDIF_LIB_PATH@
# dbflag = -g
BUILT_SOURCES = eaSDIF_wrap.cpp # $(srcdir)/SDIF_wrap.cpp
SWIG_SOURCES = ../easdif-common.i ../easdif-defines.i sdiftypemap-python.i

pkgpython_PYTHON = 	eaSDIF.py
pkgpyexec_LTLIBRARIES = _eaSDIF.la
_eaSDIF_la_SOURCES = eaSDIF_wrap.cpp $(SWIG_SOURCES)
_eaSDIF_la_CPPFLAGS = $(SWIG_PYTHON_CPPFLAGS) -I@SDIF_INC_DIR@ -I@SDIF_DEFINC_DIR@  -I@EASDIF_INC_DIR@ -I@EASDIF_DEFINC_DIR@ -I$(top_srcdir)/../SDIF/include -I$(top_srcdir)/..
_eaSDIF_la_LDFLAGS = -module
_eaSDIF_la_LIBADD = @EASDIF_LIB_PATH@/libEasdif.la

incfiles	= ../easdif-common.i ../easdif-defines.i sdiftypemap-python.i


easdifincbase	= entity _frame _matrix namevaluetable
easdifincfiles	= $(easdifincbase:%=@EASDIF_DEFINC_DIR@/easdif/sdif%.hpp)
sdifincfiles	= @SDIF_DEFINC_DIR@/sdif.h

eaSDIF_wrap.cpp : easdif-python.i $(SWIG_SOURCES)  $(easdifincfiles) $(incfiles)
	$(SWIG) $(SWIG_PYTHON_OPT)  -I$(top_srcdir)/swig/python -o $@ $<

$(srcdir)/SDIF_wrap.cpp : sdif.i $(SWIG_SOURCES)  $(sdifincfiles) $(incfiles)
	$(SWIG) $(SWIG_PYTHON_OPT)  -I$(top_srcdir)/swig/python -o $@ $<


test:	eaSDIF.py _eaSDIF.la # _SDIF.so
	PYTHONPATH=.libs;export PYTHONPATH; LD_LIBRARY_PATH=@EASDIF_LIB_PATH@:$(LD_LIBRARY_PATH) \
	$(PYTHON) test.py

CLEANFILES=eaSDIF.py SDIF.py eaSDIF.pyc SDIF.pyc eaSDIF_wrap.cpp

# clean:
# 	-rm -f eaSDIF_wrap.* SDIF_wrap.* eaSDIF.py SDIF.py eaSDIF.pyc SDIF.pyc _eaSDIF.so _SDIF.so
