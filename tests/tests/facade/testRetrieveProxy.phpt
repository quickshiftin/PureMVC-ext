--TEST--
ensure Facade::retrieveProxy passes the $proxyName to the controller
--FILE--
<?php
include(dirname(__FILE__) . '/../../testlib/include.php');
$f = MyFacade::getInstance();
$f->retrieveProxy('MyNotification');
?>
--EXPECT--
MyNotification
