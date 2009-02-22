--TEST--
ensure Facade::removeProxy passes the $proxyName to the controller
--FILE--
<?php
include(dirname(__FILE__) . '/../../testlib/include.php');
$f = MyFacade::getInstance();
$f->removeProxy('MyNotification');
?>
--EXPECT--
MyNotification
