--TEST--
ensure Facade::removeMediator passes the $mediatorName to the view
--FILE--
<?php
include(dirname(__FILE__) . '/../testlib/include.php');
$f = MyFacade::getInstance();
$f->removeMediator('MyNotification');
?>
--EXPECT--
MyNotification
