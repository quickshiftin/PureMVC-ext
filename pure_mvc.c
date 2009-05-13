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

zval* puremvc_call_method_multi_param(zval **object_pp, zend_class_entry *obj_ce, zend_function **fn_proxy, char *function_name, int function_name_len, zval **retval_ptr_ptr, int param_count, zval **params[] TSRMLS_DC)
{
	int result;
	zend_fcall_info fci;
	zval z_fname;
	zval *retval;
	HashTable *function_table;

	fci.size = sizeof(fci);
	/*fci.function_table = NULL; will be read form zend_class_entry of object if needed */
	fci.object_pp = object_pp;
	fci.function_name = &z_fname;
	fci.retval_ptr_ptr = retval_ptr_ptr ? retval_ptr_ptr : &retval;
	fci.param_count = param_count;
	fci.params = params;
	fci.no_separation = 1;
	fci.symbol_table = NULL;

	if (!fn_proxy && !obj_ce) {
		/* no interest in caching and no information already present that is
		 * needed later inside zend_call_function. */
		ZVAL_STRINGL(&z_fname, function_name, function_name_len, 0);
		fci.function_table = !object_pp ? EG(function_table) : NULL;
		result = zend_call_function(&fci, NULL TSRMLS_CC);
	} else {
		zend_fcall_info_cache fcic;

		fcic.initialized = 1;
		if (!obj_ce) {
			obj_ce = object_pp ? Z_OBJCE_PP(object_pp) : NULL;
		}
		if (obj_ce) {
			function_table = &obj_ce->function_table;
		} else {
			function_table = EG(function_table);
		}
		if (!fn_proxy || !*fn_proxy) {
			if (zend_hash_find(function_table, function_name, function_name_len+1, (void **) &fcic.function_handler) == FAILURE) {
				/* error at c-level */
				zend_error(E_CORE_ERROR, "Couldn't find implementation for method %s%s%s", obj_ce ? obj_ce->name : "", obj_ce ? "::" : "", function_name);
			}
			if (fn_proxy) {
				*fn_proxy = fcic.function_handler;
			}
		} else {
			fcic.function_handler = *fn_proxy;
		}
		fcic.calling_scope = obj_ce;
		fcic.object_pp = object_pp;
		result = zend_call_function(&fci, &fcic TSRMLS_CC);
	}
	if (result == FAILURE) {
		/* error at c-level */
		if (!obj_ce) {
			obj_ce = object_pp ? Z_OBJCE_PP(object_pp) : NULL;
		}
		if (!EG(exception)) {
			zend_error(E_CORE_ERROR, "Couldn't execute method %s%s%s", obj_ce ? obj_ce->name : "", obj_ce ? "::" : "", function_name);
		}
	}
	if (!retval_ptr_ptr) {
		if (retval) {
			zval_ptr_dtor(&retval);
		}
		return NULL;
	}
	return *retval_ptr_ptr;
}

/* {{{ puremvc_call_method
	This is a hacked version of zend_call_method which supports up to 4 parameters for php-5.2
 Only returns the returned zval if retval_ptr != NULL */
zval* puremvc_call_method(zval **object_pp, zend_class_entry *obj_ce, zend_function **fn_proxy, char *function_name, int function_name_len, zval **retval_ptr_ptr, int param_count, zval* arg1, zval* arg2, zval* arg3, zval* arg4 TSRMLS_DC)
{
	zval **args[4];

	args[0] = &arg1;
	args[1] = &arg2;
	args[2] = &arg3;
	args[3] = &arg4;

	return puremvc_call_method_multi_param(object_pp, obj_ce, fn_proxy, function_name, function_name_len, retval_ptr_ptr, param_count, args TSRMLS_DC);
}
/* }}} */
/* @param char *classname class name of method
 * @param char *methodname name of mehtod
 * @param int isStart (0|1) is this the start or end of the method
 *
 * a simple method used to create function traces of php-level functions, in the
 * php error log.  log entries will be in the form of
 * >>> classname::methodname
 * <<< classname::methodname
 * where the former is the result of isStart = 1
 *
 * @note the logging can be toggled via the PUREMVC_SHOULD_LOG_FUNC_IO constant
 */
void puremvc_log_func_io(char *classname, char *methodname, int isStart)
{
	if(PUREMVC_SHOULD_LOG_FUNC_IO == 1) {
		if(isStart)
			php_error_docref(NULL TSRMLS_CC, E_WARNING,
				">>> %s::%s", classname, methodname);
		else
			php_error_docref(NULL TSRMLS_CC, E_WARNING,
				"<<< %s::%s", classname, methodname);
	}
}

/* @param zval **val string name of class to instantiate
 * @param zval *notification INotification instance to pass to execute()
 *
 * this method is the guts of a foreach loop in MacroCommand::execute.  the
 * loop iterates over an array of strings calling this method to instantiate
 * the classes those strings name, and then invoke the execute() method on said
 * instance, supplying an INotification as an argument
 *
 * @note: this will remove the command from the hash after executing it
 */
int puremvc_execute_command_in_hash(zval **val, zval *notification TSRMLS_DC)
{
	zval tmpcpy, *subCommandInstance, *return_value;
	zend_class_entry **ce, *ze_p;

	//// @TODO: use SEPARATE_ZVAL HERE
	tmpcpy  = **val;
	zval_copy_ctor(&tmpcpy);
	INIT_PZVAL(&tmpcpy);
	convert_to_string(&tmpcpy);

	php_strtolower(Z_STRVAL_PP(val), Z_STRLEN_PP(val));

	if(zend_lookup_class(Z_STRVAL(tmpcpy), Z_STRLEN(tmpcpy),
		&ce TSRMLS_CC)  == FAILURE) {
		php_error_docref(NULL TSRMLS_CC, E_NOTICE,
		"pure_mvc-internal: Class [%s] does not exist.", Z_STRVAL_PP(val));
//		zend_dtor(&tmpcpy);
		RETURN_FALSE;
	} else {
		ze_p = *ce;
	}

	MAKE_STD_ZVAL(subCommandInstance);
	object_init_ex(subCommandInstance, ze_p);

	// attempt to call constructor, if it exists 
	if(zend_hash_exists(&ze_p->function_table,
		"__construct", strlen("__construct") + 1)) {
		zend_call_method_with_0_params(&subCommandInstance, *ce, NULL,
			"__construct", NULL);
	} else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unable to find \
			constructor for alleged class [%s]\n", ze_p->name);
	}
	//zval_dtor(&tmpcpy);

	// attempt to call the execute() method 
	if(zend_hash_exists(&ze_p->function_table,
		"execute", strlen("execute") +1)) {
		zend_call_method_with_1_params(&subCommandInstance, *ce, NULL,
			"execute", NULL, notification);
	}
	//zval_dtor(&tmpcpy);
	//zval_dtor(subCommandInstance);
	return SUCCESS;
}
/* If you declare any globals in php_pure_mvc.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(pure_mvc)
*/

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
/* core - classes */
zend_class_entry *puremvc_controller_ce;
zend_class_entry *puremvc_model_ce;
zend_class_entry *puremvc_view_ce;
/* pattern - classes */
zend_class_entry *puremvc_notification_ce;
zend_class_entry *puremvc_notifier_ce;
zend_class_entry *puremvc_observer_ce;
zend_class_entry *puremvc_facade_ce;
zend_class_entry *puremvc_mediator_ce;
zend_class_entry *puremvc_macrocommand_ce;
zend_class_entry *puremvc_simplecommand_ce;
zend_class_entry *puremvc_proxy_ce;


/* True global resources - no need for thread safety here */
static int le_pure_mvc;
/* Controller */
/* {{{ proto private void Controller::__construct()
	constructor */
PHP_METHOD(Controller, __construct)
{
	puremvc_log_func_io("Controller", "__construct", 1);

	zval *this, *commandMap;
	zend_class_entry *this_ce;

	MAKE_STD_ZVAL(commandMap);
	array_init(commandMap);

	this = getThis();
	this_ce = zend_get_class_entry(this);
	zend_update_property(this_ce, this, "commandMap",
		sizeof("commandMap")-1, commandMap TSRMLS_CC);

	zend_call_method_with_0_params(&this, this_ce, NULL,
			"initializecontroller", NULL);

	puremvc_log_func_io("Controller", "__construct", 0);
}
/* }}} */
/* {{{ proto protected void Controller::initializeController()
		a constructor hook method for children */
PHP_METHOD(Controller, initializeController)
{
	zval *this, *view;

	MAKE_STD_ZVAL(view);
	object_init_ex(view, puremvc_view_ce);
	
	this = getThis();
	zend_call_method_with_0_params(NULL, puremvc_view_ce, NULL,
				"getinstance", &view);

	zend_update_property(zend_get_class_entry(this), this,
		"view", sizeof("view")-1, view TSRMLS_CC);
}
/* }}} */
/* {{{ proto public object Controller::getInstance()
		singleton f method for Controller */
PHP_METHOD(Controller, getInstance)
{
	zval *instance;

	puremvc_log_func_io("Controller", "getInstance", 1);

	instance = zend_read_static_property(puremvc_controller_ce, "instance",
					sizeof("instance")-1, 1 TSRMLS_CC);

	if(Z_TYPE_P(instance) == IS_NULL) {
		object_init_ex(return_value, puremvc_controller_ce);
		zend_call_method_with_0_params(&return_value, puremvc_controller_ce, NULL,
				"__construct", NULL);
		ZVAL_ADDREF(return_value);
		zend_update_static_property(puremvc_controller_ce, "instance", sizeof("instance")-1,
				return_value TSRMLS_CC);
	} else {
		RETVAL_OBJECT(instance, 1);
	}

	puremvc_log_func_io("Controller", "getInstance", 0);
	return;
}
/* }}} */
/* {{{ proto public void executeCommand(object $notification)
		execute a command using the given INotification */
PHP_METHOD(Controller, executeCommand)
{
	zval *this, *notification, *notificationName, *commandMap;
	zval *hasCommand, *commandName, **tmp;
	zend_class_entry *this_ce;

	/* setup this & this_ce */
	this = getThis();
	this_ce = zend_get_class_entry(this);

	/* parse params */
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o",
			&notification) == FAILURE) {
		return;
	}

	/* populate commandMap from $this->commandMap */
	commandMap = zend_read_property(this_ce, this,
						"commandMap", strlen("commandMap"), 1 TSRMLS_CC);
	/* get the notification name
	 * call getName() on the INotification instance
	 */
	zend_call_method_with_0_params(&notification,
			zend_get_class_entry(notification), NULL,
				"getname", &notificationName);


	/* TODO: LOOK IN $this->commandMap for notificationName */
	zend_call_method_with_1_params(&this, this_ce, NULL,
			"hascommand", &hasCommand, notificationName);

	/* bail, if this Controller doesnt have a command for the supplied
	 * INotification
	 */
	if(!Z_BVAL_P(hasCommand)) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING,
			"Controller::executeCommand(); no registered Command \
			for INotification name [%s]", Z_STRVAL_P(notificationName));
		return;
	}

	/* load up the command name and bail if we cant find it..
	 * the data will be populated in tmp
	 */
	if(zend_hash_find(Z_ARRVAL_P(commandMap), Z_STRVAL_P(notificationName),
			Z_STRLEN_P(notificationName)+1, (void**)&tmp) == FAILURE) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING,
			"Controller::executeCommand(); somehow we failed fetch \
			the command name for INotificationName [%s]", Z_STRVAL_P(notificationName));
		return;
	}

	/* create an instance of the (comman name) class named in the commandMap
	 * and call execute() on it, passing it the INotification instance */
//// TODO: determine if deleting the array elements is acceptable..
	puremvc_execute_command_in_hash(tmp, notification);
}
/* }}} */
/* {{{ proto public void Controller::registerCommand( string notificationName, string commandClassName)
		register a command for a particular notification */
