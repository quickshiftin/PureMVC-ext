--TEST--
ensure we can store a command via Facade::registerMediator
--FILE--
<?php
include(dirname(__FILE__) . '/../../testlib/include.php');
MyView::makeVerbose();
$f = MyFacade::getInstance();
$f->registerMediator(new Mediator('testMediator'));
--EXPECT--
Mediator
