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
static
ZEND_BEGIN_ARG_INFO(arginfo_command_execute, 0)
	ZEND_ARG_OBJ_INFO(0, "notification", "INotification", 0)
ZEND_END_ARG_INFO();
static
function_entry puremvc_command_iface_methods [] = {
	PHP_ABSTRACT_ME(ICommand, execute, arginfo_command_execute)
	{ NULL, NULL, NULL }
};
/* IController */
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_controller_registerCommand, 0, 0, 2)
	ZEND_ARG_INFO(0, notificationName)
	ZEND_ARG_INFO(0, commandClassRef)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_controller_executeCommand, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, "notification", "INotification", 0)
ZEND_END_ARG_INFO();
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_controller_removeCommand, 0, 0, 1)
	ZEND_ARG_INFO(0, notificationName)
ZEND_END_ARG_INFO()
static
function_entry puremvc_controller_iface_methods[] = {
	PHP_ABSTRACT_ME(IController, registerCommand, arginfo_controller_registerCommand)
	PHP_ABSTRACT_ME(IController, executeCommand, arginfo_controller_executeCommand)
	PHP_ABSTRACT_ME(IController, removeCommand, arginfo_controller_removeCommand)
	{ NULL, NULL, NULL }
};
/* IFacade */
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_facade_retrieveProxy, 0, 0, 1)
	ZEND_ARG_INFO(0, proxyName)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_facade_hasProxy, 0, 0, 1)
	ZEND_ARG_INFO(0, proxyName)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_facade_removeProxy, 0, 0, 1)
	ZEND_ARG_INFO(0, proxyName)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_facade_registerProxy, 0, 0, 2)
	ZEND_ARG_INFO(0, noteName)
	ZEND_ARG_INFO(0, commandClassRef)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_facade_registerCommand, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, "command", "ICommand", 0)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_facade_notifyObservers, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, "note", "INotification", 0)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_facade_registerMediator, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, "mediator", "IMediator", 0)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_facade_retrieveMediator, 0, 0, 1)
	ZEND_ARG_INFO(0, mediatorName)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_facade_hasMediator, 0, 0, 1)
	ZEND_ARG_INFO(0, mediatorName)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_facade_removeMediator, 0, 0, 1)
	ZEND_ARG_INFO(0, mediatorName)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_facade_sendNotification, 0, 0, 3)
	ZEND_ARG_INFO(0, notificationName)
	ZEND_ARG_INFO(0, body)
	ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO()
static function_entry puremvc_facade_iface_methods[] = {
	PHP_ABSTRACT_ME(IFacade, registerProxy, arginfo_facade_registerProxy)
	PHP_ABSTRACT_ME(IFacade, retrieveProxy, arginfo_facade_retrieveProxy)
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
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_mediator_setViewComponent, 0, 0, 1)
	ZEND_ARG_INFO(0, viewComponent)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_mediator_handleNotification, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, "notification", "INotification", 0)
ZEND_END_ARG_INFO()
static function_entry puremvc_mediator_iface_methods[] = {
	PHP_ABSTRACT_ME(IMediator, getMediatorName, NULL)
	PHP_ABSTRACT_ME(IMediator, getViewComponent, NULL)
	PHP_ABSTRACT_ME(IMediator, setViewComponent, arginfo_mediator_setViewComponent)
	PHP_ABSTRACT_ME(IMediator, listNotificationInterests, NULL)
	PHP_ABSTRACT_ME(IMediator, handleNotification, arginfo_mediator_handleNotification)
	PHP_ABSTRACT_ME(IMediator, onRegister, NULL)
	PHP_ABSTRACT_ME(IMediator, onRemove, NULL)
	{ NULL, NULL, NULL }
};
/* IMdoel */
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_model_registerProxy, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, "proxy", "IProxy", 0)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_model_retrieveProxy, 0, 0, 1)
	ZEND_ARG_INFO(0, proxyName)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_model_removeProxy, 0, 0, 1)
	ZEND_ARG_INFO(0, proxyName)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_model_hasProxy, 0, 0, 1)
	ZEND_ARG_INFO(0, hasProxy)
ZEND_END_ARG_INFO()
static function_entry puremvc_model_iface_methods[] = {
	PHP_ABSTRACT_ME(IModel, registerProxy, arginfo_model_registerProxy)
	PHP_ABSTRACT_ME(IModel, retrieveProxy, arginfo_model_retrieveProxy)
	PHP_ABSTRACT_ME(IModel, removeProxy, arginfo_model_removeProxy)
	PHP_ABSTRACT_ME(IModel, hasProxy, arginfo_model_hasProxy)
	{ NULL, NULL, NULL }
};
/* INotification */
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_notification_setBody, 0, 0, 1)
	ZEND_ARG_INFO(0, body)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_notification_setType, 0, 0, 1)
	ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO()
