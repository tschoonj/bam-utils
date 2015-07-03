AC_INIT([bam-utils],[0.3],[tom.schoonjans@bam.de],,[https://github.com/tschoonj/bam-utils])
AC_PREREQ([2.60])
#AC_CONFIG_SRCDIR([src/main/xrmc_main.cpp])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CANONICAL_HOST



LIB_CURRENT=0
LIB_REVISION=0
LIB_AGE=0
AC_SUBST(LIB_CURRENT)
AC_SUBST(LIB_REVISION)
AC_SUBST(LIB_AGE)

#m4_pattern_allow([AS_TR_SH])
AC_CONFIG_MACRO_DIR([m4])

AC_USE_SYSTEM_EXTENSIONS
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])

#libtool
LT_PREREQ([2.0.0])

LT_INIT()
AC_PROG_LIBTOOL

AC_PROG_CC
if test `AS_BASENAME([$CC])` = $CC ; then
  AC_CHECK_PROG(CC_FULL, [$CC], $CC, [none])
  #this next line may never be reached...
  if test x$CC_FULL = "xnone" ; then
          AC_MSG_ERROR([no C compiler was found on the system.])
  fi
fi
AM_PROG_CC_C_O


AC_PROG_CXX
if test `AS_BASENAME([$CXX])` = $CXX ; then
  AC_CHECK_PROG(CXX_FULL, [$CXX], $CXX, [none])
  #this next line may never be reached...
  if test x$CXX_FULL = "xnone" ; then
          AC_MSG_ERROR([no C++ compiler was found on the system.])
  fi
fi
AC_PROG_CXX_C_O


LDFLAGS_EXTRA=""
OS_WINDOWS=0
OS_WINDOWS_32=0
OS_WINDOWS_64=0

case "$host" in
	i686-*mingw*)
		OS_WINDOWS_32=1
		OS_WINDOWS=1
		;;
	x86_64-*mingw*)
		OS_WINDOWS_64=1
		OS_WINDOWS=1
		;;
esac

AC_SUBST(WINDRES_ARCH)

AC_SUBST(OS_WINDOWS)
AM_CONDITIONAL([OS_WINDOWS],[test x$OS_WINDOWS = x1])
AC_SUBST(OS_WINDOWS_32)
AM_CONDITIONAL([OS_WINDOWS_32],[test x$OS_WINDOWS_32 = x1])
AC_SUBST(OS_WINDOWS_64)
AM_CONDITIONAL([OS_WINDOWS_64],[test x$OS_WINDOWS_64 = x1])

#look for openmp support in C++
#AC_LANG_PUSH(C++)
#AC_OPENMP
#AC_LANG_POP(C++)

#AC_CHECK_LIB([m], [exp], [], [AC_MSG_ERROR([math library does not contain exp])])
#AC_CHECK_FUNC(expm1,,[AC_MSG_ERROR([Required libc function not found])])
#AC_CHECK_FUNC(log1p,,[AC_MSG_ERROR([Required libc function not found])])


#look for xraylib
#initialize pkg-config
PKG_PROG_PKG_CONFIG


#search for xraylib and other modules
PKG_CHECK_MODULES([xraylib],libxrl >= 3.1.0)
#PKG_CHECK_MODULES([xmimsim],libxmimsim >= 5.1)
PKG_CHECK_MODULES([gtkmm],gtkmm-3.0 >= 3.12.0)
PKG_CHECK_MODULES([gsl], gsl)
PKG_CHECK_MODULES([plplotcxx], [plplot-c++], , 
 [PKG_CHECK_MODULES([plplotcxx], [plplotd-c++])])
#PKG_CHECK_MODULES([libxmlmm], libxml++-2.6 >= 2.30.0)

#search for xmlcatalog
#AC_CHECK_PROG([XMLCATALOG],[xmlcatalog],[xmlcatalog],[fail])
#if test x$XMLCATALOG = xfail ; then
#	AC_MSG_ERROR([xmlcatalog was not found. This should be a part of the libxml2 installation which was detected on the system. Consider verifying your PATH variable.])
#fi


AC_CONFIG_FILES([	Makefile
			bin/Makefile
			bin/chi2dat/Makefile
			bin/asc2athena/Makefile
			windows/Makefile
			])
AC_CONFIG_HEADERS([config.h])

AM_CONDITIONAL([ABS_SRC_BUILD_EQUAL],[test x$abs_top_builddir = x$abs_top_srcdir])

if test x$OS_WINDOWS = "x1" ; then
	abs_top_builddir_win=`pwd -W`
	AC_SUBST(abs_top_builddir_win)
	abs_top_srcdir_win=`AS_DIRNAME([$0])`
	cd $abs_top_srcdir_win
	abs_top_srcdir_win=`pwd -W`
	cd $abs_top_builddir_win
	AC_SUBST(abs_top_srcdir_win)
	cd
	abs_homedir_win=`pwd -W`
	AC_SUBST(abs_homedir_win)
	cd $abs_top_builddir_win
fi


AC_OUTPUT
