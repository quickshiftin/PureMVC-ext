--TEST--
the all-inclusive View::removeMediator() test..
--FILE--
<?php
include(dirname(__FILE__) . '/../../testlib/include.php');
MyMediator::makeVerbose();
MyView::showName();
$view = MyView::getInstance();
$view->registerMediator(new MyMediator());
$view->removeMediator('Mediator');
var_dump($view);
?>
--EXPECT--
MyMediator::listNotificationInterests
notificationName: MyNotification
notificationName: MyOtherNotification
MyMediator::onRegister
object(MyView)#1 (2) {
  ["mediatorMap:protected"]=>
  array(0) {
  }
  ["observerMap:protected"]=>
  array(0) {
  }
}
