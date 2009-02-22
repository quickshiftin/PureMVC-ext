/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2007 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id: header,v 1.16.2.1.2.1 2007/01/01 19:32:09 iliaa Exp $ */

#ifndef PHP_PURE_MVC_H
#define PHP_PURE_MVC_H

#define PUREMVC_SHOULD_LOG_FUNC_IO 0

extern zend_module_entry pure_mvc_module_entry;
#define phpext_pure_mvc_ptr &pure_mvc_module_entry

#ifdef PHP_WIN32
#define PHP_PURE_MVC_API __declspec(dllexport)
#else
#define PHP_PURE_MVC_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#include "zend_interfaces.h"
#include "ext/standard/php_smart_str.h"

/* return object(values) */
/* jacked from http ext.. */
#define RETVAL_OBJECT(o, addref) \
    RETVAL_OBJVAL((o)->value.obj, addref)
#define RETURN_OBJECT(o, addref) \
    RETVAL_OBJECT(o, addref); \
    return
#define RETVAL_OBJVAL(ov, addref) \
    ZVAL_OBJVAL(return_value, ov, addref)
#define RETURN_OBJVAL(ov, addref) \
    RETVAL_OBJVAL(ov, addref); \
    return
#define ZVAL_OBJVAL(zv, ov, addref) \
    (zv)->type = IS_OBJECT; \
    (zv)->value.obj = (ov);\
    if (addref && Z_OBJ_HT_P(zv)->add_ref) { \
        Z_OBJ_HT_P(zv)->add_ref((zv) TSRMLS_CC); \
    }

zval* puremvc_call_method_multi_param(zval **object_pp, zend_class_entry *obj_ce, zend_function **fn_proxy, char *function_name, int function_name_len, zval **retval_ptr_ptr, int param_count, zval **args[] TSRMLS_DC);

#define puremvc_call_method_with_0_params(obj, obj_ce, fn_proxy, function_name, retval) \
	puremvc_call_method(obj, obj_ce, fn_proxy, function_name, sizeof(function_name)-1, retval, 0, NULL, NULL, NULL, NULL TSRMLS_CC)

#define puremvc_call_method_with_1_params(obj, obj_ce, fn_proxy, function_name, retval, arg1) \
	puremvc_call_method(obj, obj_ce, fn_proxy, function_name, sizeof(function_name)-1, retval, 1, arg1, NULL, NULL, NULL TSRMLS_CC)

#define puremvc_call_method_with_2_params(obj, obj_ce, fn_proxy, function_name, retval, arg1, arg2) \
	puremvc_call_method(obj, obj_ce, fn_proxy, function_name, sizeof(function_name)-1, retval, 2, arg1, arg2, NULL, NULL TSRMLS_CC)

#define puremvc_call_method_with_3_params(obj, obj_ce, fn_proxy, function_name, retval, arg1, arg2, arg3) \
	puremvc_call_method(obj, obj_ce, fn_proxy, function_name, sizeof(function_name)-1, retval, 3, arg1, arg2, arg3, NULL TSRMLS_CC)

#define puremvc_call_method_with_4_params(obj, obj_ce, fn_proxy, function_name, retval, arg1, arg2, arg3, arg4) \
	puremvc_call_method(obj, obj_ce, fn_proxy, function_name, sizeof(function_name)-1, retval, 4, arg1, arg2, arg3, arg4 TSRMLS_CC)

PHP_MINIT_FUNCTION(pure_mvc);
PHP_MSHUTDOWN_FUNCTION(pure_mvc);
PHP_RINIT_FUNCTION(pure_mvc);
PHP_RSHUTDOWN_FUNCTION(pure_mvc);
PHP_MINFO_FUNCTION(pure_mvc);

PHP_FUNCTION(confirm_pure_mvc_compiled);	/* For testing, remove later. */

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(pure_mvc)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(pure_mvc)
*/

/* In every utility function you add that needs to use variables 
   in php_pure_mvc_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as PURE_MVC_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define PURE_MVC_G(v) TSRMG(pure_mvc_globals_id, zend_pure_mvc_globals *, v)
#else
#define PURE_MVC_G(v) (pure_mvc_globals.v)
#endif

#endif	/* PHP_PURE_MVC_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
