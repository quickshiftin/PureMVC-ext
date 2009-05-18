--TEST--
run through the registerMediator flow and ensure we hit points wew're supposed to
--FILE--
<?php
include(dirname(__FILE__) . '/../../testlib/include.php');
$view = MyView::getInstance();
$view->registerMediator(new MyMediator());
var_dump($view->hasMediator('Mediator'));
?>
--EXPECT--
bool(true)
