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
/* Controller */
PHP_METHOD(Controller, __construct)
{
}
PHP_METHOD(Controller, initializeController)
{
}
PHP_METHOD(Controller, getInstance)
{
}
PHP_METHOD(Controller, executeCommand)
{
}
PHP_METHOD(Controller, registerCommand)
{
}
PHP_METHOD(Controller, hasCommand)
{
}
PHP_METHOD(Controller, removeCommand)
{
}
/* Model */
PHP_METHOD(Model, __construct)
{
}
PHP_METHOD(Model, initializeModel)
{
}
PHP_METHOD(Model, getInstance)
{
}
PHP_METHOD(Model, registerProxy)
{
}
PHP_METHOD(Model, retrieveProxy)
{
}
PHP_METHOD(Model, removeProxy)
{
}
PHP_METHOD(Model, hasProxy)
{
}
/* View */
PHP_METHOD(View, __construct)
{
}
PHP_METHOD(View, initializeView)
{
}
PHP_METHOD(View, getInstance)
{
}
PHP_METHOD(View, registerObserver)
{
}
PHP_METHOD(View, notifyObservers)
{
}
PHP_METHOD(View, registerMediator)
{
}
PHP_METHOD(View, retrieveMediator)
{
}
PHP_METHOD(View, hasMediator)
{
}
PHP_METHOD(View, removeMediator)
{
}
/* MacroCommand */
PHP_METHOD(MacroCommand, __construct)
{
}
PHP_METHOD(MacroCommand, initializeMacroCommand)
{
}
PHP_METHOD(MacroCommand, addSubCommand)
{
}
PHP_METHOD(MacroCommand, execute)
{
}
/* SimpleCommand */
PHP_METHOD(SimpleCommand, __construct)
{
}
PHP_METHOD(SimpleCommand, execute)
{
}
/* Facade */
PHP_METHOD(Facade, __construct)
{
}
PHP_METHOD(Facade, initializeFacade)
{
}
PHP_METHOD(Facade, getInstance)
{
}
PHP_METHOD(Facade, initializeController)
{
}
PHP_METHOD(Facade, initializeModel)
{
}
PHP_METHOD(Facade, initializeView)
{
}
PHP_METHOD(Facade, notifyObservers)
{
}
PHP_METHOD(Facade, registerCommand)
{
}
PHP_METHOD(Facade, removeCommand)
{
}
PHP_METHOD(Facade, hasCommand)
{
}
PHP_METHOD(Facade, registerProxy)
{
}
PHP_METHOD(Facade, retrieveProxy)
{
}
PHP_METHOD(Facade, hasProxy)
{
}
PHP_METHOD(Facade, removeProxy)
{
}
PHP_METHOD(Facade, registerMediator)
{
}
PHP_METHOD(Facade, retrieveMediator)
{
}
PHP_METHOD(Facade, hasMediator)
{
}
PHP_METHOD(Facade, removeMediator)
{
}
PHP_METHOD(Facade, sendNotification)
{
}
/* Mediator */
PHP_METHOD(Mediator, __construct)
{
}
PHP_METHOD(Mediator, getMediatorName)
{
}
PHP_METHOD(Mediator, getViewComponent)
{
}
PHP_METHOD(Mediator, setViewComponent)
{
}
PHP_METHOD(Mediator, listNotificationInterests)
{
}
PHP_METHOD(Mediator, handleNotifications)
{
}
PHP_METHOD(Mediator, onRegister)
{
}
PHP_METHOD(Mediator, onRemove)
{
}
/* Notification */
PHP_METHOD(Notification, __construct)
{
}
PHP_METHOD(Notification, getName)
{
}
PHP_METHOD(Notification, setName)
{
}
PHP_METHOD(Notification, getBody)
{
}
PHP_METHOD(Notification, setBody)
{
}
PHP_METHOD(Notification, setType)
{
}
PHP_METHOD(Notification, getType)
{
}
PHP_METHOD(Notification, toString)
{
}
/* Notifier */
PHP_METHOD(Notifier, __construct)
{
}
PHP_METHOD(Notifier, sendNotification)
{
}
/* Observer */
PHP_METHOD(Observer, __construct)
{
}
PHP_METHOD(Observer, setNotifyMethod)
{
}
PHP_METHOD(Observer, setNotifyContext)
{
}
PHP_METHOD(Observer, getNotifyMethod)
{
}
PHP_METHOD(Observer, getNotifyContext)
{
}
PHP_METHOD(Observer, notifyObserver)
{
}
PHP_METHOD(Observer, compareNotifyContext)
{
}
/* Proxy */
PHP_METHOD(Proxy, __construct)
{
}
PHP_METHOD(Proxy, getProxyName)
{
}
PHP_METHOD(Proxy, setData)
{
}
PHP_METHOD(Proxy, getData)
{
}
PHP_METHOD(Proxy, onRegister)
{
}
PHP_METHOD(Proxy, onRemove)
{
}
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
ZEND_BEGIN_ARG_INFO_EX(arginfo_icontroller_registerCommand, 0, 0, 2)
	ZEND_ARG_INFO(0, notificationName)
	ZEND_ARG_INFO(0, commandClassRef)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_icontroller_executeCommand, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, "notification", "INotification", 0)
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
	ZEND_ARG_OBJ_INFO(0, "proxy", "IProxy", 0)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_ifacade_registerCommand, 0, 0, 2)
	ZEND_ARG_INFO(0, noteName)
	ZEND_ARG_INFO(0, commandClassRef)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_ifacade_notifyObservers, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, "note", "INotification", 0)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_ifacade_registerMediator, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, "mediator", "IMediator", 0)
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
	ZEND_ARG_OBJ_INFO(0, "notification", "INotification", 0)
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
	ZEND_ARG_OBJ_INFO(0, "proxy", "IProxy", 0)
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
ZEND_BEGIN_ARG_INFO_EX(arginfo_iobserver_setNotifyContent, 0, 0, 1)
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
	PHP_ABSTRACT_ME(IObserver, setNotifyContent, arginfo_iobserver_setNotifyContent)
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
	ZEND_ARG_OBJ_INFO(0, "note", "INotification", 0)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_iview_registerMediator, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, "mediator", "IMediator", 0)
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
	ZEND_ARG_OBJ_INFO(0, "note", "INotification", 0)
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
void puremvc_controller_addprops(zend_class_entry *controller_ce, TSRMLS_D)
{
	/* proptected */
	zend_declare_property_null(controller_ce, "instance", sizeof("instance")-1,
				ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(controller_ce, "view", sizeof("view")-1,
				ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(controller_ce, "commandMap", sizeof("commandMap")-1,
				ZEND_ACC_PROTECTED TSRMLS_CC);
}
/* Model */
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_model_class_registerProxy, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, "proxy", "IProxy", 0)
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

void puremvc_model_addprops(zend_class_entry *model_ce, TSRMLS_D)
{
	/* proptected */
	zend_declare_property_null(model_ce, "instance", sizeof("instance")-1,
			ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(model_ce, "proxyMap", sizeof("proxyMap")-1,
			 ZEND_ACC_PROTECTED TSRMLS_CC);
}
/* View */
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_view_registerObserver, 0, 0, 2)
	ZEND_ARG_INFO(0, notificationName)
	ZEND_ARG_OBJ_INFO(0, "observer", "IObserver", 0)
ZEND_END_ARG_INFO()
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_view_notifyObservers, 0, 0 , 1)
	ZEND_ARG_OBJ_INFO(0, "notefication", "INotification", 0)
