bin_PROGRAMS = chi2dat 

noinst_PROGRAMS = readchi

readchi_SOURCES = readchi.cpp \
		  chifile.cpp \
		  chifile.h

chi2dat_SOURCES = main.cpp \
		  chi2dat.cpp \
		  chi2dat.h \
		  chifile.cpp \
		  chifile.h
		  
chi2dat_CPPFLAGS = @gtkmm_CFLAGS@ \
		   @gsl_CFLAGS@
chi2dat_LDADD = @gtkmm_LIBS@ \
		@gsl_LIBS@

if OS_WINDOWS
chi2dat_LDFLAGS = -Wl,-subsystem,windows
endif

