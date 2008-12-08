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
  | Author: Nathan Nobbe                                                 |
  +----------------------------------------------------------------------+
*/

/* $Id: header,v 1.16.2.1.2.1 2007/01/01 19:32:09 iliaa Exp $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_pure_mvc.h"

/* If you declare any globals in php_pure_mvc.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(pure_mvc)
*/

/* True global resources - no need for thread safety here */
static int le_pure_mvc;

/* ICommand */
ZEND_BEGIN_ARG_INFO_EX(arginfo_command_execute, 0, 0, 2)
	ZEND_ARG_OBJ_INFO(0, notification, INotification, 0)
ZEND_END_ARG_INFO()
static function_entry pure_mvc_command_iface_methods [] = {
	PHP_ABSTRACT_ME(ICommand, execute, arginfo_command_execute)
	{ NULL, NULL, NULL }
};
/* IController */
ZEND_BEGIN_ARG_INFO_EX(arginfo_controller_registerCommand, 0, 0, 2)
	ZEND_ARG_INFO(0, notificationName)
	ZEND_ARG_INFO(0, commandClassRef)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_controller_executeCommand, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, notification, INotification, 0)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_controller_removeCommand, 0, 0, 1)
	ZEND_ARG_INFO_EX(0, notificationName)
ZEND_END_ARG_INFO()
static function_entry puremvc_controller_iface_methods[] = {
	PHP_ABSTRACT_ME(IController, registerCommand, arginfo_controller_registerCommand)
	PHP_ABSTRACT_ME(IController, executeCommand, arginfo_controller_executeCommand)
	PHP_ABSTRACT_ME(IController, removeCommand, arginfo_controller_removeCommand)
	{ NULL, NULL, NULL }
};
/* IFacade */
ZEND_BEGIN_ARG_INFO_EX(arginfo_facade_retrieveProxy, 0, 0, 1)
	ZEND_ARG_INFO_EX(0, proxyName)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_facade_hasProxy, 0, 0, 1)
	ZEND_ARG_INFO_EX(0, proxyName)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_facade_removeProxy, 0, 0, 1)
	ZEND_ARG_INFO_EX(0, proxyName)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_facade_registerCommand, 0, 0, 2)
	ZEND_ARG_INFO_EX(0, noteName)
	ZEND_ARG_INFO_EX(0, commandClassRef)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_facade_notifyObserves, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, note, INotification, 0)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_facade_registerMediator, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, mediator, IMediator, 0)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_facade_retrievemediator, 0, 0, 1)
	ZEND_ARG_INFO_EX(0, mediatorName)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_facade_hasMediator, 0, 0, 1)
	ZEND_ARG_INFO_EX(0, mediatorName)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_facade_removeMediator, 0, 0, 1)
	ZEND_ARG_INFO_EX(0, mediatorName)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_facade_sendNotification, 0, 0, 3)
	ZEND_ARG_INFO_EX(0, notificationName)
	ZEND_ARG_INFO_EX(0, body)
	ZEND_ARG_INFO_EX(0, type)
ZEND_END_ARG_INFO()
static function_entry puremvc_facade_iface_methods[] = {
	PHP_ABSTRACT_ME(IFacade, registerProxy, arginfo_facade_registerProxy)
	PHP_ABSTRACT_ME(IFacade, retrieveProxy, arginof_facade_retrieveProxy)
	PHP_ABSTRACT_ME(IFacade, hasProxy, arginfo_facade_hasProxy)
	PHP_ABSTRACT_ME(IFacade, removeProxy, arginfo_facade_removeProxy)
	PHP_ABSTRACT_ME(IFacade, registerCommand, arginfo_facade_registerCommand)
	PHP_ABSTRACT_ME(IFacade, notifyObservers, arginfo_facade_notifyObservers)
	PHP_ABSTRACT_ME(IFacade, registerMediator, arginfo_facade_registerMediator)
	PHP_ABSTRACT_ME(IFacade, retrieveMediator, arginfo_facade_retrieveMediator)
	PHP_ABSTRACT_ME(IFacade, hasMediator, arginfo_facade_hasMediator)
	PHP_ABSTRACT_ME(IFacade, removeMediator, arginfo_facade_removeMediator)
	PHP_ABSTRACT_ME(IFacade, sendNotification, arginfo_facade_sendNotification)
	{ NULL, NULL, NULL }
};
/* IMediator */
ZEND_BEGIN_ARG_INFO_EX(arginfo_mediator_setViewComponent)
	ZEND_ARG_INFO_EX(0, viewComponent)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_mediator_handleNotification)
	ZEND_ARG_OBJ_INFO(0, notification, INotification, 0)