PHP_METHOD(Controller, registerCommand)
{
	zval *this, *notificationName, *commandClassName, *commandMap;
	zval *observer, *executeCommandStr, *view;
	zend_class_entry *this_ce;
	char *rawNotificationName = NULL, *rawCommandClassName = NULL;
	int rawNotificationNameLength, rawCommandClassNameLength;

	/* parse parameters */
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss",
		&rawNotificationName, &rawNotificationNameLength,
		&rawCommandClassName, &rawCommandClassNameLength) == FAILURE) {
		return;
	}

	/* populate the notificationName zval via the raw components
	 * zend_parse_parameters gives us
	 */
	MAKE_STD_ZVAL(notificationName);
	ZVAL_STRINGL(notificationName, rawNotificationName,
			rawNotificationNameLength, 1);
	MAKE_STD_ZVAL(commandClassName);

	/* populate the commandClassName zval via the raw components
	 * zend_parse_parameters gives us
	 */
	ZVAL_STRINGL(commandClassName, rawCommandClassName,
			rawCommandClassNameLength, 1);
	ZVAL_ADDREF(commandClassName);

	/* create a string zval holding "executeCommand" */
	MAKE_STD_ZVAL(executeCommandStr);
	ZVAL_STRINGL(executeCommandStr, "executeCommand",
			strlen("executeCommand"), 1);

	/* setup this & this_ce */
	this = getThis();
	this_ce = zend_get_class_entry(this);

	/* populate the commandMap from this instance */
	commandMap = zend_read_property(this_ce, this, "commandMap",
					strlen("commandMap"), 1 TSRMLS_CC);

	/* create an entry in $this->commandMap, with a key is a notification name,
	 * and the value is a command name
	 */
	HashTable *cMapHt = Z_ARRVAL_P(commandMap);
	if(cMapHt)
		zend_hash_update(cMapHt, rawNotificationName,
				rawNotificationNameLength+1, (void*)&commandClassName,
				sizeof(zval*), NULL);

	/* instantiate an  Observer */
	MAKE_STD_ZVAL(observer);
	object_init_ex(observer, puremvc_observer_ce);

	/* call the Observer constructor, passing telling it to call 
	 * $this->executeCommand() it observes a particular notification
	 */
	zend_call_method_with_2_params(&observer, puremvc_observer_ce, NULL,
			"__construct", NULL, executeCommandStr, this);

	/* read thew View from this instance */
	view = zend_read_property(this_ce, this, "view",
				strlen("view"), 1 TSRMLS_CC);

	/* call $this->view->registerObserver() passing the notification name */
	zend_call_method_with_2_params(&view, zend_get_class_entry(view), NULL,
			"registerobserver", NULL, notificationName, observer);
}
/* }}} */
/* {{{ proto public bool Controller::hasCommand(string notificationName)
		ask the controller if it has a command registered for a given notification */
PHP_METHOD(Controller, hasCommand)
{
	zval *this, *commandMap, *notificationName;
	char *rawNotificationName;
	int rawNotificationNameLength;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
			&rawNotificationName, &rawNotificationNameLength) == FAILURE) {
		return;
	}

	this = getThis();
	commandMap = zend_read_property(zend_get_class_entry(this), this,
						"commandMap", strlen("commandMap"), 1 TSRMLS_CC);

	if(zend_hash_exists(Z_ARRVAL_P(commandMap),
			rawNotificationName, rawNotificationNameLength+1)) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
}
/* }}} */
/* {{{ proto public void Controller::removeCommand
		remove a command for a given notification */
PHP_METHOD(Controller, removeCommand)
{
	zval *this, *commandMap;
	zend_class_entry *this_ce;
	char *rawNotificationName;
	int rawNotificationNameLength;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
			&rawNotificationName, &rawNotificationNameLength) == FAILURE) {
		return;
	}

	this = getThis();
	commandMap = zend_read_property(zend_get_class_entry(this), this, "commandMap",
						strlen("commandMap"), 1 TSRMLS_CC);

	zend_hash_del(Z_ARRVAL_P(commandMap), rawNotificationName, rawNotificationNameLength+1);
}
/* }}} */
/* Model */
/* {{{ proto private void Model::__construct()
		constructor */
PHP_METHOD(Model, __construct)
{
	puremvc_log_func_io("Model", "__construct", 1);

	zval *this, *freshArray;
	zend_class_entry *this_ce;

	MAKE_STD_ZVAL(freshArray);
	array_init(freshArray);

	this = getThis();
	this_ce = zend_get_class_entry(this);

	zend_update_property(this_ce, this, "proxyMap",
			sizeof("proxyMap")-1, freshArray);

	zend_call_method_with_0_params(&this, this_ce, NULL,
			"initializemodel", NULL);

	puremvc_log_func_io("Model", "__construct", 0);
}
/* }}} */
/* {{{ proto protected void initializeModel
			hook method for children, to be run by the parent during construction */
PHP_METHOD(Model, initializeModel)
{
}
/* }}} */
/* {{{ proto static public object Model::getInstance()
			Model singleton method */
PHP_METHOD(Model, getInstance)
{
	zval *instance;

	puremvc_log_func_io("Model", "getInstance", 1);

	instance = zend_read_static_property(puremvc_model_ce, "instance",
					sizeof("instance")-1, 1 TSRMLS_CC);

	if(Z_TYPE_P(instance) == IS_NULL) {
		object_init_ex(return_value, puremvc_model_ce);
		zend_call_method_with_0_params(&return_value, puremvc_model_ce, NULL,
				"__construct", NULL);
		ZVAL_ADDREF(return_value);
		zend_update_static_property(puremvc_model_ce, "instance", sizeof("instance")-1,
				return_value TSRMLS_CC);
	} else {
		RETVAL_OBJECT(instance, 1);
	}

	puremvc_log_func_io("Model", "getInstance", 0);
	return;
}
/* }}} */
/* {{{ proto public void Model::registerProxy(object $proxy)
			register a proxy with the Model */
PHP_METHOD(Model, registerProxy)
{
	zval *this, *proxyMap, *proxy, *proxyName;
	zend_class_entry *this_ce, *proxy_ce;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o",
			&proxy) == FAILURE) {
		return;
	}

	this = getThis();
	this_ce = zend_get_class_entry(this);
	proxy_ce = zend_get_class_entry(proxy);

	zend_call_method_with_0_params(&proxy, proxy_ce, NULL,
				"getproxyname", &proxyName);
	if(!proxyName) {
		return;
	}

	proxyMap = zend_read_property(this_ce, this,
				"proxyMap", sizeof("proxyMap")-1, 1 TSRMLS_CC);

	ZVAL_ADDREF(proxy);
	add_assoc_zval(proxyMap, Z_STRVAL_P(proxyName), proxy);

	zend_call_method_with_0_params(&proxy, proxy_ce, NULL,
				"onregister", NULL);
}
/* }}} */
/* {{{ proto public object function Model::retrieveProxy(string proxyName)
			fetch an IProxy instance from the Model by supplying its name */
PHP_METHOD(Model, retrieveProxy)
{
	zval *this, *proxyName, *proxyMap, **tmp;
	zend_class_entry *this_ce;
	char *rawProxyName;
	int rawProxyNameLength;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
			&rawProxyName, &rawProxyNameLength) == FAILURE) {
		return;
	}

	this = getThis();
	this_ce = zend_get_class_entry(this);
	proxyMap = zend_read_property(this_ce, this, "proxyMap",
					sizeof("proxyMap")-1, 0 TSRMLS_CC);

	if(zend_hash_find(Z_ARRVAL_P(proxyMap), rawProxyName,
		rawProxyNameLength+1, (void**)&tmp) == SUCCESS) { 
		RETVAL_OBJECT(*tmp, 1);
	}
}
/* }}} */
/* {{{ proto public object Model::removeProxy(string proxyName)
			remove a proxy from the Model by supplying its name */
PHP_METHOD(Model, removeProxy)
{
	zval *this, *proxyName, *proxyMap, *proxy, **tmp;
	zend_class_entry *this_ce;
	char *rawProxyName;
	int rawProxyNameLength;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
			&rawProxyName, &rawProxyNameLength) == FAILURE) {
		return;
	}

	this = getThis();
	this_ce = zend_get_class_entry(this);
	proxyMap = zend_read_property(this_ce, this, "proxyMap",
					sizeof("proxyMap")-1, 1 TSRMLS_CC);

	if(zend_hash_find(Z_ARRVAL_P(proxyMap), rawProxyName,
			rawProxyNameLength+1, (void**)&tmp) == SUCCESS) {
		zend_hash_del(Z_ARRVAL_P(proxyMap), rawProxyName, rawProxyNameLength);

		zend_update_property(this_ce, this, "proxyMap",
				sizeof("proxyMap")-1, proxyMap);

		RETVAL_OBJECT(*tmp, 1);
	}
}
/* }}} */
/* {{{ proto public bool Model::hasProxy(string proxyName)
			determine if the Model has a registered IProxy, by the name of proxyName */
PHP_METHOD(Model, hasProxy)
{
	zval *this, *proxyMap, *proxyName;
	char *rawProxyName;
	int rawProxyNameLength;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
			&rawProxyName, &rawProxyNameLength) == FAILURE) {
		return;
	}

	this = getThis();	
	proxyMap = zend_read_property(zend_get_class_entry(this), this,
						"proxyMap", sizeof("proxyMap")-1, 1 TSRMLS_CC);

	if(zend_hash_exists(Z_ARRVAL_P(proxyMap),
			rawProxyName, rawProxyNameLength+1)) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
}
/* }}} */
/* View */
/* {{{ proto private void View::__construct()
		constructor */
PHP_METHOD(View, __construct)
{
	puremvc_log_func_io("View", "__construct", 1);

	zval *this, *mediatorMap, *observerMap;
	zend_class_entry *this_ce;

	MAKE_STD_ZVAL(mediatorMap);
	array_init(mediatorMap);
	MAKE_STD_ZVAL(observerMap);
	array_init(observerMap);

	this = getThis();
	this_ce = zend_get_class_entry(this);
	zend_update_property(this_ce, this, "mediatorMap",
			sizeof("mediatorMap")-1, mediatorMap TSRMLS_CC);
	zend_update_property(this_ce, this, "observerMap",
			sizeof("observerMap")-1, observerMap TSRMLS_CC);

	zend_call_method_with_0_params(&this, this_ce, NULL,
			"initializeview", NULL);

	puremvc_log_func_io("View", "__construct", 0);
}
/* }}} */
/* {{{ proto protected void View::initializeView()
		hook method for children, called by the constructor */
PHP_METHOD(View, initializeView)
{
}
/* }}} */
/* {{{ proto public static object View::getInstance()
		View singleton method */
PHP_METHOD(View, getInstance)
{
	zval *instance;

	puremvc_log_func_io("View", "getInstance", 1);

	instance = zend_read_static_property(puremvc_view_ce, "instance",
				sizeof("instance")-1, 1 TSRMLS_CC);

	if(Z_TYPE_P(instance) == IS_NULL) {
		object_init_ex(return_value, puremvc_view_ce);
		zend_call_method_with_0_params(&return_value, puremvc_view_ce, NULL,
				"__construct", NULL);
		zend_update_static_property(puremvc_view_ce, "instance", sizeof("instance")-1,
				return_value TSRMLS_CC);
	} else {
		RETVAL_OBJECT(instance, 1);
	}
	
	puremvc_log_func_io("View", "getInstance", 0);
	return;
}
/* }}} */
/* {{{ proto public void View::registerObserver(string notificationName, object $observer)
			register an observer for a particular notificationName */
PHP_METHOD(View, registerObserver)
{
	zval *this, *notificationName, *observer, *observerMap;
	zend_class_entry *this_ce;
	char *rawNotificationName;
	int rawNotificationNameLength;

	/* parse parameters */
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "so",
			&rawNotificationName, &rawNotificationNameLength,
			&observer) == FAILURE) {
		return;
	}

	/* setup this && this_ce */
	this = getThis();
	this_ce = zend_get_class_entry(this);

	/* read the observerMap from this instance */
	observerMap = zend_read_property(this_ce, this, "observerMap",
					strlen("observerMap"), 1 TSRMLS_CC);

	HashTable *oMapHt = Z_ARRVAL_P(observerMap);

	/* check to see if we've yet registered an Observer for the supplied
	 * notification name yet
	 */
	if(!zend_hash_exists(oMapHt, rawNotificationName, rawNotificationNameLength+1)) {
		/* if not, lets initialize an array and drop the Observer in there
		 * w/ the notification name as the key
		 */
		zval *tmpMap;

		/* create an array for the observers corresponding to the notfication name */
		MAKE_STD_ZVAL(tmpMap);
		array_init(tmpMap);
		/* add the observer to said array */
		ZVAL_ADDREF(observer);
		add_next_index_zval(tmpMap, observer);

		/* now add the new array to the observerMap */
		ZVAL_ADDREF(tmpMap);
		if(oMapHt)
			zend_hash_update(oMapHt, rawNotificationName,
					rawNotificationNameLength+1, (void**)&tmpMap,
					sizeof(zval*), NULL);
	} else {
		/* otherwise read observerMap from the instance and append to the
		 * existing list of Observers, the value will be stored in tmpMap
		 */
		zval **tmpMap;

		//// TODO check SUCCESS / FAILURE ..
		zend_hash_find(oMapHt, rawNotificationName,
			rawNotificationNameLength+1, (void**)&tmpMap);

		ZVAL_ADDREF(observer);
		add_next_index_zval(*tmpMap, observer);
	}
}
/* }}} */
/* {{{ proto public void View::notifyObservers(object notification)
		alert registered observers of a particualr event */
PHP_METHOD(View, notifyObservers)
{
	zval *this, *notification, *observerMap, *notificationName;
	zend_class_entry *this_ce;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o",
			notification) == FAILURE) {	
		return;
	}

	observerMap = zend_read_property(this_ce, this, "observerMap",
					sizeof("observerMap")-1, 1 TSRMLS_CC);

	zend_call_method_with_0_params(&this, this_ce, NULL,
				"getname", &notificationName); 

	if(!zend_hash_exists(Z_ARRVAL_P(observerMap), Z_STRVAL_P(notificationName),
			Z_STRLEN_P(notificationName))) {
		return;
	}

