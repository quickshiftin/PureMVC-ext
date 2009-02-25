--TEST--
see if hasProxy() returns true when the IProxy instance is there
--FILE--
<?php
include(dirname(__FILE__) . '/../../testlib/include.php');
MyProxy::disableBlabOnRegister();
$m = Model::getInstance();
$m->registerProxy(new MyProxy());
var_dump($m->hasProxy('testProxy'));
--EXPECT--
bool(true)
