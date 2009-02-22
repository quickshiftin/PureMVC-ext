--TEST--
ensure Facade::hasCommand passes the $notificationName to the controller
--FILE--
<?php
include(dirname(__FILE__) . '/../testlib/include.php');
$f = MyFacade::getInstance();
$f->hasCommand('MyNotification');
?>
--EXPECT--
MyNotification
