SUBDIRS = swe
DISTCLEANFILES = local.h
local.h: Makefile
	echo '#define EPHE_DIR "$(datadir)/swisseph"' >$@
	echo '#define EXTEPH_DIR "$(datadir)/$(PACKAGE)/exteph"' >>$@
	echo '#define DEFAULT_DIR "$(datadir)/$(PACKAGE)/"' >>$@
astrolog.c: local.h
bin_PROGRAMS = astrolog aastrolog 
aastrolog_SOURCES = astrolog.c  charts1.c  charts4.c  general.c   io.c  xcharts1.c  xdevice.c  calc.c      charts2.c  data.c     intrpalt.c  matrix.c    xcharts2.c  xgeneral.c riyal.c riyal2.c gtopo.c charts0.c   charts3.c  data2.c    intrpret.c  swe_call.c  xcharts0.c  xdata.c     xscreen.c  xcharts3.c
astrolog_SOURCES = astrolog.c  charts1.c  charts4.c  general.c   io.c  xcharts1.c  xdevice.c  calc.c      charts2.c  data.c     intrpalt.c  matrix.c    xcharts2.c  xgeneral.c riyal.c gtopo.c  charts0.c   charts3.c  data2.c    intrpret.c  swe_call.c  xcharts0.c  xdata.c     xscreen.c xcharts3.c
astrolog_LDADD = $(builddir)/swe/src/libswe.la
aastrolog_LDADD = $(builddir)/swe/src/libswe.la
#noinst_LIBRARIES = libswe.a
#libswe_a_SOURCES= swecl.c swedate.c swehouse.c swejpl.c swemmoon.c swemplan.c swepcalc.c swepdate.c sweph.c swephlib.c 
astrolog_HEADERS = local.h 
#astrolog_HEADERS = astrolog.h config.h extern.h resource.h swedate.h swedll.h swehouse.h swejpl.h swenut2000a.h sweodef.h swepcalc.h sweph.h swephexp.h swephlib.h tr.h local.h swemplan.c swemptab.c
aastrologdir = $(datadir)/${PACKAGE}
astrologdir = $(datadir)/${PACKAGE}
#kastrolog_SOURCES = astrolog.c  charts1.c  charts4.c  general.c   io.c  xcharts1.c  xdevice.c  AspectSet.cpp AstroCommand.cpp AstroInfo.cpp AstroObjectSet.cpp AstroProgress.cpp AstroSetAspects.cpp AstroTransits.cpp CalcSettings.cpp ChartDetails.cpp ColorVector.cpp GraphSettings.cpp ObjectSet.cpp ObscureSettings.cpp Restrictions.cpp SetAstroColor.cpp SetColors.cpp StarRestrict.cpp AspectSetData.cpp AstroCommandData.cpp AstroInfoData.cpp AstroObjectSetData.cpp AstroProgressData.cpp AstroSetAspectsData.cpp AstroTransitsData.cpp CalcSettingsData.cpp ChartDetailsData.cpp ColorVectorData.cpp GraphSettingsData.cpp ObjectSetData.cpp ObscureSettingsData.cpp RestrictionsData.cpp SetAstroColorData.cpp SetColorsData.cpp StarRestrictData.cpp AstroWidget.cpp myLineEdit.cpp main.cpp AstroMenu.cpp AstroColor.cpp calc.c      charts2.c  data.c     intrpalt.c  matrix.c    xcharts2.c  xgeneral.c riyal.c gtopo.c charts0.c   charts3.c  data2.c    intrpret.c  swe_call.c  xcharts0.c  xdata.c  xscreen.c xcharts3.c
if X_DISPLAY_MISSING
astrolog_LDFLAGS = -Wall -O  
aastrolog_LDFLAGS = -Wall -O  
else
astrolog_LDFLAGS =  $(X_LIBS)
aastrolog_LDFLAGS = $(X_LIBS) -D ASTEROIDS
#kastrolog_LDFLAGS = -U X11 -D KDE  
endif

if X_DISPLAY_MISSING
astrolog_CFLAGS = -Wall -O -I$(srcdir)/swe/src 
aastrolog_CFLAGS = -Wall -O -I$(srcdir)/swe/src -D ASTEROIDS
aastrolog_CPPFLAGS = -I$(srcdir)/swe/src -D ASTEROIDS
else
astrolog_CFLAGS = -Wall -O $(X_CFLAGS)
aastrolog_CFLAGS = -Wall -O $(X_CFLAGS)
aastrolog_CPPFLAGS = -I$(srcdir)/swe/src -D ASTEROIDS 
astrolog_CPPFLAGS = -I$(srcdir)/swe/src  
#kastrolog_CFLAGS = -U X11 -D KDE 
endif

EXTRA_DIST = AspectSet.cpp AspectSet.h AspectSetData.cpp AspectSetData.h AstroColor.cpp AstroColor.h AstroCommand.cpp AstroCommand.h AstroCommandData.cpp AstroCommandData.h AstroInfo.cpp AstroInfo.h AstroInfoData.cpp AstroInfoData.h AstroMenu.cpp AstroMenu.h AstroObjectSet.cpp AstroObjectSet.h AstroObjectSetData.cpp AstroObjectSetData.h AstroProgress.cpp AstroProgress.h AstroProgressData.cpp AstroProgressData.h AstroSetAspects.cpp AstroSetAspects.h AstroSetAspectsData.cpp AstroSetAspectsData.h AstroTransits.cpp AstroTransits.h AstroTransitsData.cpp AstroTransitsData.h AstroWidget.cpp AstroWidget.h CalcSettings.cpp CalcSettings.h CalcSettingsData.cpp CalcSettingsData.h ChartDetails.cpp ChartDetails.h ChartDetailsData.cpp ChartDetailsData.h ColorVector.cpp ColorVector.h ColorVectorData.cpp ColorVectorData.h GraphSettings.cpp GraphSettings.h GraphSettingsData.cpp GraphSettingsData.h ObjectSet.cpp ObjectSet.h ObjectSetData.cpp ObjectSetData.h ObscureSettings.cpp ObscureSettings.h ObscureSettingsData.cpp ObscureSettingsData.h Restrictions.cpp Restrictions.h RestrictionsData.cpp RestrictionsData.h SetAstroColor.cpp SetAstroColor.h SetAstroColorData.cpp SetAstroColorData.h SetColors.cpp SetColors.h SetColorsData.cpp SetColorsData.h StarRestrict.cpp StarRestrict.h StarRestrictData.cpp StarRestrictData.h config.h main.cpp myLineEdit.cpp myLineEdit.h astrolog.h config.h extern.h resource.h tr.h 

