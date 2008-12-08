dnl $Id$
dnl config.m4 for extension pure_mvc

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(pure_mvc, for pure_mvc support,
dnl Make sure that the comment is aligned:
dnl [  --with-pure_mvc             Include pure_mvc support])

dnl Otherwise use enable:

 PHP_ARG_ENABLE(pure_mvc, whether to enable pure_mvc support,
	[  --enable-pure_mvc           Enable pure_mvc support])

if test "$PHP_PURE_MVC" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-pure_mvc -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/pure_mvc.h"  # you most likely want to change this
  dnl if test -r $PHP_PURE_MVC/$SEARCH_FOR; then # path given as parameter
  dnl   PURE_MVC_DIR=$PHP_PURE_MVC
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for pure_mvc files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       PURE_MVC_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$PURE_MVC_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the pure_mvc distribution])
  dnl fi

  dnl # --with-pure_mvc -> add include path
  dnl PHP_ADD_INCLUDE($PURE_MVC_DIR/include)

  dnl # --with-pure_mvc -> check for lib and symbol presence
  dnl LIBNAME=pure_mvc # you may want to change this
  dnl LIBSYMBOL=pure_mvc # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $PURE_MVC_DIR/lib, PURE_MVC_SHARED_LIBADD)
  AC_DEFINE(HAVE_PURE_MVCLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong pure_mvc lib version or lib not found])
  dnl ],[
  dnl   -L$PURE_MVC_DIR/lib -lm -ldl
  dnl ])
  dnl
  dnl PHP_SUBST(PURE_MVC_SHARED_LIBADD)

  PHP_NEW_EXTENSION(pure_mvc, pure_mvc.c, $ext_shared)
fi
