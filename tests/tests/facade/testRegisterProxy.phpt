--TEST--
ensure we can store a command via Facade::registerProxy
--FILE--
<?php
include(dirname(__FILE__) . '/../../testlib/include.php');
$f = MyFacade::getInstance();
$f->registerProxy(new Proxy());
--EXPECT--
Proxy
