--TEST--
see if the hasMediator method returns true, when we know the mediator has been registered
--FILE--
<?php
include(dirname(__FILE__) . '/../../testlib/include.php');
MyMediator::makeVerbose();
MyView::showName();
$view = MyView::getInstance();
$view->registerMediator(new MyMediator());
?>
--EXPECT--
MyMediator::listNotificationInterests
notificationName: MyNotification
notificationName: MyOtherNotification
MyMediator::onRegister
