SUBDIRS = sdif include tools data doc test

EXTRA_DIST = \
	README COPYING AUTHORS INSTALL

sdifincdir = @top_srcdir@/include

grep:
	grep $a sdif/*.[ch] /dev/null

ircam_deps:
	@cd sdif; $(MAKE) ircam_deps

.PHONY	:	test

test	:	check