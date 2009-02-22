--TEST--
ensure Facade::hasProxy passes the $notificationName to the controller
--FILE--
<?php
include(dirname(__FILE__) . '/../../testlib/include.php');
$f = MyFacade::getInstance();
$f->hasProxy('MyNotification');
?>
--EXPECT--
MyNotification
