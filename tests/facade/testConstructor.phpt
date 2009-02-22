--TEST--
ensure facade constructor calls $this->initializeFacade()
--FILE--
<?php
include(dirname(__FILE__) . '/../testlib/include.php');
MyFacade::enableCallPAndE();
var_dump(MyFacade::getInstance());
?>
--EXPECT--
Model
Controller
View
Facade
object(MyFacade)#1 (3) {
  ["model:protected"]=>
  object(Model)#2 (1) {
    ["proxyMap:protected"]=>
    array(0) {
    }
  }
  ["view:protected"]=>
  object(View)#4 (2) {
    ["mediatorMap:protected"]=>
    NULL
    ["observerMap:protected"]=>
    NULL
  }
  ["controller:protected"]=>
  object(Controller)#3 (2) {
    ["view:protected"]=>
    NULL
    ["commandMap:protected"]=>
    NULL
  }
}
