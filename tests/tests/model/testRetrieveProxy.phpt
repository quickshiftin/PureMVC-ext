--TEST--
put an IProxy instance in proxyMap, and see if we can get it back out via retrieveProxy
--FILE--
<?php
include(dirname(__FILE__) . '/../../testlib/include.php');
MyProxy::disableBlabOnRegister();
MyModel::useMyProxy();
$m = MyModel::getInstance();
echo get_class($m->retrieveProxy('testProxy'));
?>
--EXPECT--
MyProxy