//// TODO I HAVE A FOREACH YOU NEED TO DO
}
/* }}} */
/* {{{ proto public void View::registerMediator(object mediator)
		register an IMediator with the View */
PHP_METHOD(View, registerMediator)
{
	zval *this, *mediator;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o",
		mediator) == FAILURE) {	
		return;
	}

//// TODO
}

/* }}} */
/* {{{ proto public object View::retrieveMediator(string mediatorName)
		fetch a registered IMediator by name */
PHP_METHOD(View, retrieveMediator)
{
	zval *this, *mediatorName;
	char *rawMediatorName;
	int rawMediatorNameLength;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
			&rawMediatorName, &rawMediatorNameLength) == FAILURE) {
		return;
	}

//// TODO
}

/* }}} */
/* {{{ proto public bool View::hasMediator(string mediatorName)
		determine if the IMediator with name mediatorName is registered with the View */
PHP_METHOD(View, hasMediator)
{
	zval *this, *mediatorName;
	char *rawMediatorName;
	int rawMediatorNameLength;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
			&rawMediatorName, &rawMediatorNameLength) == FAILURE) {
		return;
	}

//// TODO
}
/* }}} */
/* {{{ proto public object View::removeMediator(string mediatorName)
		remove the registered IMediator with name mediatorName */
PHP_METHOD(View, removeMediator)
{
	zval *this, *mediatorName;
	char *rawMediatorName;
	int rawMediatorNameLength;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
			&rawMediatorName, &rawMediatorNameLength) == FAILURE) {
		return;
	}

//// TODO
}
/* }}} */
/* MacroCommand */
/* {{{ proto protected void MacroCommand::initializeMacroCommand
   this is a hook method to be overriden by subclasses, which will be called by the constructor */
PHP_METHOD(MacroCommand, initializeMacroCommand)
{
	puremvc_log_func_io("MacroCommand", "initializeMacroCommand", 1);
	puremvc_log_func_io("MacroCommand", "initializeMacroCommand", 0);
}
/* }}} */
/* {{{ proto public void MacroCommand::__construct()
   Constructor. */
PHP_METHOD(MacroCommand, __construct)
{
	puremvc_log_func_io("MacroCommand", "__construct", 1);
	zval *this;

	this = getThis();
	array_init(return_value);

	zend_update_property(puremvc_macrocommand_ce, this, "subCommands", sizeof("subCommands")-1,
			return_value TSRMLS_CC);

	zend_call_method_with_0_params(&this, zend_get_class_entry(this), NULL,
			"initializemacrocommand", NULL);

/*  the puremvc php code doesnt call the parent constructor,
	but its something that may merit a question on the mailin list / forum
/////
	zend_call_method_with_0_params(&this, puremvc_notifier_ce, NULL,
			"__construct", NULL);
*/

	puremvc_log_func_io("MacroCommand", "__construct", 0);
}
/* }}} */
/* {{{ proto public void MacroCommand::addSubCommand(string commandClassRef)
   add a subcommand to this MacroCommand */
PHP_METHOD(MacroCommand, addSubCommand)
{
	puremvc_log_func_io("MacroCommand", "addSubCommand", 1);

	zval *this, *subCommands, *subCommand;
	char *rawSubCommand;
	int rawSubCommandLength;

	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
			&rawSubCommand, &rawSubCommandLength) == FAILURE) {
		RETURN_NULL();
	}

	MAKE_STD_ZVAL(subCommand);
	ZVAL_STRINGL(subCommand, rawSubCommand, rawSubCommandLength, 1);

	this = getThis();
	zend_class_entry *this_ce = zend_get_class_entry(this);
	subCommands = zend_read_property(puremvc_macrocommand_ce, this, "subCommands",
					sizeof("subCommands")-1, 1 TSRMLS_CC);

	ZVAL_ADDREF(subCommand);
	add_next_index_zval(subCommands, subCommand);

	zend_update_property(puremvc_macrocommand_ce, this, "subCommands", sizeof("subCommands")-1,
			subCommands TSRMLS_CC);

	puremvc_log_func_io("MacroCommand", "addSubCommand", 0);
}
/* }}} */
/* {{{ proto public final void MacroCommand::execute
   run this MacroCommand (execute subCommands that have been assigned to this MacroCommand) */ 
PHP_METHOD(MacroCommand, execute)
{
	puremvc_log_func_io("MacroCommand", "execute", 1);

	zval *this, *notification, *subCommandsVal;

	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o",
			&notification) == FAILURE) {
		RETURN_NULL();
	}

	this = getThis();

	/* get the $subCommands HashTable, wraped in a zval */
	subCommandsVal = zend_read_property(puremvc_macrocommand_ce, this, "subCommands",
					sizeof("subCommands")-1, 1 TSRMLS_CC);

	if(Z_TYPE_P(subCommandsVal) != IS_ARRAY) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING,
			"$this->subCommands is not an array");
		RETURN_FALSE;
	}

	/*  iterate over the subCommands calling the execute() method on
	 *	instances of them we make; poping them off $subCommands as we go
	 */
	for( zend_hash_internal_pointer_reset(Z_ARRVAL_P(subCommandsVal));
		 zend_hash_has_more_elements(Z_ARRVAL_P(subCommandsVal)) == SUCCESS;
		 zend_hash_move_forward(Z_ARRVAL_P(subCommandsVal)) ) {
			zval **ppzval;
			ulong index;

			if(zend_hash_get_current_data(Z_ARRVAL_P(subCommandsVal), (void**)&ppzval)
				== FAILURE) {
				php_error_docref(NULL TSRMLS_CC, E_WARNING,
					"couldnt get current data");
				continue;
			}
		puremvc_execute_command_in_hash(ppzval, notification);
	}
	zend_hash_clean(Z_ARRVAL_P(subCommandsVal));

	puremvc_log_func_io("MacroCommand", "execute", 0);
}
/* }}} */
/* SimpleCommand */
/* {{{ proto public final void SimpleCommand::__construct
    */ 
PHP_METHOD(SimpleCommand, __construct)
{
	zval *this;

	puremvc_log_func_io("SimpleCommand", "__construct", 1);
	zend_call_method_with_0_params(NULL, puremvc_facade_ce, NULL, "getinstance",
			&return_value);
	this = getThis();
	zend_update_property(puremvc_simplecommand_ce, this, "facade", sizeof("facade")-1,
			return_value TSRMLS_CC);

	puremvc_log_func_io("SimpleCommand", "__construct", 0);
}
/* }}} */
/* {{{ proto public final void SimpleCommand::execute
   essentially a hook method, designed to be overriden by child methods */ 
PHP_METHOD(SimpleCommand, execute)
{
}
/* Facade */
/* {{{ proto public final void Facade::__construct
	Facade constructor, calls initializeFacade
    */ 
PHP_METHOD(Facade, __construct)
{
	zval *this;

	puremvc_log_func_io("Facade", "__construct", 1);

	this = getThis();

	zend_call_method_with_0_params(&this, zend_get_class_entry(this), NULL, "initializefacade",
			NULL);

	puremvc_log_func_io("Facade", "__construct", 0);
}
/* }}} */
/* {{{ proto public final void Facade::initializeFacade
	initialize the model, view and controller instance variables
    */ 
PHP_METHOD(Facade, initializeFacade)
{
	zval *this;

	puremvc_log_func_io("Facade", "iniitializeFacade", 1);

	this = getThis();

	zend_call_method_with_0_params(&this, zend_get_class_entry(this), NULL, "initializemodel",
			NULL);
	zend_call_method_with_0_params(&this, zend_get_class_entry(this), NULL, "initializecontroller",
			NULL);
	zend_call_method_with_0_params(&this, zend_get_class_entry(this), NULL, "initializeview",
			NULL);

	puremvc_log_func_io("Facade", "iniitializeFacade", 0);
}
/* }}} */
/* {{{ proto public static object Facade::getInstance
	get the Facade instance and store a handle to it
    */ 
PHP_METHOD(Facade, getInstance)
{
	zval *instance;

	puremvc_log_func_io("Facade", "getInstance", 1);

	instance = zend_read_static_property(puremvc_facade_ce, "instance",
					sizeof("instance")-1, 0 TSRMLS_CC);
	if(Z_TYPE_P(instance) == IS_NULL) {
		object_init_ex(return_value, puremvc_facade_ce);
		zend_call_method_with_0_params(&return_value, puremvc_facade_ce, NULL,
				"__construct", NULL);
		ZVAL_ADDREF(return_value);
		zend_update_static_property(puremvc_facade_ce, "instance", sizeof("instance")-1,
				return_value TSRMLS_CC);
	} else {
		RETVAL_OBJECT(instance, 1);
	}

	puremvc_log_func_io("Facade", "getInstance", 0);

	return;
}
/* }}} */
/* {{{ proto public void Facade::initializeController
	get the Controller instance and store a handle to it
    */ 
PHP_METHOD(Facade, initializeController)
{
	zval *this;
	
	puremvc_log_func_io("Facade", "initializeController", 1);

	this = getThis();

	/* just return the instance in return_value if it already exists */
	if(Z_TYPE_P(return_value) != IS_NULL) {
		puremvc_log_func_io("Facade", "initializeController", 0);
		return;
	}

	this = getThis();
	zend_call_method_with_0_params(&this, puremvc_controller_ce, NULL, "getinstance",
				&return_value);
	zend_update_property(puremvc_facade_ce, this, "controller", sizeof("controller")-1,
			return_value TSRMLS_CC);

	puremvc_log_func_io("Facade", "initializeController", 0);
}
/* }}} */
/* {{{ proto public final void Facade::initializeModel
	get the Model instance and store a handle to it
    */ 
PHP_METHOD(Facade, initializeModel)
{
	zval *this;
	
	puremvc_log_func_io("Facade", "initializeModel", 1);

	this = getThis();
	//return_value = zend_read_property(&puremvc_macrocommand_ce, this, "model",
	return_value = zend_read_property(zend_get_class_entry(this), this, "model",
					sizeof("model")-1, 1 TSRMLS_CC);

	/* just return the instance in return_value if it already exists */
	if(Z_TYPE_P(return_value) != IS_NULL) {
		puremvc_log_func_io("Facade", "initializeModel", 0);
		return;
	}
	this = getThis();
	zend_call_method_with_0_params(&this, puremvc_model_ce, NULL, "getinstance",
				&return_value);
/// add a ref here ?
	zend_update_property(puremvc_facade_ce, this, "model", sizeof("model")-1,
			return_value TSRMLS_CC);

	puremvc_log_func_io("Facade", "initializeModel", 0);
}
/* }}} */
/* {{{ proto public final void Facade::initializeView
	get the View instance and store a handle to it
    */ 
PHP_METHOD(Facade, initializeView)
{
	zval *this;
	
	puremvc_log_func_io("Facade", "initializeView", 1);

	this = getThis();
	return_value = zend_read_property(zend_get_class_entry(this), this, "view",
					sizeof("view")-1, 1 TSRMLS_CC);

	/* just return the instance in return_value if it already exists */
	if(Z_TYPE_P(return_value) != IS_NULL) {
		puremvc_log_func_io("Facade", "initializeView", 0);
		return;
	}
	this = getThis();
	zend_call_method_with_0_params(&this, puremvc_view_ce, NULL, "getinstance",
				&return_value);
	zend_update_property(puremvc_facade_ce, this, "view", sizeof("view")-1,
			return_value TSRMLS_CC);

	puremvc_log_func_io("Facade", "initializeView", 0);
}
/* }}} */
/* {{{ proto public final void Facade::notifyObservers(INotification notification)
	invoke $this->view->notifyObservers, supplyting $inotification to it
    */ 
PHP_METHOD(Facade, notifyObservers)
{
	zval *view, *this, *inotification;

	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o",
			&inotification) == FAILURE) {
		return;
	}

	this = getThis();
	view = zend_read_property(zend_get_class_entry(this), this, "view",
					sizeof("view")-1, 1 TSRMLS_CC);

	zend_call_method_with_1_params(&view, zend_get_class_entry(view), NULL,
			"notifyobservers", NULL, inotification);
}
/* }}} */
/* {{{ proto public final void Facade::registerCommand(notificationName, commandClassRef)
	register commandClassRef w/ the controller composed by Facade
    */ 
PHP_METHOD(Facade, registerCommand)
{
	zval *this, *notificationName, *commandClassName, *controller;
	char *rawNotificationName, *rawCommandClassName;
	int rawNotificationNameLength, rawCommandClassNameLength;

	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss",
			&rawNotificationName, &rawNotificationNameLength,
			&rawCommandClassName, &rawCommandClassNameLength) == FAILURE) {
		return;
	}

	this = getThis();
	controller = zend_read_property(puremvc_facade_ce, this, "controller",
					sizeof("controller")-1, 1 TSRMLS_CC);

	if(Z_TYPE_P(controller) == IS_NULL)
		return;

	MAKE_STD_ZVAL(notificationName);
	MAKE_STD_ZVAL(commandClassName);
	ZVAL_STRING(notificationName, rawNotificationName, 1);
	ZVAL_STRING(commandClassName, rawCommandClassName, 1);

	zend_call_method_with_2_params(&controller, zend_get_class_entry(controller),
			NULL, "registercommand", NULL, notificationName, commandClassName);
}
/* }}} */
/* {{{ proto public final void Facade::removeCommand(notificationName)
	remove the command identified by notificationName
    */ 