ZEND_END_ARG_INFO();
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_view_registerMediator, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, "mediator", "IMediator", 0)
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
void puremvc_view_addprops(zend_class_entry *view_ce, TSRMLS_D)
{
	/* proptected */
	zend_declare_property_null(view_ce, "instance", sizeof("instance")-1,
				ZEND_ACC_PROTECTED TSRMLS_CC);
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
	ZEND_ARG_OBJ_INFO(0, "notification", "INotification", 0)
ZEND_END_ARG_INFO()
static function_entry puremvc_macrocommand_class_methods[] = {
	PHP_ME(MacroCommand, __construct, NULL, ZEND_ACC_CTOR|ZEND_ACC_PUBLIC)
	PHP_ME(MacroCommand, initializeMacroCommand, NULL, ZEND_ACC_PROTECTED)
	PHP_ME(MacroCommand, addSubCommand, arginfo_macrocommand_addSubCommand, ZEND_ACC_PROTECTED)
	PHP_ME(MacroCommand, execute, arginfo_macrocommand_execute, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	{ NULL, NULL, NULL }
};
void puremvc_macrocommand_addprops(zend_class_entry *macrocommand_ce, TSRMLS_D)
{
	// private /
	zend_declare_property_null(macrocommand_ce, "subCommands", sizeof("subCommands")-1,
			ZEND_ACC_PRIVATE TSRMLS_CC);
}
/* SimpleCommand */
ZEND_BEGIN_ARG_INFO_EX(arginfo_simplecommand_execute, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, "notification", "INotification", 0)
ZEND_END_ARG_INFO()
static function_entry puremvc_simplecommand_class_methods[] = {
	PHP_ME(SimpleCommand, __construct, NULL, ZEND_ACC_CTOR|ZEND_ACC_PUBLIC)
	PHP_ME(SimpleCommand, execute, arginfo_simplecommand_execute, ZEND_ACC_PUBLIC)
	{ NULL, NULL, NULL }
};
void puremvc_simplecommand_addprops(zend_class_entry *simplecommand_ce, TSRMLS_D)
{
	// protected /
	zend_declare_property_null(simplecommand_ce, "facade", sizeof("facade")-1,
				ZEND_ACC_PROTECTED TSRMLS_CC);
}
/* Facade */
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_facade_notifyObservers, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, "notification", "INotification", 0)
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
	ZEND_ARG_OBJ_INFO(0, "proxy", "IProxy", 0)
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
void puremvc_facade_addprops(zend_class_entry *facade_ce, TSRMLS_D)
{
	// protected /
	zend_declare_property_string(facade_ce, "model", sizeof("model")-1,
				"", ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_string(facade_ce, "view", sizeof("view")-1,
				"", ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_string(facade_ce, "controller", sizeof("controller")-1,
				"", ZEND_ACC_PROTECTED TSRMLS_CC);
}
/* Mediator */
ZEND_BEGIN_ARG_INFO_EX(arginfo_mediator_setViewComponent, 0, 0, 1)
	ZEND_ARG_INFO(0, component)
ZEND_END_ARG_INFO()
static function_entry puremvc_mediator_class_methods[] = {
	PHP_ME(Mediator, __construct, NULL, ZEND_ACC_CTOR|ZEND_ACC_PUBLIC)
	PHP_ME(Mediator, getMediatorName, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Mediator, getViewComponent, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Mediator, setViewComponent, arginfo_mediator_setViewComponent, ZEND_ACC_PUBLIC)
	PHP_ME(Mediator, listNotificationInterests, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Mediator, handleNotifications, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Mediator, onRegister, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Mediator, onRemove, NULL, ZEND_ACC_PUBLIC)
	{ NULL, NULL, NULL }
};
void puremvc_mediator_addprops(zend_class_entry *mediator_ce, TSRMLS_D)
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
void puremvc_notification_addprops(zend_class_entry *notification_ce, TSRMLS_D)
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
void puremvc_notifier_addprops(zend_class_entry *notifier_ce, TSRMLS_D)
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
void puremvc_observer_addprops(zend_class_entry *observer_ce, TSRMLS_D)
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
void puremvc_proxy_addprops(zend_class_entry *proxy_ce, TSRMLS_D)
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
/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(pure_mvc)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	zend_class_entry ce;
	/* interfaces */
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

	/* core classes */
	INIT_CLASS_ENTRY(ce, "Controller", puremvc_controller_class_methods);
	puremvc_controller_ce = zend_register_internal_class(&ce TSRMLS_CC);
	//puremvc_controller_ce->create_object = puremvc_controller_object_new;
	zend_class_implements(puremvc_controller_ce TSRMLS_CC,
				1, puremvc_controller_iface_entry);
	puremvc_controller_addprops(puremvc_controller_ce TSRMLS_CC);

	INIT_CLASS_ENTRY(ce, "Model", puremvc_model_class_methods);
	puremvc_model_ce = zend_register_internal_class(&ce TSRMLS_CC);
	zend_class_implements(puremvc_model_ce TSRMLS_CC,
				1, puremvc_model_iface_entry);
	puremvc_model_addprops(puremvc_model_ce TSRMLS_CC);

	INIT_CLASS_ENTRY(ce, "View", puremvc_view_class_methods);
	puremvc_view_ce = zend_register_internal_class(&ce TSRMLS_CC);
	zend_class_implements(puremvc_view_ce TSRMLS_CC,
				1, puremvc_view_iface_entry);
	puremvc_view_addprops(puremvc_view_ce TSRMLS_CC);

	/* pattern classes */
	INIT_CLASS_ENTRY(ce, "Notification", puremvc_notification_class_methods);
	puremvc_notification_ce = zend_register_internal_class(&ce TSRMLS_CC);
	zend_class_implements(puremvc_notification_ce TSRMLS_CC,
				1, puremvc_notification_iface_entry);
	puremvc_notification_addprops(puremvc_notification_ce TSRMLS_CC);

	INIT_CLASS_ENTRY(ce, "Notifier", puremvc_notifier_class_methods);
	puremvc_notifier_ce = zend_register_internal_class(&ce TSRMLS_CC);
	zend_class_implements(puremvc_notifier_ce TSRMLS_CC,
				1, puremvc_notifier_iface_entry);
	puremvc_notifier_addprops(puremvc_notifier_ce TSRMLS_CC);

	INIT_CLASS_ENTRY(ce, "Observer", puremvc_observer_class_methods);
	puremvc_observer_ce = zend_register_internal_class(&ce TSRMLS_CC);
	zend_class_implements(puremvc_observer_ce TSRMLS_CC,
				1, puremvc_observer_iface_entry);
	puremvc_observer_addprops(puremvc_observer_ce TSRMLS_CC);

	INIT_CLASS_ENTRY(ce, "Facade", puremvc_facade_class_methods);
	puremvc_facade_ce = zend_register_internal_class(&ce TSRMLS_CC);
	zend_class_implements(puremvc_facade_ce TSRMLS_CC,
				1, puremvc_facade_iface_entry);
	puremvc_facade_addprops(puremvc_facade_ce TSRMLS_CC);

	INIT_CLASS_ENTRY(ce, "MacroCommand", puremvc_macrocommand_class_methods);
	puremvc_macrocommand_ce = zend_register_internal_class_ex(&ce, 
				puremvc_notifier_ce, NULL TSRMLS_CC);
	zend_class_implements(puremvc_macrocommand_ce TSRMLS_CC,
				1, puremvc_command_iface_entry);
	puremvc_macrocommand_addprops(puremvc_macrocommand_ce TSRMLS_CC);

	INIT_CLASS_ENTRY(ce, "SimpleCommand", puremvc_simplecommand_class_methods);
	puremvc_simplecommand_ce = zend_register_internal_class_ex(&ce, 
				puremvc_notifier_ce, NULL TSRMLS_CC);
	zend_class_implements(puremvc_simplecommand_ce TSRMLS_CC,
				1, puremvc_command_iface_entry);
	puremvc_simplecommand_addprops(puremvc_simplecommand_ce TSRMLS_CC);

	INIT_CLASS_ENTRY(ce, "Mediator", puremvc_mediator_class_methods);
	puremvc_mediator_ce = zend_register_internal_class_ex(&ce,
				puremvc_notifier_ce, NULL TSRMLS_CC);
	zend_class_implements(puremvc_mediator_ce TSRMLS_CC,
				1, puremvc_mediator_iface_entry);
	puremvc_mediator_addconstants(puremvc_mediator_ce);
	puremvc_mediator_addprops(puremvc_mediator_ce TSRMLS_CC);

	INIT_CLASS_ENTRY(ce, "Proxy", puremvc_proxy_class_methods);
	puremvc_proxy_ce = zend_register_internal_class_ex(&ce,
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
