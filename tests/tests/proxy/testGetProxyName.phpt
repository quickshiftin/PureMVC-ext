--TEST--
lets see if the proxy returns the corrent name in 2 cases
--FILE--
<?php
include(dirname(__FILE__) . '/../../testlib/include.php');
$pp = new Proxy();
$pp2 = new MyProxy('MyProxy');
echo $pp->getProxyName() . PHP_EOL;
echo $pp2->getProxyName() . PHP_EOL;
?>
--EXPECT--
Proxy
testProxy
