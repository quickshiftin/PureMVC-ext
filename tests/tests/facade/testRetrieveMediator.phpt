--TEST--
ensure Facade::retrieveMediator passes the $mediatorName to the view
--FILE--
<?php
include(dirname(__FILE__) . '/../../testlib/include.php');
$f = MyFacade::getInstance();
$f->retrieveMediator('MyNotification');
?>
--EXPECT--
MyNotification
