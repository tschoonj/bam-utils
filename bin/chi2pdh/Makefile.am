bin_PROGRAMS = chi2pdh

chi2pdh_SOURCES = \
									main.cpp \
		  						chi2pdh.cpp \
		  						chi2pdh.h

chi2pdh_CPPFLAGS = \
									 @gtkmm_CFLAGS@ \
		   				 		 @gsl_CFLAGS@ \
									 -I$(top_srcdir)/bin/common
chi2pdh_LDADD = \
								@gtkmm_LIBS@ \
								@gsl_LIBS@ \
								../common/libcommon.la



if OS_WINDOWS
chi2pdh_LDFLAGS = -Wl,-subsystem,windows
endif
