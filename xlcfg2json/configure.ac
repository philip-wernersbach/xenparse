AC_INIT([xlcfg2json], [1.0], [philip.wernersbach@gmail.com])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])

AC_PROG_CC_C99

AC_CHECK_HEADERS([libxlu_internal.h], [], [
  AC_MSG_CHECKING([for local libxlu_internal.h])
  AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM([#include "libxlu_internal.h"], [])], 
    [AC_MSG_RESULT([yes])], 
    [
      AC_MSG_RESULT([no])
      AC_MSG_ERROR([Please install libxlu_internal.h, or copy it to the source directory.])
    ]
  )]
)

MATH_SEARCH_LIBS([floor])
MATH_SEARCH_LIBS([pow])

GETOPT_CHECK

XEN_SEARCH_LIBS(libxl_ctx_alloc)
XEN_SEARCH_LIBS(xlu_cfg_init)
XEN_SEARCH_LIBS(xlu_cfg_readfile)
XEN_SEARCH_LIBS(xlu_cfg_destroy)

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
