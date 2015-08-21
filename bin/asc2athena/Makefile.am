noinst_LTLIBRARIES = libbam_file_asc.la
libbam_file_asc_la_SOURCES = bam_file_asc.cpp \
                             bam_file_asc.h \
                             bam_math.h \
                             bam_math.cpp \
                             bam_exception.h
libbam_file_asc_la_CXXFLAGS = $(gtkmm_CFLAGS) $(xraylib_CFLAGS)  -I$(top_srcdir)/bin/common
libbam_file_asc_la_LIBADD =  $(gtkmm_LIBS) $(xraylib_LIBS) ../common/libcommon.la


noinst_PROGRAMS = test-read-asc test-deriv
test_read_asc_SOURCES = test-read-asc.cpp
test_read_asc_CXXFLAGS = $(gtkmm_CFLAGS)   -I$(top_srcdir)/bin/common
test_read_asc_LDADD = libbam_file_asc.la $(gtkmm_LIBS)
test_deriv_SOURCES = test-deriv.cpp
test_deriv_LDADD = libbam_file_asc.la

bin_PROGRAMS = asc2athena
asc2athena_SOURCES = asc2athena.h asc2athena.cpp main.cpp
asc2athena_LDADD = libbam_file_asc.la
asc2athena_CXXFLAGS = $(gtkmm_CFLAGS) $(xraylib_CFLAGS)  -I$(top_srcdir)/bin/common

if OS_WINDOWS
asc2athena_LDFLAGS = -Wl,-subsystem,windows
endif
