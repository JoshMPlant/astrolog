DISTCLEANFILES = swelocal.h
swelocal.h: Makefile
	echo '#define SE_EPHE_PATH "$(datadir)/$(PACKAGE)/ephe"' >$@
swephexp.h: swelocal.h
bin_PROGRAMS = swetest swemini
swetest_SOURCES = swetest.c
swemini_SOURCES = swemini.c
swetest_LDADD = $(builddir)/libswe.la
swemini_LDADD = $(builddir)/libswe.la
lib_LTLIBRARIES = libswe.la
libswe_la_SOURCES= swecl.c swedate.c swehouse.c swejpl.c swemmoon.c swemplan.c swepcalc.c swepdate.c sweph.c swephlib.c swehel.c
libswe_la_HEADERS = swephexp.h swelocal.h
swetest_HEADERS = swephexp.h swelocal.h
swemini_HEADERS = swephexp.h swelocal.h
libswe_ladir = $(libdir)
#include_HEADERS = swephexp.h swelocal.h
swetest_DATA = fixstars.cat sedeltat.txt.inactive seorbel.txt seleapsec.txt seasnam.txt
swetestdir= $(datadir)/$(PACKAGE)
sweminidir= $(datadir)/$(PACKAGE)
EXTRA_DIST = swedate.h swedll.h swehouse.h swejpl.h swenut2000a.h sweodef.h swepcalc.h sweph.h swephexp.h swephlib.h swemplan.c swemptab.c fixstars.cat sedeltat.txt.inactive seorbel.txt seleapsec.txt seasnam.txt

