if OS_WINDOWS_32
iss:
	iscc.exe "-dabs_top_srcdir_win=$(abs_top_srcdir_win)" "-dabs_top_builddir_win=$(abs_top_builddir_win)" "-dBAM_UTILS32" "-dMyAppVersion=$(VERSION)" "-dMY_HOME=$(abs_homedir_win)" "$(srcdir)/bam-utils.iss"
endif

if OS_WINDOWS_64
iss:
	iscc.exe "-dabs_top_srcdir_win=$(abs_top_srcdir_win)" "-dabs_top_builddir_win=$(abs_top_builddir_win)" "-dBAM_UTILS64" "-dMyAppVersion=$(VERSION)" "-dMY_HOME=$(abs_homedir_win)" "$(srcdir)/bam-utils.iss"
endif

EXTRA_DIST = bam-utils.iss