PHP_METHOD(Facade, removeCommand)
{
	zval *this, *notificationName, *controller;
	char *rawNotificationName;
	int rawNotificationNameLength;

	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
			&rawNotificationName, &rawNotificationNameLength) == FAILURE) {
		return;
	}

	MAKE_STD_ZVAL(notificationName);
	ZVAL_STRINGL(notificationName, rawNotificationName, rawNotificationNameLength, 1);

	this = getThis();
	controller = zend_read_property(puremvc_facade_ce, this, "controller",
					sizeof("controller")-1, 1 TSRMLS_CC);

	zend_call_method_with_1_params(&controller, zend_get_class_entry(controller), NULL,
			"removecommand", NULL, notificationName);
}
/* }}} */
/* {{{ proto public final void Facade::hasCommand(string notificationName)
	determine if the command referenced by notificationName has been registered on the controller
    */ 
PHP_METHOD(Facade, hasCommand)
{
	zval *this, *notificationName, *controller;
	char *rawNotificationName;
	int rawNotificationNameLength;

	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
			&rawNotificationName, &rawNotificationNameLength) == FAILURE) {
		return;
	}

	MAKE_STD_ZVAL(notificationName);
	ZVAL_STRINGL(notificationName, rawNotificationName, rawNotificationNameLength, 1);

	this = getThis();
	controller = zend_read_property(zend_get_class_entry(this), this, "controller",
					sizeof("controller")-1, 1 TSRMLS_CC);
	zend_call_method_with_1_params(&controller, zend_get_class_entry(controller), NULL,
			"hascommand", &return_value, notificationName);
}
/* }}} */
/* {{{ proto public final void Facade::registerProxy(IProxy proxy)
	register commandClassRef w/ the controller composed by Facade
    */ 
PHP_METHOD(Facade, registerProxy)
{
	zval *this, *proxy, *model;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o",
			&proxy) == FAILURE) {
		return;
	}

	this = getThis();

	model = zend_read_property(puremvc_facade_ce, this, "model",
					sizeof("model")-1, 1 TSRMLS_CC);

	if(proxy == IS_NULL)
		return;

	zend_call_method_with_1_params(&model, puremvc_model_ce, NULL, "registerproxy",
			NULL, proxy);
}
/* }}} */
/* {{{ proto public final mixed Facade::retrieveProxy(string proxyName)
	retrieve proxy with name, proxyName, if it has been registered
    */ 
PHP_METHOD(Facade, retrieveProxy)
{
	zval *this, *proxyName, *model;
	char *rawProxyName;
	int rawProxyNameLength;

	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
			&rawProxyName, &rawProxyNameLength) == FAILURE) {
		return;
	}

	MAKE_STD_ZVAL(proxyName);
	ZVAL_STRINGL(proxyName, rawProxyName, rawProxyNameLength, 1);

	this = getThis();
	model = zend_read_property(zend_get_class_entry(this), this, "model",
				sizeof("model")-1, 1 TSRMLS_CC);

	zend_call_method_with_1_params(&model, zend_get_class_entry(model), NULL,
			"retrieveproxy", &return_value, proxyName);
}
/* }}} */
/* {{{ proto public final boolean Facade::hasProxy(string proxyName)
	determine if proxy is registered w/ the model
    */ 
PHP_METHOD(Facade, hasProxy)
{
	zval *this, *proxyName, *model;
	char *rawProxyName;
	int rawProxyNameLength;

	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
			&rawProxyName, &rawProxyNameLength) == FAILURE) {
		return;
	}

	MAKE_STD_ZVAL(proxyName);
	ZVAL_STRINGL(proxyName, rawProxyName, rawProxyNameLength, 1);

	this = getThis();
	model = zend_read_property(zend_get_class_entry(this), this, "model",
				sizeof("model")-1, 1 TSRMLS_CC);

	zend_call_method_with_1_params(&model, zend_get_class_entry(model), NULL, "hasproxy",
		&return_value, proxyName);
}
/* }}} */
/* {{{ proto public final mixed Facade::removeProxy(string proxyName)
	remove registered proxy w/ proxyName, if said proxy has been registered
    */ 
PHP_METHOD(Facade, removeProxy)
{
	zval *this, *proxyName, *model;
	char *rawProxyName;
	int rawProxyNameLength;

	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
			&rawProxyName, &rawProxyNameLength) == FAILURE) {
		return;
	}

	MAKE_STD_ZVAL(proxyName);
	ZVAL_STRINGL(proxyName, rawProxyName, rawProxyNameLength, 1);

	this = getThis();
	model = zend_read_property(zend_get_class_entry(this), this, "model",
			sizeof("model")-1, 1 TSRMLS_CC);

	zend_call_method_with_1_params(&model, zend_get_class_entry(model), NULL,
			"removeproxy", NULL, proxyName);
}
/* }}} */
/* {{{ proto public final mixed Facade::removeProxy(IMediator mediator)
	register mediator w/ the Facade
    */ 
PHP_METHOD(Facade, registerMediator)
{
	zval *this, *mediator, *view;

	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o",
			&mediator) == FAILURE) {
		return;
	}

	this = getThis();
	view = zend_read_property(zend_get_class_entry(this), this, "view",
				sizeof("view")-1, 1 TSRMLS_CC);
/*
	zend_call_method_with_1_params(&view, puremvc_view_ce, NULL, "registermediator",
			NULL, mediator);
*/
}
/* }}} */
/* {{{ proto public final mixed Facade::retrieveMediator(string mediatorName)
	retrieve the registered mediator w/ mediatorName, if it has been registered
    */ 
PHP_METHOD(Facade, retrieveMediator)
{
	zval *this, *mediatorName, *view;
	char *rawMediatorName;
	int rawMediatorNameLength;

	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
			&rawMediatorName, &rawMediatorNameLength) == FAILURE) {
		return;
	}

	MAKE_STD_ZVAL(mediatorName);
	ZVAL_STRINGL(mediatorName, rawMediatorName, rawMediatorNameLength, 1);

	this = getThis();
	view = zend_read_property(zend_get_class_entry(this), this, "view",
				sizeof("view")-1, 1 TSRMLS_CC);

	zend_call_method_with_1_params(&view, zend_get_class_entry(view), NULL,
			"retrievemediator", &return_value, mediatorName);
}
/* }}} */
/* {{{ proto public final mixed Facade::hasMediator(string mediatorName)
	retrieve the registered mediator w/ mediatorName, if it has been registered
    */ 
PHP_METHOD(Facade, hasMediator)
{
	zval *this, *mediatorName, *view;
	char *rawMediatorName;
	int rawMediatorNameLength;

	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
			&rawMediatorName, &rawMediatorNameLength) == FAILURE) {
		return;
	}

	MAKE_STD_ZVAL(mediatorName);
	ZVAL_STRINGL(mediatorName, rawMediatorName, rawMediatorNameLength, 1);

	this = getThis();
	view = zend_read_property(zend_get_class_entry(this), this, "view",
				sizeof("view")-1, 1 TSRMLS_CC);
	zend_call_method_with_1_params(&view, zend_get_class_entry(view), NULL,
			"hasmediator", &return_value, mediatorName);
}
/* }}} */
/* {{{ proto public final mixed Facade::removeMediator(string mediatorName)
	retrieve the registered mediator w/ mediatorName, if it has been registered
    */ 
PHP_METHOD(Facade, removeMediator)
{
	zval *this, *mediatorName, *view;
	char *rawMediatorName;
	int rawMediatorNameLength;

	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
			&rawMediatorName, &rawMediatorNameLength) == FAILURE) {
		return;
	}

	MAKE_STD_ZVAL(mediatorName);
	ZVAL_STRINGL(mediatorName, rawMediatorName, rawMediatorNameLength, 1);

	this = getThis();
	view = zend_read_property(zend_get_class_entry(this), this, "view",
				sizeof("view")-1, 1 TSRMLS_CC);

	zend_call_method_with_1_params(&view, zend_get_class_entry(view), NULL,
			"removemediator", NULL, mediatorName);
}
/* }}} */
/* {{{ proto public final mixed Facade::removeMediator(string mediatorName [, string body=null [, string type=null ]])
	retrieve the registered mediator w/ mediatorName, if it has been registered
    */ 
PHP_METHOD(Facade, sendNotification)
{
	zval *this, *tmpcpy, *notification, *notificationName, *body, *type, *params[3];
	zval *tmpcpy2, *ctor = NULL;
	char *rawNotificationName = NULL, *rawBody = NULL, *rawType = NULL;
	int rawNotificationNameLength, rawBodyLength, rawTypeLength, paramCount = 0;
	int numParams = ZEND_NUM_ARGS();

	if( zend_parse_parameters(numParams TSRMLS_CC, "s|ss",
			&rawNotificationName, &rawNotificationNameLength,
			&rawBody, &rawBodyLength, &rawType, &rawTypeLength) == FAILURE) {
		return;
	}

	MAKE_STD_ZVAL(notificationName);
	ZVAL_STRINGL(notificationName, rawNotificationName, rawNotificationNameLength, 1);

	MAKE_STD_ZVAL(body);
	if(numParams == 2 || numParams == 3) {
		ZVAL_STRINGL(body, rawBody, rawBodyLength, 1);
	} else {
		ZVAL_NULL(body);
	}

	MAKE_STD_ZVAL(type);
	if(numParams == 3) {
		ZVAL_STRINGL(type, rawType, rawTypeLength, 1);
	} else {
		ZVAL_NULL(type);
	}

	this = getThis();

	/* instantiate a Notification */
	MAKE_STD_ZVAL(notification);
	object_init_ex(notification, puremvc_notification_ce);

	// build HashTable w/ 2 zvals to mark constructor //
	MAKE_STD_ZVAL(tmpcpy);
	ZVAL_STRING(tmpcpy, "__construct", 1);

	puremvc_call_method_with_3_params(&notification, puremvc_notification_ce,
		NULL, "__construct", NULL, notificationName, body, type);

	/* notify observers, passing the above create notification */
	zend_call_method_with_1_params(&this, zend_get_class_entry(this), NULL,
			"notifyobservers", NULL, notification);
}
/* }}} */
/* Mediator */
/* {{{ proto public void construct(string mediatorName [mixed viewComponent = null])
		constructor
	*/
PHP_METHOD(Mediator, __construct)
{
	zval *this, *mediatorName, *viewComponent = NULL, *facade, **tmp;
	zend_class_entry *this_ce;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|o",
		&mediatorName, &viewComponent) == FAILURE) {
		return;
	}

	/* get the facade instance */
	zend_call_method_with_0_params(NULL, puremvc_facade_ce, NULL,
			"getinstance", &facade);

	/* setup this && this_ce */
	this = getThis();
	this_ce = zend_get_class_entry(this);

	/* store a reference to the facade in this instance */
	zend_update_property(this_ce, this, "facade",
			strlen("facade"), facade TSRMLS_CC);

	/* create a zval pointing w/ the value null */
	if(!viewComponent) {
		MAKE_STD_ZVAL(viewComponent);
		ZVAL_NULL(viewComponent);
	}

	/* store the viewComponent on this instance */
	if(viewComponent != NULL) {
		zend_update_property(this_ce, this, "viewComponent",
			strlen("viewComponent"), viewComponent TSRMLS_CC);
	}

	/* potentially set the mediatorName */
	if(Z_TYPE_P(mediatorName) == IS_NULL) {
		if(zend_hash_find(&this_ce->constants_table, "NAME", strlen("NAME")+1,
			(void**)&tmp) == FAILURE) {
			return;
		}
		zend_update_property(this_ce, this, "mediatorName",
			strlen("mediatorName"), *tmp);
	} else {
		convert_to_string(mediatorName);

		zend_update_property(this_ce, this, "mediatorName",
			strlen("mediatorName"), mediatorName);
	}
}
/* }}} */
/* {{{ proto public string Mediator::getMediatorName()
		get the name for this instance */
PHP_METHOD(Mediator, getMediatorName)
{
	zval *this, *mediatorName;

	this = getThis();
	mediatorName = zend_read_property(zend_get_class_entry(this), this,
			"mediatorName", strlen("mediatorName"), 1 TSRMLS_CC);

	RETURN_STRINGL(Z_STRVAL_P(mediatorName), Z_STRLEN_P(mediatorName), 1);
}
/* }}} */ PHP_METHOD(Mediator, getViewComponent)
{
	zval *this, *viewComponent;
	this = getThis();
	viewComponent = zend_read_property(zend_get_class_entry(this), this,
						"viewComponent", strlen("viewComponent"), 1 TSRMLS_CC);

	RETVAL_ZVAL(viewComponent, 0, 0);
}
/* }}} */
PHP_METHOD(Mediator, setViewComponent)
{
	zval *viewComponent, *this;

	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &viewComponent)
			== FAILURE ) {
		return;
	}

	this = getThis();

	zend_update_property(zend_get_class_entry(this), this, "viewComponent",
			strlen("viewComponent"), viewComponent TSRMLS_CC);
}
/* }}} */
PHP_METHOD(Mediator, listNotificationInterests)
{
	zval *emptyArray;

	MAKE_STD_ZVAL(emptyArray);
	array_init(emptyArray);

	RETURN_ZVAL(emptyArray, 0, 0);
}
/* }}} */
PHP_METHOD(Mediator, handleNotification)
{
	/* intentionally left blank */
}
/* }}} */
PHP_METHOD(Mediator, onRegister)
{
	return;
}
/* }}} */
PHP_METHOD(Mediator, onRemove)
{
	return;
}
/* }}} */
/* Notification */
/* {{{ proto public void __construct(string name [, string body [, string type ]])
		constructor */