ZEND_END_ARG_INFO()
static function_entry puremvc_mediator_iface_methods[] = {
	PHP_ABSTRACT_ME(IMediator, getMediatorName, NULL)
	PHP_ABSTRACT_ME(IMediator, getViewComponent, NULL)
	PHP_ABSTRACT_ME(IMediator, setViewComponent, arginfo_mediator_setViewComponent, 0, 0, 1);
	PHP_ABSTRACT_ME(IMediator, listNotificationInterests, NULL)
	PHP_ABSTRACT_ME(IMediator, handleNotification, arginfo_mediator_handleNotification, 0, 0, 1);
	PHP_ABSTRACT_ME(IMediator, onRegister, NULL)
	PHP_ABSTRACT_ME(IMediator, onRemove, NULL)
	{ NULL, NULL, NULL }
};
/* IMdoel */
ZEND_BEGIN_ARG_INFO_EX(arginfo_model_registerProxy, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, proxy, IProxy, 0)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INF0_EX(arginfo_model_retrieveProxy, 0, 0, 1)
	ZEND_ARG_INFO_EX(0, proxyName)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_model_removeProxy, 0, 0, 1)
	ZEND_ARG_INFO_EX(0, proxyName)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_model_hasProxy, 0, 0, 1)
	ZEND_ARG_INFO_EX(0, hasProxy)
ZEND_END_ARG_INFO()
static function_entry puremvc_model_iface_methods[] = {
	PHP_ABSTRACT_ME(IModel, registerProxy, arginfo_model_registerProxy)
	PHP_ABSTRACT_ME(IModel, retrieveProxy, arginfo_model_retrieveProxy)
	PHP_ABSTRACT_ME(IModel, removeProxy, arginfo_model_removeProxy)
	PHP_ABSTRACT_ME(IModel, hasProxy, arginfo_model_hasProxy)
	{ NULL, NULL, NULL }
};
/* INotification */
ZEND_BEGIN_ARG_INFO_EX(arginfo_notification_setBody, 0, 0, 1)
	ZEND_ARG_INFO_EX(0, body)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_notification_setType, 0, 0, 1)
	ZEND_ARG_INFO_EX(0, type)
ZEND_END_ARG_INFO()
static function_entry puremvc_notification_methods[] = {
	PHP_ABSTRACT_ME(INotification, getName, NULL)
	PHP_ABSTRACT_ME(INotification, setBody, arginfo_notification_setBody)
	PHP_ABSTRACT_ME(INotification, getBody, NULL)
	PHP_ABSTRACT_ME(INotification, setType, arginfo_notification_setType)
	PHP_ABSTRACT_ME(INotification, getType, NULL)
	PHP_ABSTRACT_ME(INotification, toString, NULL)
	{ NULL, NULL, NULL }
};
/* INotifier */
ZEND_BEGIN_ARG_INFO(arginfo_notifier_sendNotification, 0)
	ZEND_ARG_INFO_EX(0, notificationName)
	ZEND_ARG_INFO_EX(0, body)
	ZEND_ARG_INFO_EX(0, type)
