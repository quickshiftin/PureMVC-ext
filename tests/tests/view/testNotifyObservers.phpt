--TEST--
see if the observers actually work when they are notified
--FILE--
<?php
include(dirname(__FILE__) . '/../../testlib/include.php');
MyMediator::makeVerbose();
$view = MyView::getInstance();
$view->registerMediator(new MyMediator());
$n = new Notification('MyNotification');
$view->notifyObservers($n);
?>
--EXPECT--
MyMediator::listNotificationInterests
MyMediator::onRegister
MyNotification