PHP_METHOD(Notification, __construct)
{
	zval *this, *name, *body, *type;
	char *rawName = NULL, *rawBody = NULL, *rawType = NULL;
	int rawNameLength, rawBodyLength, rawTypeLength;

	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|ss",
			&rawName, &rawNameLength, &rawBody, &rawBodyLength,
			&rawType, &rawTypeLength) == FAILURE) {
		return;
	}

	this = getThis();

	MAKE_STD_ZVAL(name);
	ZVAL_STRINGL(name, rawName, rawNameLength, 1);
	ZVAL_ADDREF(name);
	zend_update_property(puremvc_notification_ce, this,
		"name", sizeof("name")-1, name TSRMLS_CC);
	if(rawBody != NULL) {
		MAKE_STD_ZVAL(body);
		ZVAL_STRINGL(body, rawBody, rawBodyLength, 1);
		zend_update_property(puremvc_notification_ce, this,
			"body", sizeof("body")-1, body TSRMLS_CC);
	}
	if(rawType != NULL) {
		MAKE_STD_ZVAL(type);
		ZVAL_STRINGL(type, rawType, rawTypeLength, 1);
		zend_update_property(puremvc_notification_ce, this,
			"type", sizeof("type")-1, type TSRMLS_CC);
	}
}
/* }}} */
/* {{{ proto public string Notification::getName
		name getter method */
PHP_METHOD(Notification, getName)
{
	zval *this, *name;
	this = getThis();
	name = zend_read_property(zend_get_class_entry(this), this,
		"name", strlen("name"), 1 TSRMLS_CC);
	RETVAL_STRINGL(Z_STRVAL_P(name), Z_STRLEN_P(name), 1);
}
/* }}} */
/* {{{ proto public string Notification::getBody()
	body getter method */
PHP_METHOD(Notification, getBody)
{
	zval *body;

	body = zend_read_property(puremvc_notification_ce, getThis(),
		"body", sizeof("body")-1, 1 TSRMLS_CC);

	RETVAL_STRINGL(Z_STRVAL_P(body), Z_STRLEN_P(body), 1);
}
/* }}} */
/* {{{ proto public string Notification::getType()
		type getter method */
PHP_METHOD(Notification, getType)
{
	zval *this, *type;

	this = getThis();
	type = zend_read_property(puremvc_notification_ce, this,
		"type", sizeof("type")-1, 1 TSRMLS_CC);
	RETVAL_STRINGL(Z_STRVAL_P(type), Z_STRLEN_P(type), 1);
}
/* }}} */
/* {{{ proto public void Notification::setBody(string body)
	body setter method */
PHP_METHOD(Notification, setBody)
{
	zval *body;
	char *rawBody;
	int rawBodyLength;

	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
			&rawBody, &rawBodyLength) == FAILURE) {
		return;
	}

	MAKE_STD_ZVAL(body);
	ZVAL_STRINGL(body, rawBody, rawBodyLength, 1);
	ZVAL_ADDREF(body);

	zend_update_property(puremvc_notification_ce, getThis(),
		"body", sizeof("body")-1, body TSRMLS_CC);
}
/* }}} */
/* {{{ proto public void Notification::setType(string type)
		type setter method */
PHP_METHOD(Notification, setType)
{
	zval *type;
	char *rawType;
	int rawTypeLength;

	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
			&rawType, &rawTypeLength) == FAILURE) {
		return;
	}

	MAKE_STD_ZVAL(type);
	ZVAL_STRINGL(type, rawType, rawTypeLength, 1);
	ZVAL_ADDREF(type);

	zend_update_property(puremvc_notification_ce, getThis(),
		"type", sizeof("type")-1, type TSRMLS_CC);
}
/* }}} */
/* {{{ proto public string Notification::toString()
		explicit toString() method */
PHP_METHOD(Notification, toString)
{
	zval *this, *buffer;
	zend_class_entry *this_ce;
	smart_str stringVal = {0};

	this = getThis();
	this_ce = zend_get_class_entry(this); 
	/* append the name */
	smart_str_appends(&stringVal, "Notification Name:");
	buffer = zend_read_property(this_ce, this,
			"name", sizeof("name")-1, 1 TSRMLS_CC);
	smart_str_appends(&stringVal, Z_STRVAL_P(buffer));
	/* append the body */
	smart_str_appends(&stringVal, "\nBody:");
	buffer = zend_read_property(this_ce, this,
			"body", sizeof("body")-1, 1 TSRMLS_CC);
	if( buffer == NULL ||
		(Z_TYPE_P(buffer) == IS_STRING && Z_STRLEN_P(buffer) == 0) )
		smart_str_appends(&stringVal, "null");
	else
		smart_str_appends(&stringVal, Z_STRVAL_P(buffer));
	/* append the type */
	smart_str_appends(&stringVal, "\nType:");
	buffer = zend_read_property(this_ce, this,
			"type", sizeof("type")-1, 1 TSRMLS_CC);
	if( buffer == NULL ||
		(Z_TYPE_P(buffer) == IS_STRING && Z_STRLEN_P(buffer) == 0) )
		smart_str_appends(&stringVal, "null");
	else
		smart_str_appends(&stringVal, Z_STRVAL_P(buffer));

	RETVAL_STRINGL(stringVal.c, stringVal.len, 1);
}
/* }}} */
/* Notifier */
/* {{{ proto public final void Facade::__construct
	Notifier constructor, calls Facade::getInstance
    */ 
PHP_METHOD(Notifier, __construct)
{
	puremvc_log_func_io("SimpleCommand", "__construct", 1);

	zend_call_method_with_0_params(NULL, puremvc_facade_ce, NULL,
			"getinstance", &return_value);
	zend_update_property(puremvc_notifier_ce, getThis(), "facade", sizeof("facade")-1,
			return_value);

	puremvc_log_func_io("SimpleCommand", "__construct", 0);
}
/* }}} */
/* proto public void Notifier::sendNotification(string notificationNamea [, string body [, string type ]])
			sendin a notification via the facade */
PHP_METHOD(Notifier, sendNotification)
{
	zval *this, *notificationName, *body, *type, *facade;
	char *rawNotificationName, *rawBody, *rawType;
	int rawNotificationNameLength, rawBodyLength, rawTypeLength;
	int numParams = ZEND_NUM_ARGS();

	if(zend_parse_parameters(numParams TSRMLS_CC, "s|ss",
			&rawNotificationName, &rawNotificationNameLength,
			&rawBody, &rawBodyLength, &rawType, &rawTypeLength) == FAILURE) {
		return;
	}

	MAKE_STD_ZVAL(notificationName);
	ZVAL_STRINGL(notificationName, rawNotificationName,
			rawNotificationNameLength, 1);

	MAKE_STD_ZVAL(body);
	if(numParams == 2 || numParams == 3) {
		ZVAL_STRINGL(body, rawBody, rawBodyLength, 1);
	} else {
		ZVAL_NULL(body);
	}

	MAKE_STD_ZVAL(type);
	if(numParams == 3) {
		ZVAL_STRINGL(type, rawType, rawTypeLength, 1);
	} else {
		ZVAL_NULL(type);
	}

	this = getThis();
	facade = zend_read_property(zend_get_class_entry(this), this,
				"facade", sizeof("facade")-1, 1 TSRMLS_CC);

	puremvc_call_method_with_3_params(&facade, zend_get_class_entry(facade),
			NULL, "sendnotification", NULL, notificationName, body, type);
}
/* }}} */
/* Observer */
/* {{{ proto public void Observer::__construct(string notifyMethod, string notifyContext)
		constructor */
PHP_METHOD(Observer, __construct)
{
	zval *this, *notifyMethod, *notifyContext;
	zend_class_entry *this_ce;
	char *rawNotifyMethod;
	int rawNotifyMethodLength;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "so",
		&rawNotifyMethod, &rawNotifyMethodLength,
		&notifyContext) == FAILURE) {
		return;
	}

	MAKE_STD_ZVAL(notifyMethod);
	ZVAL_STRINGL(notifyMethod, rawNotifyMethod, rawNotifyMethodLength, 1);

	this = getThis();
	this_ce = zend_get_class_entry(this);
	zend_call_method_with_1_params(&this, this_ce, NULL,
		"setnotifymethod", NULL, notifyMethod);
	zend_call_method_with_1_params(&this, this_ce, NULL,
		"setnotifycontext", NULL, notifyContext);
}
/* }}} */
/* {{{ proto public void Observer::setNotifyMethod(string notifyMethod)
		set the notify method for the instance */
PHP_METHOD(Observer, setNotifyMethod)
{
	zval *notifyMethod;
	char *rawNotifyMethod;
	int rawNotifyMethodLength;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
		&rawNotifyMethod, &rawNotifyMethodLength) == FAILURE) {
		return;
	}

	MAKE_STD_ZVAL(notifyMethod);
	ZVAL_STRINGL(notifyMethod, rawNotifyMethod, rawNotifyMethodLength, 1);
	ZVAL_ADDREF(notifyMethod);

	zend_update_property(puremvc_observer_ce, getThis(),
		"notify", sizeof("notify")-1, notifyMethod TSRMLS_CC);
}
/* }}} */
/* {{{ proto public void Observer::setNotifyContext(string notifyContext)
		set the notify method for the instance */
PHP_METHOD(Observer, setNotifyContext)
{
	zval *notifyContext, *this;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o",
		&notifyContext) == FAILURE) {
		return;
	}

	this = getThis();
	zend_update_property(puremvc_observer_ce, this,
		"context", sizeof("context")-1, notifyContext TSRMLS_CC);
}
/* }}} */
/* {{{ proto string Observer::getNotifyMethod()
		fetch the notify method for the instance */
PHP_METHOD(Observer, getNotifyMethod)
{
	zval *this, *notifyMethod;

	this = getThis();
	notifyMethod = zend_read_property(puremvc_observer_ce, this,
					"notify", sizeof("notify")-1, 1 TSRMLS_CC);

	RETVAL_STRINGL(Z_STRVAL_P(notifyMethod), Z_STRLEN_P(notifyMethod), 1);
}
/* }}} */
/* {{{ proto string Observer::getNotifyContext()
		fetch the notify method for the instance */
PHP_METHOD(Observer, getNotifyContext)
{
	zval *this, *notifyContext;

	this = getThis();
	notifyContext = zend_read_property(puremvc_observer_ce, this,
					"context", sizeof("context")-1, 1 TSRMLS_CC);

	RETVAL_OBJECT(notifyContext, 1);
}
/* }}} */
/* {{{ proto public void Observer:;notifyObserver(object notification)
		notify this observer with the given INotification */
PHP_METHOD(Observer, notifyObserver)
{
	zval *this, *notifyMethod, *notifyContext, *context, *notification;
	zend_class_entry *this_ce;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o",
			&notification) == FAILURE) {
		return;
	}

	this = getThis();
	this_ce = zend_get_class_entry(this);

	zend_call_method_with_0_params(&this, this_ce, NULL,
			"getnotifymethod", &notifyMethod);
	zend_call_method_with_0_params(&this, this_ce, NULL,
			"getnotifycontext", &notifyContext);

	zend_call_method_with_1_params(&context, zend_get_class_entry(context), NULL,
			Z_STRVAL_P(notifyMethod), NULL, notification);
}
/* }}} */
/* {{{ proto public bool Observer::compareByContext(object object)
			compare some arbitrary context object to this context */
PHP_METHOD(Observer, compareNotifyContext)
{
	zval *this, *object, *context;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o",
			&object) == FAILURE) {
		return;
	}

	context = zend_read_property(zend_get_class_entry(this), this,
					"context", sizeof("context")-1, 1 TSRMLS_CC);

	if(Z_OBJHANDLE_P(context) == Z_OBJHANDLE_P(object)) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
}
/* }}} */
/* Proxy */
PHP_METHOD(Proxy, __construct)
{
}
/* }}} */
PHP_METHOD(Proxy, getProxyName)
{
}
/* }}} */
PHP_METHOD(Proxy, setData)
{
}
/* }}} */
PHP_METHOD(Proxy, getData)
{
}
/* }}} */
PHP_METHOD(Proxy, onRegister)
{
}
/* }}} */
PHP_METHOD(Proxy, onRemove)
{
}
/* }}} */
/* ICommand */
static
ZEND_BEGIN_ARG_INFO(arginfo_command_execute, 0)
//// @TODO does ZEND_ARG_OBJ_INFO not support interfaces / subclasses ?
////	ZEND_ARG_OBJ_INFO(0, "notification", "INotification", 0)
	ZEND_ARG_INFO(0, notification)
