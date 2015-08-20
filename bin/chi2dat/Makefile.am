bin_PROGRAMS = chi2dat

noinst_PROGRAMS = readchi

readchi_SOURCES = readchi.cpp
readchi_LDADD = ../common/libcommon.la
readchi_CPPFLAGS = -I$(top_srcdir)/bin/common

chi2dat_SOURCES = \
									main.cpp \
		  						chi2dat.cpp \
		  						chi2dat.h

chi2dat_CPPFLAGS = \
									 @gtkmm_CFLAGS@ \
		   				 		 @gsl_CFLAGS@ \
									 -I$(top_srcdir)/bin/common
chi2dat_LDADD = \
								@gtkmm_LIBS@ \
								@gsl_LIBS@ \
								../common/libcommon.la



if OS_WINDOWS
chi2dat_LDFLAGS = -Wl,-subsystem,windows
endif
