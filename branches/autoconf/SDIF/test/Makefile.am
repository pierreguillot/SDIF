SUBDIRS = files
EXTRA_DIST = test_conv test_cmpsdif cmd_testcheck xpianoRes.m5.FIL.sdif

noinst_PROGRAMS = headerTest NVTTest testcheck testselect
noinst_HEADERS = utils.h
headerTest_SOURCES = test1.c utils.c
NVTTest_SOURCES = test2.c utils.c
testcheck_SOURCES = testcheck.c
testselect_SOURCES = testselect.c

LDADD = -lsdif
LDFLAGS = -L../sdif

TESTS_ENVIRONMENT = SDIFTYPES=../data/SdifTypes.STYP
TESTS = test_conv test_cmpsdif headerTest NVTTest cmd_testcheck

