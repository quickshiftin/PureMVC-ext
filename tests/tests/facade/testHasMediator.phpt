--TEST--
ensure Facade::hasMediator passes the $notificationName to the controller
--FILE--
<?php
include(dirname(__FILE__) . '/../../testlib/include.php');
$f = MyFacade::getInstance();
$f->hasMediator('MyNotification');
?>
--EXPECT--
MyNotification
