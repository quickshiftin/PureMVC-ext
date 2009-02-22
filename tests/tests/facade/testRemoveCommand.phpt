--TEST--
ensure Facade::removeCommand passes the $commandName to the controller
--FILE--
<?php
include(dirname(__FILE__) . '/../testlib/include.php');
$f = MyFacade::getInstance();
$f->removeCommand('MyNotification');
?>
--EXPECT--
MyNotification