ZEND_END_ARG_INFO();
static
function_entry puremvc_command_iface_methods [] = {
	PHP_ABSTRACT_ME(ICommand, execute, arginfo_command_execute)
	{ NULL, NULL, NULL }
};
/* IController */
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_icontroller_registerCommand, 0, 0, 2)
	ZEND_ARG_INFO(0, notificationName)
	ZEND_ARG_INFO(0, commandClassRef)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_icontroller_executeCommand, 0, 0, 1)
	ZEND_ARG_INFO(0, "notification")
ZEND_END_ARG_INFO();
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_icontroller_removeCommand, 0, 0, 1)
	ZEND_ARG_INFO(0, notificationName)
ZEND_END_ARG_INFO()
static
function_entry puremvc_controller_iface_methods[] = {
	PHP_ABSTRACT_ME(IController, registerCommand, arginfo_icontroller_registerCommand)
	PHP_ABSTRACT_ME(IController, executeCommand, arginfo_icontroller_executeCommand)
	PHP_ABSTRACT_ME(IController, removeCommand, arginfo_icontroller_removeCommand)
	{ NULL, NULL, NULL }
};
/* IFacade */
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_ifacade_retrieveProxy, 0, 0, 1)
	ZEND_ARG_INFO(0, proxyName)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_ifacade_hasProxy, 0, 0, 1)
	ZEND_ARG_INFO(0, proxyName)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_ifacade_removeProxy, 0, 0, 1)
	ZEND_ARG_INFO(0, proxyName)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_ifacade_registerProxy, 0, 0, 1)
	ZEND_ARG_INFO(0, proxy)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_ifacade_registerCommand, 0, 0, 2)
	ZEND_ARG_INFO(0, noteName)
	ZEND_ARG_INFO(0, commandClassRef)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_ifacade_notifyObservers, 0, 0, 1)
	ZEND_ARG_INFO(0, note)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_ifacade_registerMediator, 0, 0, 1)
	ZEND_ARG_INFO(0, mediator)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_ifacade_retrieveMediator, 0, 0, 1)
	ZEND_ARG_INFO(0, mediatorName)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_ifacade_hasMediator, 0, 0, 1)
	ZEND_ARG_INFO(0, mediatorName)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_ifacade_removeMediator, 0, 0, 1)
	ZEND_ARG_INFO(0, mediatorName)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_ifacade_sendNotification, 0, 0, 3)
	ZEND_ARG_INFO(0, notificationName)
	ZEND_ARG_INFO(0, body)
	ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO()
static function_entry puremvc_facade_iface_methods[] = {
	PHP_ABSTRACT_ME(IFacade, registerProxy, arginfo_ifacade_registerProxy)
	PHP_ABSTRACT_ME(IFacade, retrieveProxy, arginfo_ifacade_retrieveProxy)
	PHP_ABSTRACT_ME(IFacade, hasProxy, arginfo_ifacade_hasProxy)
	PHP_ABSTRACT_ME(IFacade, removeProxy, arginfo_ifacade_removeProxy)
	PHP_ABSTRACT_ME(IFacade, registerCommand, arginfo_ifacade_registerCommand)
	PHP_ABSTRACT_ME(IFacade, notifyObservers, arginfo_ifacade_notifyObservers)
	PHP_ABSTRACT_ME(IFacade, registerMediator, arginfo_ifacade_registerMediator)
	PHP_ABSTRACT_ME(IFacade, retrieveMediator, arginfo_ifacade_retrieveMediator)
	PHP_ABSTRACT_ME(IFacade, hasMediator, arginfo_ifacade_hasMediator)
	PHP_ABSTRACT_ME(IFacade, removeMediator, arginfo_ifacade_removeMediator)
	PHP_ABSTRACT_ME(IFacade, sendNotification, arginfo_ifacade_sendNotification)
	{ NULL, NULL, NULL }
};
/* IMediator */
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_imediator_setViewComponent, 0, 0, 1)
	ZEND_ARG_INFO(0, viewComponent)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_imediator_handleNotification, 0, 0, 1)
	ZEND_ARG_INFO(0, notification)
ZEND_END_ARG_INFO()
static function_entry puremvc_mediator_iface_methods[] = {
	PHP_ABSTRACT_ME(IMediator, getMediatorName, NULL)
	PHP_ABSTRACT_ME(IMediator, getViewComponent, NULL)
	PHP_ABSTRACT_ME(IMediator, setViewComponent, arginfo_imediator_setViewComponent)
	PHP_ABSTRACT_ME(IMediator, listNotificationInterests, NULL)
	PHP_ABSTRACT_ME(IMediator, handleNotification, arginfo_imediator_handleNotification)
	PHP_ABSTRACT_ME(IMediator, onRegister, NULL)
	PHP_ABSTRACT_ME(IMediator, onRemove, NULL)
	{ NULL, NULL, NULL }
};
/* IMdoel */
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_imodel_registerProxy, 0, 0, 1)
	ZEND_ARG_INFO(0, proxy)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_imodel_retrieveProxy, 0, 0, 1)
	ZEND_ARG_INFO(0, proxyName)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_imodel_removeProxy, 0, 0, 1)
	ZEND_ARG_INFO(0, proxyName)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_imodel_hasProxy, 0, 0, 1)
	ZEND_ARG_INFO(0, hasProxy)
ZEND_END_ARG_INFO()
static function_entry puremvc_model_iface_methods[] = {
	PHP_ABSTRACT_ME(IModel, registerProxy, arginfo_imodel_registerProxy)
	PHP_ABSTRACT_ME(IModel, retrieveProxy, arginfo_imodel_retrieveProxy)
	PHP_ABSTRACT_ME(IModel, removeProxy, arginfo_imodel_removeProxy)
	PHP_ABSTRACT_ME(IModel, hasProxy, arginfo_imodel_hasProxy)
	{ NULL, NULL, NULL }
};
/* INotification */
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_inotification_setBody, 0, 0, 1)
	ZEND_ARG_INFO(0, body)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_inotification_setType, 0, 0, 1)
	ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO()
static function_entry puremvc_notification_iface_methods[] = {
	PHP_ABSTRACT_ME(INotification, getName, NULL)
	PHP_ABSTRACT_ME(INotification, setBody, arginfo_inotification_setBody)
	PHP_ABSTRACT_ME(INotification, getBody, NULL)
	PHP_ABSTRACT_ME(INotification, setType, arginfo_inotification_setType)
	PHP_ABSTRACT_ME(INotification, getType, NULL)
	PHP_ABSTRACT_ME(INotification, toString, NULL)
	{ NULL, NULL, NULL }
};
/* INotifier */
static
ZEND_BEGIN_ARG_INFO(arginfo_inotifier_sendNotification, 0)
	ZEND_ARG_INFO(0, notificationName)
	ZEND_ARG_INFO(0, body)
	ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO()
static function_entry puremvc_notifier_iface_methods[] = {
	PHP_ABSTRACT_ME(INotifier, sendNotification, arginfo_inotifier_sendNotification)
	{ NULL, NULL, NULL }
};
/* IObserver */
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_iobserver_setNotifyMethod, 0, 0, 1)
	ZEND_ARG_INFO(0, setNotifyMethod)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_iobserver_setNotifyContext, 0, 0, 1)
	ZEND_ARG_INFO(0, notifyContent)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_iobserver_notifyObserver, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, "notification", "INotification", 0)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_iobserver_compareNotifyContent, 0, 0, 1)
	ZEND_ARG_INFO(0, object)
ZEND_END_ARG_INFO()
static function_entry puremvc_observer_iface_methods[] = {
	PHP_ABSTRACT_ME(IObserver, setNotifyMethod, arginfo_iobserver_setNotifyMethod)
	PHP_ABSTRACT_ME(IObserver, setNotifyContext, arginfo_iobserver_setNotifyContext)
	PHP_ABSTRACT_ME(IObserver, notifyObserver, arginfo_iobserver_notifyObserver)
	PHP_ABSTRACT_ME(IObserver, compareNotifyContext, arginfo_iobserver_compareNotifyContent)
	{ NULL, NULL, NULL }
};
/* IProxy */
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_iproxy_setData, 0, 0, 1)
	ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()
static function_entry puremvc_proxy_iface_methods[] = {
	PHP_ABSTRACT_ME(IProxy, getProxyName, NULL)
	PHP_ABSTRACT_ME(IProxy, getData, NULL)
	PHP_ABSTRACT_ME(IProxy, setData, arginfo_iproxy_setData)
	PHP_ABSTRACT_ME(IProxy, onRegister, NULL)
	PHP_ABSTRACT_ME(IProxy, onRemove, NULL)
	{ NULL, NULL, NULL }
};
/* IView */
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_iview_notifyObservers, 0, 0, 1)
	ZEND_ARG_INFO(0, note)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_iview_registerMediator, 0, 0, 1)
	ZEND_ARG_INFO(0, mediator)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_iview_retrieveMediator, 0, 0, 1)
	ZEND_ARG_INFO(0, "mediatorName")
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_iview_removeMediator, 0, 0, 1)
	ZEND_ARG_INFO(0, "mediatorName")
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_iview_hasMediator, 0, 0, 1)
	ZEND_ARG_INFO(0, "mediatorName")
ZEND_END_ARG_INFO()
static function_entry puremvc_view_iface_methods[] = {
	PHP_ABSTRACT_ME(IView, notifyObservers, arginfo_iview_notifyObservers)
	PHP_ABSTRACT_ME(IView, registerMediator, arginfo_iview_registerMediator)
	PHP_ABSTRACT_ME(IView, retrieveMediator, arginfo_iview_retrieveMediator)
	PHP_ABSTRACT_ME(IView, removeMediator, arginfo_iview_removeMediator)
	PHP_ABSTRACT_ME(IView, hasMediator, arginfo_iview_hasMediator)
	{ NULL, NULL, NULL }
};

/* Controller */
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_controller_executeCommand, 0, 0, 1)
	ZEND_ARG_INFO(0, "note")
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_controller_registerCommand, 0, 0, 2)
	ZEND_ARG_INFO(0, notificationName)
	ZEND_ARG_INFO(0, commandClassRef)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_controller_hasCommand, 0, 0, 1)
	ZEND_ARG_INFO(0, commandName)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_controller_removeCommand, 0, 0, 1)
	ZEND_ARG_INFO(0, commandName)
ZEND_END_ARG_INFO()
static function_entry puremvc_controller_class_methods[] = {
	PHP_ME(Controller, __construct, NULL, ZEND_ACC_CTOR|ZEND_ACC_PRIVATE)
	PHP_ME(Controller, initializeController, NULL, ZEND_ACC_PROTECTED)
	PHP_ME(Controller, getInstance, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(Controller, executeCommand, arginfo_controller_executeCommand, ZEND_ACC_PUBLIC)
	PHP_ME(Controller, registerCommand, arginfo_controller_registerCommand, ZEND_ACC_PUBLIC)
	PHP_ME(Controller, hasCommand, arginfo_controller_hasCommand, ZEND_ACC_PUBLIC)
	PHP_ME(Controller, removeCommand, arginfo_controller_removeCommand, ZEND_ACC_PUBLIC)
	{ NULL, NULL, NULL }
};
void puremvc_controller_addprops(zend_class_entry *controller_ce TSRMLS_DC)
{
	/* proptected */
	zend_declare_property_null(controller_ce, "instance", sizeof("instance")-1,
				ZEND_ACC_PROTECTED | ZEND_ACC_STATIC TSRMLS_CC);
	zend_declare_property_null(controller_ce, "view", sizeof("view")-1,
				ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(controller_ce, "commandMap", sizeof("commandMap")-1,
				ZEND_ACC_PROTECTED TSRMLS_CC);
}
/* Model */
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_model_class_registerProxy, 0, 0, 1)
	ZEND_ARG_INFO(0, proxy)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_model_class_retrieveProxy, 0, 0, 1)
	ZEND_ARG_INFO(0, proxyName)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_model_class_removeProxy, 0, 0, 1)
	ZEND_ARG_INFO(0, proxyName)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_model_class_hasProxy, 0, 0, 1)
	ZEND_ARG_INFO(0, proxyName)
