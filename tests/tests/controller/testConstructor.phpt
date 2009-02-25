--TEST--
test the constructor to see if it initializes proxyMap, and base hook initializes View and calls the hook method
--FILE--
<?php
include(dirname(__FILE__) . '/../../testlib/include.php');
MyController::enableWacky();
MyController::getInstance();
?>
--EXPECT--
object(View)#3 (2) {
  ["mediatorMap:protected"]=>
  array(0) {
  }
  ["observerMap:protected"]=>
  array(0) {
  }
}
array(0) {
}
