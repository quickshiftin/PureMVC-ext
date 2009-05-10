--TEST--
put an IProxy instance in proxyMap, and see if we can get it back out via retrieveProxy
--FILE--
<?php
include(dirname(__FILE__) . '/../../testlib/include.php');
MyProxy::disableBlabOnRegister();
MyModel::useMyProxy();
$m = MyModel::getInstance();
var_dump($m);
var_dump($m->retrieveProxy('testProxy'));
?>
--EXPECT--
object(MyModel)#1 (1) {
  ["proxyMap:protected"]=>
  array(1) {
    ["testProxy"]=>
    object(MyProxy)#2 (0) {
    }
  }
}
object(MyModel)#1 (1) {
  ["proxyMap:protected"]=>
  array(1) {
    ["testProxy"]=>
    object(MyProxy)#2 (0) {
    }
  }
}
object(MyProxy)#2 (0) {
}
