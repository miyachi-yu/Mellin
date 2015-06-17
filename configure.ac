#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.0)

AC_INIT( Mellin, 1.0.0, slab-qcd@nucl.phys.titech.ac.jp )
AM_INIT_AUTOMAKE

AC_CONFIG_SRCDIR([.])
AC_CONFIG_HEADER([config.h])

AC_PREFIX_DEFAULT(~/mellin)
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_FC
AC_PROG_F77
AC_PROG_INSTALL
AC_PROG_LIBTOOL

# Checks for libraries.
# FIXME: Replace `main' with a function in `-ldb_cxx':
AC_CHECK_LIB([db_cxx], [main])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([limits.h stddef.h stdlib.h string.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_HEADER_TIME

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_STAT
AC_CHECK_FUNCS([atexit floor getcwd gettimeofday mkdir pow sqrt])

AC_CHECK_PROGS(DOXYGEN,  doxygen)
AC_CHECK_PROGS(ROOTCONF, root-config)
AC_CHECK_PROGS(ROOTCINT, rootcint)
AC_CHECK_PROGS(DOT,      dot)

AC_CHECK_LIB([m], [pow])
AC_CHECK_LIB([g2c],[c_sqrt])
AC_CHECK_LIB([gfortran],[_gfortran_st_write])
AC_CHECK_LIB([pthread],[pthread_exit])

AC_CHECK_HEADERS( getopt.h \
		  db.h \
		  /sw/include/db3/db.h \
		  /sw/include/db4/db.h )

AC_HEADER_STDBOOL
AC_C_INLINE

#
#  check cfortran provided by CERNlib
#
AC_CHECK_FILE(/usr/include/cernlib/${CERN_LEVEL}/cfortran,
	[AC_SUBST(CFORTRAN,/usr/include/cernlib/${CERN_LEVEL}/cfortran)])
AC_CHECK_FILE(/usr/include/cfortran,
	[AC_SUBST(CFORTRAN,/usr/include/cfortran)])
AC_CHECK_FILE(/sw/include/cfortran,
	[AC_SUBST(CFORTRAN,/sw/include/cfortran)])
AC_CHECK_FILE(${CERN_ROOT}/include/cfortran,
	[AC_SUBST(CFORTRAN,${CERN_ROOT}/include/cfortran)])

PKG_CHECK_MODULES(LIBXML, libxml++-2.6 >= 2.6.0)
AC_SUBST(LIBXML_CFLAGS)
AC_SUBST(LIBXML_LIBS)

# Checks for library functions.
AC_CHECK_FUNCS([pow sqrt])

AC_CONFIG_FILES([Makefile
                 setup.sh
                 Doxyfile
                 Tranform/Makefile
		 StrFncParam/Makefile
		 Evolution/Makefile
		 QCD/Makefile
		 PDFSET/Makefile
		 Fragmentation/Makefile
		 polPDF/Makefile
		 polFrag/Makefile
		 unpolPDF/Makefile
		 doc/header.html
		 doc/mellin.css
		 doc/expdata.html
		 Utility/Makefile
		 ExpData/Makefile
		 PDFLIB/Makefile
		 Xsec/Makefile
		 polDIS/Makefile
		 unpolDIS/Makefile
		 unpolDISXsec/Makefile
		 polSIDIS/Makefile
		 unpolSIDIS/Makefile
		 unpolSIDISXsec/Makefile
		 FragXsec_T/Makefile
		 IntegXsec/Makefile
		 IntegUnpolDIS/Makefile
		 IntegUnpolSIDIS/Makefile
		 IntegUnpolDrellYan/Makefile
		 IntegUnpolVBoson/Makefile
		 IntegPolDIS/Makefile
		 IntegPolSIDIS/Makefile
		 IntegPolDrellYan/Makefile
		 IntegPolVBoson/Makefile
		 IntegSPolVBoson/Makefile
		 IntegFragXsec/Makefile
		 Fitting/Makefile
		 ErrorCalc/Makefile
		 DrawTool/Makefile
		 browser/Makefile
                 example/Makefile
		 plotter/Makefile
		 script/Makefile
		 script/setup.sh
		 script/mellin-conf.sh      
		 script/fitting.sh      
		 user_program/Makefile
		 ])
AC_OUTPUT