static function_entry puremvc_notification_iface_methods[] = {
	PHP_ABSTRACT_ME(INotification, getName, NULL)
	PHP_ABSTRACT_ME(INotification, setBody, arginfo_notification_setBody)
	PHP_ABSTRACT_ME(INotification, getBody, NULL)
	PHP_ABSTRACT_ME(INotification, setType, arginfo_notification_setType)
	PHP_ABSTRACT_ME(INotification, getType, NULL)
	PHP_ABSTRACT_ME(INotification, toString, NULL)
	{ NULL, NULL, NULL }
};
/* INotifier */
static
ZEND_BEGIN_ARG_INFO(arginfo_notifier_sendNotification, 0)
	ZEND_ARG_INFO(0, notificationName)
	ZEND_ARG_INFO(0, body)
	ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO()
static function_entry puremvc_notifier_iface_methods[] = {
	PHP_ABSTRACT_ME(INotifier, sendNotification, arginfo_notifier_sendNotification)
	{ NULL, NULL, NULL }
};
/* IObserver */
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_observer_setNotifyMethod, 0, 0, 1)
	ZEND_ARG_INFO(0, setNotifyMethod)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_observer_setNotifyContent, 0, 0, 1)
	ZEND_ARG_INFO(0, notifyContent)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_observer_notifyObserver, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, "notification", "INotification", 0)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_observer_compareNotifyContent, 0, 0, 1)
	ZEND_ARG_INFO(0, object)
ZEND_END_ARG_INFO()
static function_entry puremvc_observer_iface_methods[] = {
	PHP_ABSTRACT_ME(IObserver, setNotifyMethod, arginfo_observer_setNotifyMethod)
	PHP_ABSTRACT_ME(IObserver, setNotifyContent, arginfo_observer_setNotifyContent)
	PHP_ABSTRACT_ME(IObserver, notifyObserver, arginfo_observer_notifyObserver)
	PHP_ABSTRACT_ME(IObserver, compareNotifyContext, arginfo_observer_compareNotifyContent)
	{ NULL, NULL, NULL }
};
/* IProxy */
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_proxy_setData, 0, 0, 1)
	ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()
static function_entry puremvc_proxy_iface_methods[] = {
	PHP_ABSTRACT_ME(IProxy, getProxyName, NULL)
	PHP_ABSTRACT_ME(IProxy, getData, NULL)
	PHP_ABSTRACT_ME(IProxy, setData, arginfo_proxy_setData)
	PHP_ABSTRACT_ME(IProxy, onRegister, NULL)
	PHP_ABSTRACT_ME(IProxy, onRemove, NULL)
	{ NULL, NULL, NULL }
};
/* IView */
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_view_notifyObservers, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, "note", "INotification", 0)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_view_registerMediator, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, "mediator", "IMediator", 0)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_view_retrieveMediator, 0, 0, 1)
	ZEND_ARG_INFO(0, "mediatorName")
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_view_removeMediator, 0, 0, 1)
	ZEND_ARG_INFO(0, "mediatorName")
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_view_hasMediator, 0, 0, 1)
	ZEND_ARG_INFO(0, "mediatorName")
ZEND_END_ARG_INFO()
static function_entry puremvc_view_iface_methods[] = {
	PHP_ABSTRACT_ME(IView, notifyObservers, arginfo_view_notifyObservers)
	PHP_ABSTRACT_ME(IView, registerMediator, arginfo_view_registerMediator)
	PHP_ABSTRACT_ME(IView, retrieveMediator, arginfo_view_retrieveMediator)
	PHP_ABSTRACT_ME(IView, removeMediator, arginfo_view_removeMediator)
	PHP_ABSTRACT_ME(IView, hasMediator, arginfo_view_hasMediator)
	{ NULL, NULL, NULL }
};

/* Controller */
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_controller_executeCommand, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, "note", "INotification", 0)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_controller_registerCommand, 0, 0, 2)
	ZEND_ARG_INFO_EX(0, notificationName)
	ZEND_ARG_INFO_EX(0, commandClassRef)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_controller_hasCommand, 0, 0, 1)
	ZEND_ARG_INFO_EX(0, commandName)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_controller_removeCommand, 0, 0, 1)
	ZEND_ARG_INFO_EX(0, commandName)