ZEND_END_ARG_INFO()
static function_entry puremvc_model_class_methods[] = {
	PHP_ME(Model, __construct, NULL, ZEND_ACC_CTOR|ZEND_ACC_PRIVATE)
	PHP_ME(Model, initializeModel, NULL, ZEND_ACC_PROTECTED)
	PHP_ME(Model, getInstance, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(Model, registerProxy, arginfo_model_class_registerProxy, ZEND_ACC_PUBLIC)
	PHP_ME(Model, retrieveProxy, arginfo_model_class_retrieveProxy, ZEND_ACC_PUBLIC)
	PHP_ME(Model, removeProxy, arginfo_model_class_removeProxy, ZEND_ACC_PUBLIC)
	PHP_ME(Model, hasProxy, arginfo_model_class_hasProxy, ZEND_ACC_PUBLIC)
	{ NULL, NULL, NULL }
};

void puremvc_model_addprops(zend_class_entry *model_ce TSRMLS_DC)
{
	/* proptected */
	zend_declare_property_null(model_ce, "instance", sizeof("instance")-1,
			ZEND_ACC_PROTECTED | ZEND_ACC_STATIC TSRMLS_CC);
	zend_declare_property_null(model_ce, "proxyMap", sizeof("proxyMap")-1,
			 ZEND_ACC_PROTECTED TSRMLS_CC);
}
/* View */
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_view_registerObserver, 0, 0, 2)
//// @TODO does ZEND_ARG_OBJ_INFO not support interfaces / subclasses ?
////	ZEND_ARG_OBJ_INFO(0, "notification", "INotification", 0)
	ZEND_ARG_INFO(0, notificationName)
	//ZEND_ARG_OBJ_INFO(0, "observer", "IObserver", 0)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_view_notifyObservers, 0, 0 , 1)
	ZEND_ARG_INFO(0, notefication)
ZEND_END_ARG_INFO();
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_view_registerMediator, 0, 0, 1)
	ZEND_ARG_INFO(0, mediator)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_view_retrieveMediator, 0, 0, 1)
	ZEND_ARG_INFO(0, mediatorName)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_view_hasMediator, 0, 0, 1)
	ZEND_ARG_INFO(0, mediatorName)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_view_removeMediator, 0, 0, 1)
	ZEND_ARG_INFO(0, mediaName)
ZEND_END_ARG_INFO()
static function_entry puremvc_view_class_methods[] = {
	PHP_ME(View, __construct, NULL, ZEND_ACC_CTOR|ZEND_ACC_PRIVATE)
	PHP_ME(View, initializeView, NULL, ZEND_ACC_PROTECTED)
	PHP_ME(View, getInstance, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(View, registerObserver, arginfo_view_registerObserver, ZEND_ACC_PUBLIC)
	PHP_ME(View, notifyObservers, arginfo_view_notifyObservers, ZEND_ACC_PUBLIC)
	PHP_ME(View, registerMediator, arginfo_view_registerMediator, ZEND_ACC_PUBLIC)
	PHP_ME(View, retrieveMediator, arginfo_view_retrieveMediator, ZEND_ACC_PUBLIC)
	PHP_ME(View, hasMediator, arginfo_view_hasMediator, ZEND_ACC_PUBLIC)
	PHP_ME(View, removeMediator, arginfo_view_removeMediator, ZEND_ACC_PUBLIC)
	{ NULL, NULL, NULL }
};
void puremvc_view_addprops(zend_class_entry *view_ce TSRMLS_DC)
{
	/* proptected */
	zend_declare_property_null(view_ce, "instance", sizeof("instance")-1,
				ZEND_ACC_PROTECTED | ZEND_ACC_STATIC TSRMLS_CC);
	zend_declare_property_null(view_ce, "mediatorMap", sizeof("mediatorMap")-1,
				ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(view_ce, "observerMap", sizeof("observerMap")-1,
				ZEND_ACC_PROTECTED TSRMLS_CC);
}
/* MacroCommand */
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_macrocommand_addSubCommand, 0, 0, 1)
	ZEND_ARG_INFO(0, commandClassRef)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_macrocommand_execute, 0, 0, 1)
//// @TODO does ZEND_ARG_OBJ_INFO not support interfaces / subclasses ?
////	ZEND_ARG_OBJ_INFO(0, "notification", "INotification", 0)
	ZEND_ARG_INFO(0, notification)
ZEND_END_ARG_INFO()
static function_entry puremvc_macrocommand_class_methods[] = {
	PHP_ME(MacroCommand, __construct, NULL, ZEND_ACC_CTOR|ZEND_ACC_PUBLIC)
	PHP_ME(MacroCommand, initializeMacroCommand, NULL, ZEND_ACC_PROTECTED)
	PHP_ME(MacroCommand, addSubCommand, arginfo_macrocommand_addSubCommand, ZEND_ACC_PROTECTED)
	//PHP_ME(MacroCommand, execute, arginfo_macrocommand_execute, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(MacroCommand, execute, arginfo_macrocommand_execute, ZEND_ACC_PUBLIC )
	{ NULL, NULL, NULL }
};
void puremvc_macrocommand_addprops(zend_class_entry *macrocommand_ce TSRMLS_DC)
{
	// private /
	zend_declare_property_null(macrocommand_ce, "subCommands", sizeof("subCommands")-1,
			ZEND_ACC_PRIVATE TSRMLS_CC);
}
/* SimpleCommand */
ZEND_BEGIN_ARG_INFO_EX(arginfo_simplecommand_execute, 0, 0, 1)
//// @TODO does ZEND_ARG_OBJ_INFO not support interfaces / subclasses ?
////	ZEND_ARG_OBJ_INFO(0, "notification", "INotification", 0)
	ZEND_ARG_INFO(0, notification)
ZEND_END_ARG_INFO()
static function_entry puremvc_simplecommand_class_methods[] = {
	PHP_ME(SimpleCommand, __construct, NULL, ZEND_ACC_CTOR|ZEND_ACC_PUBLIC)
	PHP_ME(SimpleCommand, execute, arginfo_simplecommand_execute, ZEND_ACC_PUBLIC)
	{ NULL, NULL, NULL }
};
void puremvc_simplecommand_addprops(zend_class_entry *simplecommand_ce TSRMLS_DC)
{
	// protected /
	zend_declare_property_null(simplecommand_ce, "facade", sizeof("facade")-1,
				ZEND_ACC_PROTECTED TSRMLS_CC);
}
/* Facade */
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_facade_notifyObservers, 0, 0, 1)
	ZEND_ARG_INFO(0, notification)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_facade_registerCommand, 0, 0, 2)
	ZEND_ARG_INFO(0, noteName)
	ZEND_ARG_INFO(0, commandClassRef)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_facade_removeCommand, 0, 0, 1)
	ZEND_ARG_INFO(0, notificationName)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_facade_hasCommand, 0, 0, 1)
	ZEND_ARG_INFO(0, notificationName)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_facade_registerProxy, 0, 0, 1)
	ZEND_ARG_INFO(0, proxy)
ZEND_END_ARG_INFO()
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
ZEND_BEGIN_ARG_INFO_EX(arginfo_facade_registerMediator, 0, 0, 1)
	ZEND_ARG_INFO(0, mediator)
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
ZEND_BEGIN_ARG_INFO(arginfo_facade_sendNotification, 0)
	ZEND_ARG_INFO(0, notificationName)
	ZEND_ARG_INFO(0, body)
	ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO()
static function_entry puremvc_facade_class_methods[] = {
	PHP_ME(Facade, __construct, NULL, ZEND_ACC_CTOR|ZEND_ACC_PROTECTED)
	PHP_ME(Facade, initializeFacade, NULL, ZEND_ACC_PROTECTED)
	PHP_ME(Facade, getInstance, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(Facade, initializeController, NULL, ZEND_ACC_PROTECTED)
	PHP_ME(Facade, initializeModel, NULL, ZEND_ACC_PROTECTED)
	PHP_ME(Facade, initializeView, NULL, ZEND_ACC_PROTECTED)
	PHP_ME(Facade, notifyObservers, arginfo_facade_notifyObservers, ZEND_ACC_PUBLIC)
	PHP_ME(Facade, registerCommand, arginfo_facade_registerCommand, ZEND_ACC_PUBLIC)
	PHP_ME(Facade, removeCommand, arginfo_facade_removeCommand, ZEND_ACC_PUBLIC)
	PHP_ME(Facade, hasCommand, arginfo_facade_hasCommand, ZEND_ACC_PUBLIC)
	PHP_ME(Facade, registerProxy, arginfo_facade_registerProxy, ZEND_ACC_PUBLIC)
	PHP_ME(Facade, retrieveProxy, arginfo_facade_retrieveProxy, ZEND_ACC_PUBLIC)
	PHP_ME(Facade, hasProxy, arginfo_facade_hasProxy, ZEND_ACC_PUBLIC)
	PHP_ME(Facade, removeProxy, arginfo_facade_removeProxy, ZEND_ACC_PUBLIC)
	PHP_ME(Facade, registerMediator, arginfo_facade_registerMediator, ZEND_ACC_PUBLIC)
	PHP_ME(Facade, retrieveMediator, arginfo_facade_retrieveMediator, ZEND_ACC_PUBLIC)
	PHP_ME(Facade, hasMediator, arginfo_facade_hasMediator, ZEND_ACC_PUBLIC)
	PHP_ME(Facade, removeMediator, arginfo_facade_removeMediator, ZEND_ACC_PUBLIC)
	PHP_ME(Facade, sendNotification, arginfo_facade_sendNotification, ZEND_ACC_PUBLIC)
	{ NULL, NULL, NULL }
};
void puremvc_facade_addprops(zend_class_entry *facade_ce TSRMLS_DC)
{
	// protected /
	zend_declare_property_null(facade_ce, "model", sizeof("model")-1,
				ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(facade_ce, "view", sizeof("view")-1,
				ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(facade_ce, "controller", sizeof("controller")-1,
				ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(facade_ce, "instance", sizeof("instance")-1,
				ZEND_ACC_PROTECTED | ZEND_ACC_STATIC TSRMLS_CC);
}
/* Mediator */
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_mediator_setViewComponent, 0, 0, 1)
	ZEND_ARG_INFO(0, component)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_mediator_handleNotification, 0, 0, 1)
	ZEND_ARG_INFO(0, notification)
ZEND_END_ARG_INFO()

static function_entry puremvc_mediator_class_methods[] = {
	PHP_ME(Mediator, __construct, NULL, ZEND_ACC_CTOR|ZEND_ACC_PUBLIC)
	PHP_ME(Mediator, getMediatorName, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Mediator, getViewComponent, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Mediator, setViewComponent, arginfo_mediator_setViewComponent, ZEND_ACC_PUBLIC)
	PHP_ME(Mediator, listNotificationInterests, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Mediator, handleNotification, arginfo_mediator_handleNotification, ZEND_ACC_PUBLIC)
	PHP_ME(Mediator, onRegister, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Mediator, onRemove, NULL, ZEND_ACC_PUBLIC)
	{ NULL, NULL, NULL }
};
void puremvc_mediator_addprops(zend_class_entry *mediator_ce TSRMLS_DC)
{
	// proptected /
	zend_declare_property_string(mediator_ce, "mediatorName", sizeof("mediatorName")-1,
				"", ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(mediator_ce, "viewComponent", sizeof("viewComponent")-1,
				ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(mediator_ce, "facade", sizeof("facade")-1,
				ZEND_ACC_PROTECTED TSRMLS_CC);
}
void puremvc_mediator_addconstants(zend_class_entry *ce)
{
	zval *constval;
	// const /
	constval = pemalloc(sizeof(zval), 1);
	INIT_PZVAL(constval);
	Z_TYPE_P(constval) = IS_STRING;
	Z_STRLEN_P(constval) = sizeof("Mediator") - 1;
	Z_STRVAL_P(constval) = pemalloc(Z_STRLEN_P(constval)+1, 1);
	memcpy(Z_STRVAL_P(constval), "Mediator",
						Z_STRLEN_P(constval) + 1);
	zend_hash_add(&ce->constants_table, 
						"NAME", sizeof("NAME"),
						(void*)&constval, sizeof(zval*), NULL);
}
/* Notification */
static
ZEND_BEGIN_ARG_INFO(arginfo_notification__construct, 0)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, body)
	ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_notification_setBody, 0, 0, 1)
	ZEND_ARG_INFO(0, body)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_notification_setType, 0, 0, 1)
	ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO()
static function_entry puremvc_notification_class_methods[] = {
	PHP_ME(Notification, __construct, arginfo_notification__construct, ZEND_ACC_CTOR|ZEND_ACC_PUBLIC)
	PHP_ME(Notification, getName, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Notification, setBody, arginfo_notification_setBody, ZEND_ACC_PUBLIC)
	PHP_ME(Notification, getBody, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Notification, setType, arginfo_notification_setType, ZEND_ACC_PUBLIC)
	PHP_ME(Notification, getType, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Notification, toString, NULL, ZEND_ACC_PUBLIC)
	{ NULL, NULL, NULL }
};
void puremvc_notification_addprops(zend_class_entry *notification_ce TSRMLS_DC)
{
	// private /
	zend_declare_property_string(notification_ce, "name", sizeof("name")-1,
				"", ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_string(notification_ce, "type", sizeof("type")-1,
				"", ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_string(notification_ce, "body", sizeof("body")-1,
				"", ZEND_ACC_PRIVATE TSRMLS_CC);
}
/* Notifier */
static
ZEND_BEGIN_ARG_INFO(arginfo_notifier_sendNotification, 0)
	ZEND_ARG_INFO(0, notificationName)
	ZEND_ARG_INFO(0, body)
	ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO()
static function_entry puremvc_notifier_class_methods[] = {
	PHP_ME(Notifier, __construct, NULL, ZEND_ACC_CTOR|ZEND_ACC_PUBLIC)
	PHP_ME(Notifier, sendNotification, arginfo_notifier_sendNotification, ZEND_ACC_PUBLIC)
	{ NULL, NULL, NULL }
};
void puremvc_notifier_addprops(zend_class_entry *notifier_ce TSRMLS_DC)
{
	// protected /
	zend_declare_property_string(notifier_ce, "facade", sizeof("facade")-1,
			"", ZEND_ACC_PROTECTED TSRMLS_CC);
}
/* Observer */
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_observer___construct, 0, 0, 2)
	ZEND_ARG_INFO(0, notifyMethod)
	ZEND_ARG_INFO(0, notifyContext)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_observer_setNotifyMethod, 0, 0, 1)
	ZEND_ARG_INFO(0, notifyMethod)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_observer_setNotifyContext, 0, 0, 1)
	ZEND_ARG_INFO(0, notifyContext)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_observer_notifyObserver, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, "notification", "INotification", 0)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_observer_compareNotifyContext, 0, 0, 1)
	ZEND_ARG_INFO(0, object)
