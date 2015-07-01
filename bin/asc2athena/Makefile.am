noinst_LTLIBRARIES = libbam_file_asc.la
libbam_file_asc_la_SOURCES = bam_file_asc.cpp \
                             bam_file_asc.h \
                             bam_plplot_window.h \
                             bam_plplot_window.cpp \
                             bam_math.h \
                             bam_math.cpp \
                             bam_exception.h
libbam_file_asc_la_CXXFLAGS = $(plplotcxx_CFLAGS) $(gtkmm_CFLAGS) $(gsl_CFLAGS)
libbam_file_asc_la_LIBADD =  $(plplotcxx_LIBS) $(gtkmm_LIBS) $(gsl_LIBS)


noinst_PROGRAMS = test-read-asc test-deriv
test_read_asc_SOURCES = test-read-asc.cpp
test_read_asc_CXXFLAGS = $(gtkmm_CFLAGS) $(plplotcxx_CFLAGS)
test_read_asc_LDADD = libbam_file_asc.la $(gtkmm_LIBS)
test_deriv_SOURCES = test-deriv.cpp
test_deriv_LDADD = libbam_file_asc.la

bin_PROGRAMS = asc2athena
asc2athena_SOURCES = asc2athena.h asc2athena.cpp main.cpp
asc2athena_LDADD = libbam_file_asc.la $(xraylib_LIBS)
asc2athena_CXXFLAGS = libbam_file_asc.la $(plplotcxx_CFLAGS) $(gtkmm_CFLAGS) $(xraylib_CFLAGS)