ZEND_END_ARG_INFO()
static function_entry puremvc_controller_class_methods[] = {
	PHP_ME(Controller, __construct, NULL, ZEND_ACC_PRIVATE)
	PHP_ME(Controller, initializeController, NULL, ZEND_ACC_PROTECTED)
	PHP_ME(Controller, getInstance, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(Controller, executeCommand, arginfo_controller_executeCommand, ZEND_ACC_PUBLIC)
	PHP_ME(Controller, registerCommand arginfo_controller_registerCommand, ZEND_ACC_PUBLIC)
	PHP_ME(Controller, hasCommand, arginfo_controller_hasCommand, ZEND_ACC_PUBLIC)
	PHP_ME(Controller, removeCommand, arginfo_controller_removeCommand, ZEND_ACC_PUBLIC)
};

void puremvc_controller_addprops(zval *objvar)
{
	char *propname;
	int propname_len;
	/* protected */
	zend_mangle_property_name(&propname, &propname_len,
		"*", 1, "view", sizeof("view")-1, 0);
	add_property_zval_ex(objvar, propname, propname_len,
		"Properties", 1 TRMLS_CC);
	efree(propname);
	zend_mangle_property_name(&propname, &propname_len,
		"*", 1, "commandMap", sizeof("commaneMap")-1, 0);
	add_property_zval_ex(objvar, propname, propname_len,
		"Properties", 1 TRMLS_CC);
	efree(propname);
	zend_mangle_property_name(&propname, &propname_len,
		"*", 1, "instance", sizeof("instance")-1, 1);
	add_property_zval_ex(objvar, &propname, &propname_len,
		"Properties", 1 TRMLS_CC);
	efree(propname);
}
/* Model */
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_model_class_registerProxy, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, "proxy", "IProxy", 0)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_model_class_retrieveProxy, 0, 0, 1)
	ZEND_ARG_INFO_EX(0, proxyName)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_model_class_removeProxy, 0, 0, 1)
	ZEND_ARG_INFO_EX(0, proxyName)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_model_class_hasProxy, 0, 0, 1)
	ZEND_ARG_INFO_EX(0, proxyName)
ZEND_END_ARG_INFO()
static function_entry puremvc_model_class_methods[] = {
	PHP_FE(Model, __construct, NULL, ZEND_ACC_PRIVATE)
	PHP_FE(Model, initializeModel, NULL, ZEND_ACC_PROTECTED)
	PHP_FE(Model, getInstance, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_FE(Model, registerProxy, arginfo_model_class_registerProxy, ZEND_ACC_PUBLIC)
	PHP_FE(Model, retrieveProxy, arginfo_model_class_retrieveProxy, ZEND_ACC_PUBLIC)
	PHP_FE(Model, removeProxy, arginfo_model_class_removeProxy, ZEND_ACC_PUBLIC)
	PHP_FE(Model, hasProxy, arginfo_model_class_hasProxy, ZEND_ACC_PUBLIC)
};

void puremvc_model_addprops(zval *objvar)
{
	char *propname;
	int propname_len;
	/*protected*/
	zend_mangle_property_name(&propname, &propname_len,
		"*", 1, "instance", sizeof("instance")-1, 0);
	add_property_zval_ex(objvar, propname, propname_len,
		"Properties", 1 TRMLS_CC);
	efree(propname);
	zend_mangle_property_name(&propname, propname_len,
		"*", 1, "proxyMap", sizeof("proxyMap")-1, 0);
	add_property_zval_ex(objvar, propname, propname_len,
		"Properties", 1 TRMLS_CC);
	efree(propname);
}
/* View */
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_view_registerObserver, 0, 0, 2)
	ZEND_ARG_INFO_EX(0, notificationName)
	ZEND_ARG_OBJ_INFO(0, "observer", "IObserver", 0)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_view_notifyObservers, 0, 0 , 1)
	ZEND_ARG_OBJ_INFO(0, "notefication", "INotification", 0)
ZEND_END_ARG_INFO();
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_view_registerMediator, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, "mediator", "IMeidator", 0)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_view_retrieveMediator, 0, 0, 1)
	ZEND_ARG_INFO_EX(0, mediatorName)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_view_hasMediator, 0, 0, 1)
	ZEND_ARG_INFO_EX(0, mediatorName)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_view_removeMediator, 0, 0, 1)
	ZEND_ARG_INFO_EX(0, mediaName)
