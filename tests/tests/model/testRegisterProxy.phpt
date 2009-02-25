--TEST--
test registerProxy to see if we add the IProxy instance and call its onRegister() method
--FILE--
<?php
include(dirname(__FILE__) . '/../../testlib/include.php');
$m = Model::getInstance();
$m->registerProxy(new MyProxy());
?>
--EXPECT--
testProxy
