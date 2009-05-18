--TEST--
ensure Facade::retrieveMediator passes the $mediatorName to the view
--FILE--
<?php
include(dirname(__FILE__) . '/../../testlib/include.php');
MyView::makeVerbose();
$f = MyFacade::getInstance();
$f->retrieveMediator('MyMediator');
?>
--EXPECT--
MyMediator