ZEND_END_ARG_INFO()
static function_entry puremvc_view_class_methods[] = {
	PHP_ME(View, __construct, NULL, ZEND_ACC_PRIVATE);
	PHP_ME(View, initializeView, NULL, ZEND_ACC_PROTECTED);
	PHP_ME(View, getInstance, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC);
	PHP_ME(View, registerObserver, arginfo_view_registerObserver, ZEND_ACC_PUBLIC);
	PHP_ME(View, notifyObservers, arginfo_view_notifyObservers, ZEND_ACC_PUBLIC);
	PHP_ME(View, registerMediator, arginfo_view_registerMediator, ZEND_ACC_PUBLIC);
	PHP_ME(View, retrieveMediator, arginfo_view_retrieveMediator, ZEND_ACC_PUBLIC);
	PHP_ME(View, hasMediator, arginfo_view_hasMediator, ZEND_ACC_PUBLIC);
	PHP_ME(View, removewMediator, arginfo_view_removeMediator, ZEND_ACC_PUBLIC);
};

void puremvc_view_addprops(zval *objvar)
{
	char *propname;
	int propname_len;
	/* proptected */
	zend_mangle_property_name(&propname, &propname_len,
		"*", 1, "instance", sizeof("instance")-1, 0);
	add_property_zval_ex(objvar, propname, propname_len,
		"Properties", 1 TRMLS_CC);
	efree(propname);
	zend_mangle_property_name(&propname, propname_len,
		"*", 1, "mediatorMap", sizeof("mediatorMap")-1, 0);
	add_property_zval_ex(objvar, propname, propname_len,
		"Properties", 1 TRMLS_CC);
	efree(propname);
	zend_mangle_property_name(&propname, propname_len,
		"*", 1, "observerMap", sizeof("observerMap")-1, 0);
	add_property_zval_ex(objvar, propname, propname_len,
		"Properties", 1 TRMLS_CC);
	efree(propname);
}
/* {{{ pure_mvc_functions[]
 *
 * Every user visible function must have an entry in pure_mvc_functions[].
 */
zend_function_entry pure_mvc_functions[] = {
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

/* interfaces */
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
/* classes */
zend_class_entry *pure_mvc_cotroller_class_entry;
zend_class_entry *pure_mvc_model_class_entry;
zend_class_entry *pure_mvc_view_class_entry;
/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(pure_mvc)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "ICommand", puremvc_command_iface_methods);
	puremvc_command_iface_entry = zend_register_internal_class(&ce TSRMLS_CC);
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
	puremvc_notification_iface_entry = zend_register_internal_class(&ce TSRMLS_CC);
	puremvc_notification_iface_entry->ce_flags|= ZEND_ACC_INTERFACE;

	INIT_CLASS_ENTRY(ce, "INotifier", puremvc_notifier_iface_methods);
	puremvc_notifier_iface_entry = zend_register_internal_class(&ce TSRMLS_CC);
	puremvc_notifier_iface_entry->ce_flags|= ZEND_ACC_INTERFACE;

	INIT_CLASS_ENTRY(ce, "IObserver", puremvc_observer_iface_methods);
	puremvc_observer_iface_entry = zend_register_internal_class(&ce TSRMLS_CC);
	puremvc_observer_iface_entry->ce_flags|= ZEND_ACC_INTERFACE;

	INIT_CLASS_ENTRY(ce, "IProxy", puremvc_proxy_iface_methods);
	puremvc_proxy_iface_entry = zend_register_internal_class(&ce TSRMLS_CC);
	puremvc_proxy_iface_entry->ce_flags|= ZEND_ACC_INTERFACE;

	INIT_CLASS_ENTRY(ce, "IView", puremvc_view_iface_methods);
	puremvc_view_iface_entry = zend_register_internal_class(&ce TSRMLS_CC);
	puremvc_view_iface_entry->ce_flags|= ZEND_ACC_INTERFACE;

	INIT_CLASS_ENTRY(ce, "Controller", puremvc_controller_class_methods);
	puremvc_controller_class_entry = zend_register_internal_class(&ce TSRMLS_CC);
	zend_class_implements(puremvc_controller_class_entry TRMLS_CC,
				1, puremvc_controller_iface_entry);

	INIT_CLASS_ENTRY(ce, "Model", puremvc_model_class_methods);
	puremvc_model_class_entry = zend_register_internal_class(&ce TSRMLS_CC);
	zend_class_implements(puremvc_model_class_entry TRMLS_CC,
				1, puremvc_model_iface_entry);

	INIT_CLASS_ENTRY(ce, "View", puremvc_view_class_methods);
	puremvc_view_class_entry = zend_register_internal_class(&ce TSRMLS_CC);
	zend_class_implements(puremvc_view_class_entry TRMLS_CC,
				1, puremvc_view_iface_entry);

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
