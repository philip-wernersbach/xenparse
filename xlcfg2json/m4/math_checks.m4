AC_DEFUN([MATH_LIBRARY_ERROR], [
  AC_MSG_ERROR([Missing required math library function!])
])

AC_DEFUN([MATH_SEARCH_LIBS], [
  AC_SEARCH_LIBS($1, [m], [], MATH_LIBRARY_ERROR)
])
