--TEST--
ensure Facade::hasMediator passes the $notificationName to the controller
--FILE--
<?php
include(dirname(__FILE__) . '/../../testlib/include.php');
MyView::makeVerbose();
$f = MyFacade::getInstance();
$f->hasMediator('MyMediator');
?>
--EXPECT--
MyMediator
