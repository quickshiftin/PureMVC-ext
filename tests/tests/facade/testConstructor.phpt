--TEST--
ensure facade constructor calls $this->initializeFacade()
--FILE--
<?php
include(dirname(__FILE__) . '/../../testlib/include.php');
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
  object(View)#5 (2) {
    ["mediatorMap:protected"]=>
    array(0) {
    }
    ["observerMap:protected"]=>
    array(0) {
    }
  }
  ["controller:protected"]=>
  object(Controller)#3 (2) {
    ["view:protected"]=>
    object(View)#5 (2) {
      ["mediatorMap:protected"]=>
      array(0) {
      }
      ["observerMap:protected"]=>
      array(0) {
      }
    }
    ["commandMap:protected"]=>
    array(0) {
    }
  }
}
