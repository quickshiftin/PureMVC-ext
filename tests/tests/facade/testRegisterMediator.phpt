--TEST--
ensure we can store a command via Facade::registerMediator
--FILE--
<?php
include(dirname(__FILE__) . '/../../testlib/include.php');
$f = Facade::getInstance();
$f->registerMediator(new Mediator());
--EXPECT--