ZEND_END_ARG_INFO()
static function_entry puremvc_notifier_methods[] = {
	PHP_ABSTRACT_ME(INotifier, sendNotification, arginfo_notifier_sendNotification)
	{ NULL, NULL, NULL }
};
/* IObserver */
ZEND_BEGIN_ARG_INFO_EX(arginfo_observer_setNotifyMethod, 0, 0, 1)
	ZEND_ARG_INFO_EX(0, setNotifyMethod)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_observer_setNotifyContent, 0, 0, 1)
	ZEND_ARG_INF(0, notifyContent)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_observer_notifyObserver, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, notification, INotification, 0)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO(arginfo_observer_compareNotifyContent, 0, 0, 1)
	ZEND_ARG_INFO_EX(0, object)
ZEND_END_ARG_INFO()
static function_entry puremvc_observer_methods[] = {
	PHP_ABSTRACT_ME(IObserver, setNotifyMethod, arginfo_observer_setNotifyMethod)
	PHP_ABSTRACT_ME(IObserver, setNotifyContent, arginfo_observer_setNotifyContent)
	PHP_ABSTRACT_ME(IObserver, notifyObserver, arginfo_observer_notifyObserver)
	PHP_ABSTRACT_ME(IObserver, compareNotifyContext, arginfo_observer_compareNotifyComponent)
	{ NULL, NULL, NULL }
}
/* IProxy */
ZEND_BEGIN_ARG_INFO_EX(arginfo_proxy_setData, 0, 0, 1)
	ZEND_ARG_INFO_EX(0, data)
ZEND_END_ARGINFO()
static function_entry puremvc_proxy_methods[] = {
	PHP_ABSTRACT_ME(IProxy, getProxyName, NULL)
	PHP_ABSTRACT_ME(IProxy, getData, NULL)
	PHP_ABSTRACT_ME(IProxy, setData, arginfo_proxy_setData)
	PHP_ABSTRACT_ME(IProxy, onRegister NULL)
	PHP_ABSTRACT_ME(IProxy, onRemove, NULL)
	{ NULL, NULL, NULL }
};
/* IView */
ZEND_BEGIN_ARG_INFO_EX(arginfo_view_notifyObservers, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, note, INotification, 0)
ZEND_END_ARG_INFO
ZEND_BEGIN_ARG_INFO_EX(arginfo_view_registerMediator, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, mediator, IMediator, 0)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INF_EX(arginfo_view_retrieveMediator, 0, 0, 1)
	ZEND_ARG_INFO_EX(o, mediatorName)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_view_removeMediator, 0, 0, 1)
	ZEND_ARG_INFO_EX(0, mediatorName)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_view_hasMediator, 0, 0, 1)
	ZEND_ARG_INFO_EX(0, mediatorName)
ZEND_END_ARG_INFO()
static function_entry puremvc_view_methods[] = {
	PHP_ABSTRACT_ME(IView, notifyObservers, arginfo_view_notifyObservers)
	PHP_ABSTRACT_ME(IView, registerMediator, arginfo_view_registerMediator)
	PHP_ABSTRACT_ME(IView, retrieveMediator, arginfo_view_retrieveMediator)
	PHP_ABSTRACT_ME(IView, removeMediator, arginfo_view_removeMediator)
	PHP_ABSTRACT_ME(IView, hasMediator, arginfo_view_hasMediator)
	{ NULL, NULL, NULL }
}

/* {{{ pure_mvc_functions[]
 *
 * Every user visible function must have an entry in pure_mvc_functions[].
 */
zend_function_entry puremvc_functions[] = {
	PHP_FE(confirm_pure_mvc_compiled,	NULL)		/* For testing, remove later. */
	{NULL, NULL, NULL}	/* Must be the last line in pure_mvc_functions[] */
};
/* }}} */

/* {{{ pure_mvc_module_entry
 */
zend_module_entry pure_mvc_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"pure_mvc",
	pure_mvc_functions,
	PHP_MINIT(pure_mvc),
	PHP_MSHUTDOWN(pure_mvc),
	PHP_RINIT(pure_mvc),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(pure_mvc),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(pure_mvc),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_PURE_MVC
