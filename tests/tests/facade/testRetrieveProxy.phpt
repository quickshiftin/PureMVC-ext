--TEST--
ensure Facade::retrieveProxy passes the $proxyName to the controller
--FILE--
<?php
include(dirname(__FILE__) . '/../../testlib/include.php');
MyProxy::disableBlabOnRegister();
MyModel::useMyProxy();
$f = MyFacade::getInstance();
echo get_class($f->retrieveProxy('testProxy'));
?>
--EXPECT--
MyProxy