ZEND_END_ARG_INFO()
static function_entry puremvc_observer_class_methods[] = {
	PHP_ME(Observer, __construct, arginfo_observer___construct, ZEND_ACC_CTOR|ZEND_ACC_PUBLIC)
	PHP_ME(Observer, setNotifyMethod, arginfo_observer_setNotifyMethod, ZEND_ACC_PUBLIC)
	PHP_ME(Observer, setNotifyContext, arginfo_observer_setNotifyContext, ZEND_ACC_PUBLIC)
	PHP_ME(Observer, getNotifyMethod, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Observer, getNotifyContext, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Observer, notifyObserver, arginfo_observer_notifyObserver, ZEND_ACC_PUBLIC)
	PHP_ME(Observer, compareNotifyContext, arginfo_observer_compareNotifyContext, ZEND_ACC_PUBLIC)
	{ NULL, NULL, NULL }
};
void puremvc_observer_addprops(zend_class_entry *observer_ce TSRMLS_DC)
{
	/* private */
	zend_declare_property_string(observer_ce, "notify", sizeof("notify")-1,
				"", ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_string(observer_ce, "context", sizeof("context")-1,
				"", ZEND_ACC_PRIVATE TSRMLS_CC);
}
/* Proxy */
static
ZEND_BEGIN_ARG_INFO(arginfo_proxy___construct, 0)
	ZEND_ARG_INFO(0, proxyName)
	ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_proxy_setData, 0, 0, 1)
	ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()
static function_entry puremvc_proxy_class_methods[] = {
	PHP_ME(Proxy, __construct, arginfo_proxy___construct, ZEND_ACC_CTOR|ZEND_ACC_PUBLIC)
	PHP_ME(Proxy, getProxyName, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Proxy, setData, arginfo_proxy_setData, ZEND_ACC_PUBLIC)
	PHP_ME(Proxy, getData, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Proxy, onRegister, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Proxy, onRemove, NULL, ZEND_ACC_PUBLIC)
	{ NULL, NULL, NULL }
};
void puremvc_proxy_addprops(zend_class_entry *proxy_ce TSRMLS_DC)
{
	/* protected */
	zend_declare_property_string(proxy_ce, "proxyName", sizeof("proxyName")-1,
				"", ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_null(proxy_ce, "context", sizeof("context")-1,
				ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_null(proxy_ce, "facade", sizeof("facade")-1,
				ZEND_ACC_PRIVATE TSRMLS_CC);
}
void puremvc_proxy_addconstants(zend_class_entry *ce)
{
	zval *constval;
	// const /
	constval = pemalloc(sizeof(zval), 1);
	INIT_PZVAL(constval);
	Z_TYPE_P(constval) = IS_STRING;
	Z_STRLEN_P(constval) = sizeof("Proxy") - 1;
	Z_STRVAL_P(constval) = pemalloc(Z_STRLEN_P(constval)+1, 1);
	memcpy(Z_STRVAL_P(constval), "Proxy",
						Z_STRLEN_P(constval) + 1);
	zend_hash_add(&ce->constants_table, 
						"NAME", sizeof("NAME"),
						(void*)&constval, sizeof(zval*), NULL);

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

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(pure_mvc)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	zend_class_entry icommand_ce;
	/* interfaces */
	INIT_CLASS_ENTRY(icommand_ce, "ICommand", puremvc_command_iface_methods);
	puremvc_command_iface_entry = zend_register_internal_class(&icommand_ce TSRMLS_CC);
	puremvc_command_iface_entry->ce_flags|= ZEND_ACC_INTERFACE;

	zend_class_entry icontroller_ce;
	INIT_CLASS_ENTRY(icontroller_ce, "IController", puremvc_controller_iface_methods);
	puremvc_controller_iface_entry = zend_register_internal_class(&icontroller_ce TSRMLS_CC);
	puremvc_controller_iface_entry->ce_flags|= ZEND_ACC_INTERFACE;

	zend_class_entry ifacade_ce;
	INIT_CLASS_ENTRY(ifacade_ce, "IFacade", puremvc_facade_iface_methods);
	puremvc_facade_iface_entry = zend_register_internal_class(&ifacade_ce TSRMLS_CC);
	puremvc_facade_iface_entry->ce_flags|= ZEND_ACC_INTERFACE;

	zend_class_entry imediator_ce;
	INIT_CLASS_ENTRY(imediator_ce, "IMediator", puremvc_mediator_iface_methods);
	puremvc_mediator_iface_entry = zend_register_internal_class(&imediator_ce TSRMLS_CC);
	puremvc_mediator_iface_entry->ce_flags|= ZEND_ACC_INTERFACE;

	zend_class_entry imodel_ce;
	INIT_CLASS_ENTRY(imodel_ce, "IModel", puremvc_model_iface_methods);
	puremvc_model_iface_entry = zend_register_internal_class(&imodel_ce TSRMLS_CC);
	puremvc_model_iface_entry->ce_flags|= ZEND_ACC_INTERFACE;

	zend_class_entry inotification_ce;
	INIT_CLASS_ENTRY(inotification_ce, "INotification", puremvc_notification_iface_methods);
	puremvc_notification_iface_entry = zend_register_internal_class(&inotification_ce TSRMLS_CC);
	puremvc_notification_iface_entry->ce_flags|= ZEND_ACC_INTERFACE;

	zend_class_entry inotifier_ce;
	INIT_CLASS_ENTRY(inotifier_ce, "INotifier", puremvc_notifier_iface_methods);
	puremvc_notifier_iface_entry = zend_register_internal_class(&inotifier_ce TSRMLS_CC);
	puremvc_notifier_iface_entry->ce_flags|= ZEND_ACC_INTERFACE;

	zend_class_entry iobserver_ce;
	INIT_CLASS_ENTRY(iobserver_ce, "IObserver", puremvc_observer_iface_methods);
	puremvc_observer_iface_entry = zend_register_internal_class(&iobserver_ce TSRMLS_CC);
	puremvc_observer_iface_entry->ce_flags|= ZEND_ACC_INTERFACE;

	zend_class_entry iproxy_ce;
	INIT_CLASS_ENTRY(iproxy_ce, "IProxy", puremvc_proxy_iface_methods);
	puremvc_proxy_iface_entry = zend_register_internal_class(&iproxy_ce TSRMLS_CC);
	puremvc_proxy_iface_entry->ce_flags|= ZEND_ACC_INTERFACE;

	zend_class_entry iview_ce;
	INIT_CLASS_ENTRY(iview_ce, "IView", puremvc_view_iface_methods);
	puremvc_view_iface_entry = zend_register_internal_class(&iview_ce TSRMLS_CC);
	puremvc_view_iface_entry->ce_flags|= ZEND_ACC_INTERFACE;

	/* core classes */
	zend_class_entry controller_ce;
	INIT_CLASS_ENTRY(controller_ce, "Controller", puremvc_controller_class_methods);
	puremvc_controller_ce = zend_register_internal_class(&controller_ce TSRMLS_CC);
	//puremvc_controller_ce->create_object = puremvc_controller_object_new;
	zend_class_implements(puremvc_controller_ce TSRMLS_CC,
				1, puremvc_controller_iface_entry);
	puremvc_controller_addprops(puremvc_controller_ce TSRMLS_CC);

	zend_class_entry model_ce;
	INIT_CLASS_ENTRY(model_ce, "Model", puremvc_model_class_methods);
	puremvc_model_ce = zend_register_internal_class(&model_ce TSRMLS_CC);
	zend_class_implements(puremvc_model_ce TSRMLS_CC,
				1, puremvc_model_iface_entry);
	puremvc_model_addprops(puremvc_model_ce TSRMLS_CC);

	zend_class_entry view_ce;
	INIT_CLASS_ENTRY(view_ce, "View", puremvc_view_class_methods);
	puremvc_view_ce = zend_register_internal_class(&view_ce TSRMLS_CC);
	zend_class_implements(puremvc_view_ce TSRMLS_CC,
				1, puremvc_view_iface_entry);
	puremvc_view_addprops(puremvc_view_ce TSRMLS_CC);

	/* pattern classes */
	zend_class_entry notification_ce;
	INIT_CLASS_ENTRY(notification_ce, "Notification", puremvc_notification_class_methods);
	puremvc_notification_ce = zend_register_internal_class(&notification_ce TSRMLS_CC);
	zend_class_implements(puremvc_notification_ce TSRMLS_CC,
				1, puremvc_notification_iface_entry);
	puremvc_notification_addprops(puremvc_notification_ce TSRMLS_CC);

	zend_class_entry notifier_ce;
	INIT_CLASS_ENTRY(notifier_ce, "Notifier", puremvc_notifier_class_methods);
	puremvc_notifier_ce = zend_register_internal_class(&notifier_ce TSRMLS_CC);
	zend_class_implements(puremvc_notifier_ce TSRMLS_CC,
				1, puremvc_notifier_iface_entry);
	puremvc_notifier_addprops(puremvc_notifier_ce TSRMLS_CC);

	zend_class_entry observer_ce;
	INIT_CLASS_ENTRY(observer_ce, "Observer", puremvc_observer_class_methods);
	puremvc_observer_ce = zend_register_internal_class(&observer_ce TSRMLS_CC);
	zend_class_implements(puremvc_observer_ce TSRMLS_CC,
				1, puremvc_observer_iface_entry);
	puremvc_observer_addprops(puremvc_observer_ce TSRMLS_CC);

	zend_class_entry facade_ce;
	INIT_CLASS_ENTRY(facade_ce, "Facade", puremvc_facade_class_methods);
	puremvc_facade_ce = zend_register_internal_class(&facade_ce TSRMLS_CC);
	zend_class_implements(puremvc_facade_ce TSRMLS_CC,
				1, puremvc_facade_iface_entry);
	puremvc_facade_addprops(puremvc_facade_ce TSRMLS_CC);

	zend_class_entry macrocommand_ce;
	INIT_CLASS_ENTRY(macrocommand_ce, "MacroCommand", puremvc_macrocommand_class_methods);
	puremvc_macrocommand_ce = zend_register_internal_class_ex(&macrocommand_ce, 
				puremvc_notifier_ce, NULL TSRMLS_CC);
	zend_class_implements(puremvc_macrocommand_ce TSRMLS_CC,
				1, puremvc_command_iface_entry);
	puremvc_macrocommand_addprops(puremvc_macrocommand_ce TSRMLS_CC);

	zend_class_entry simplecommand_ce;
	INIT_CLASS_ENTRY(simplecommand_ce, "SimpleCommand", puremvc_simplecommand_class_methods);
	puremvc_simplecommand_ce = zend_register_internal_class_ex(&simplecommand_ce, 
				puremvc_notifier_ce, NULL TSRMLS_CC);
	zend_class_implements(puremvc_simplecommand_ce TSRMLS_CC,
				1, puremvc_command_iface_entry);
	puremvc_simplecommand_addprops(puremvc_simplecommand_ce TSRMLS_CC);

	zend_class_entry mediator_ce;
	INIT_CLASS_ENTRY(mediator_ce, "Mediator", puremvc_mediator_class_methods);
	puremvc_mediator_ce = zend_register_internal_class_ex(&mediator_ce,
				puremvc_notifier_ce, NULL TSRMLS_CC);
	zend_class_implements(puremvc_mediator_ce TSRMLS_CC,
				1, puremvc_mediator_iface_entry);
	puremvc_mediator_addconstants(puremvc_mediator_ce);
	puremvc_mediator_addprops(puremvc_mediator_ce TSRMLS_CC);

	zend_class_entry proxy_ce;
	INIT_CLASS_ENTRY(proxy_ce, "Proxy", puremvc_proxy_class_methods);
	puremvc_proxy_ce = zend_register_internal_class_ex(&proxy_ce,
				puremvc_notifier_ce, NULL TSRMLS_CC);
	zend_class_implements(puremvc_proxy_ce TSRMLS_CC,
				1, puremvc_proxy_iface_entry);
	puremvc_proxy_addconstants(puremvc_proxy_ce);
	puremvc_proxy_addprops(puremvc_proxy_ce TSRMLS_CC);
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
