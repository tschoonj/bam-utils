noinst_LTLIBRARIES = libbam_file_asc.la
libbam_file_asc_la_SOURCES = bam_file_asc.cpp \
                             bam_file_asc.h \
                             bam_plplot_window.h \
                             bam_plplot_window.cpp
libbam_file_asc_la_CXXFLAGS = $(plplotcxx_CFLAGS) $(gtkmm_CFLAGS) $(gsl_CFLAGS)
libbam_file_asc_la_LIBADD =  $(plplotcxx_LIBS) $(gtkmm_LIBS) $(gsl_LIBS)


#noinst_PROGRAMS = test-read-asc
#test_read_asc_SOURCES = test-read-asc.cpp
#test_read_asc_LDADD = libbam_file_asc.la

bin_PROGRAMS = asc2athena
asc2athena_SOURCES = asc2athena.h asc2athena.cpp main.cpp
asc2athena_LDADD = libbam_file_asc.la
asc2athena_CXXFLAGS = libbam_file_asc.la $(plplotcxx_CFLAGS) $(gtkmm_CFLAGS)
