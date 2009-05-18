--TEST--
see if the Mediator is there after we've registered it
--FILE--
<?php
include(dirname(__FILE__) . '/../../testlib/include.php');
$view = MyView::getInstance();
$view->registerMediator(new MyMediator());
echo get_class($view->retrieveMediator('Mediator'));
?>
--EXPECT--
MyMediator