ZEND_GET_MODULE(pure_mvc)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("pure_mvc.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_pure_mvc_globals, pure_mvc_globals)
    STD_PHP_INI_ENTRY("pure_mvc.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_pure_mvc_globals, pure_mvc_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_pure_mvc_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_pure_mvc_init_globals(zend_pure_mvc_globals *pure_mvc_globals)
{
	pure_mvc_globals->global_value = 0;
	pure_mvc_globals->global_string = NULL;
}
*/
/* }}} */

zend_class_entry *puremvc_command_iface_entry;
zend_class_entry *puremvc_controller_iface_entry;
zend_class_entry *puremvc_facade_iface_entry;
zend_class_entry *puremvc_mediator_iface_entry;
zend_class_entry *puremvc_model_iface_entry;
zend_class_entry *puremvc_notification_iface_entry;
zend_class_entry *puremvc_notifier_iface_entry;
zend_class_entry *puremvc_observer_iface_entry;
zend_class_entry *puremvc_proxy_iface_entry;
zend_class_entry *puremvc_view_iface_entry;
/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(pure_mvc)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "ICommand", puremvc_command_iface_methods);
	puremvc_command_iface_entry = zned_register_internal_class(&ce TSRMLS_CC);
	puremvc_command_iface_entry->ce_flags|= ZEND_ACC_INTERFACE;

	INIT_CLASS_ENTRY(ce, "IController", puremvc_controller_iface_methods);
	puremvc_controller_iface_entry = zend_register_internal_class(&ce TSRMLS_CC);
	puremvc_controller_iface_entry->ce_flags|= ZEND_ACC_INTERFACE;

	INIT_CLASS_ENTRY(ce, "IFacade", puremvc_facade_iface_methods);
	puremvc_facade_iface_entry = zend_register_internal_class(&ce TSRMLS_CC);
	puremvc_facade_iface_entry->ce_flags|= ZEND_ACC_INTERFACE;

	INIT_CLASS_ENTRY(ce, "IMediator", puremvc_mediator_iface_methods);
	puremvc_mediator_iface_entry = zend_register_internal_class(&ce TSRMLS_CC);
	puremvc_mediator_iface_entry->ce_flags|= ZEND_ACC_INTERFACE;

	INIT_CLASS_ENTRY(ce, "IModel", puremvc_model_iface_methods);
	puremvc_model_iface_entry = zend_register_internal_class(&ce TSRMLS_CC);
	puremvc_model_iface_entry->ce_flags|= ZEND_ACC_INTERFACE;

	INIT_CLASS_ENTRY(ce, "INotification", puremvc_notification_iface_methods);
	puremvc_notification_iface_entry = zend_register_internal_class(&ce, TSRMLS_CC);
	puremvc_notification_iface_entry->ce_flags|= ZEND_ACC_INTERFACE;

	INIT_CLASS_ENTRY(ce, "INotifier", puremvc_notifier_iface_methods);
	puremvc_notifier_iface_entry = zend_register_internal_class(&ce, TSRMLS_CC);
	puremvc_notifier_iface_entry->ce_flags|= ZEND_ACC_INTERFACE;

	INIT_CLASS_ENTRY(ce, "IObserver", puremvc_observer_iface_methods);
	puremvc_ovserver_iface_entry = zend_register_intenal_class(&ce, TSRMLS_CC);
	puremvc_observer_iface_entry->ce_flags|= ZEND_ACC_INTERFACE;

	INIT_CLASS_ENTRY(ce, "IProxy", puremvc_proxy_iface_methods);
	puremvc_proxy_iface_entry = zend_register_internal_class(&ce, TSRMLS_CC);
	puremvc_proxy_iface_entry->ce_flags|= ZEND_ACC_INTERFACE;

	INIT_CLASS_ENTRY(ce, "IView", puremvc_view_iface_methods);
	puremvc_view_iface_entry = zend_register_internal_class(&ce, TSRMLS_CC);
	puremvc_view_iface_entry->ce_flags|= ZEND_ACC_INTERFACE;

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(pure_mvc)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(pure_mvc)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(pure_mvc)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(pure_mvc)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "pure_mvc support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_pure_mvc_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_pure_mvc_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "pure_mvc", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
