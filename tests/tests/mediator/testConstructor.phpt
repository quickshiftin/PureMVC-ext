--TEST--
lets make sure the constructor runs
--FILE--
<?php
/// not sure what we should do here atm..
// var_dump(new Mediator());

var_dump(new Mediator(null));
var_dump(new Mediator("blah"));
var_dump(null, new stdClass());
var_dump("blah", new stdClass());
?>
--EXPECT--
object(Mediator)#1 (3) {
  ["facade:protected"]=>
  object(Facade)#2 (3) {
    ["model:protected"]=>
    object(Model)#3 (1) {
      ["proxyMap:protected"]=>
      array(0) {
      }
    }
    ["view:protected"]=>
    object(View)#6 (2) {
      ["mediatorMap:protected"]=>
      array(0) {
      }
      ["observerMap:protected"]=>
      array(0) {
      }
    }
    ["controller:protected"]=>
    object(Controller)#4 (2) {
      ["view:protected"]=>
      object(View)#6 (2) {
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
  ["mediatorName:protected"]=>
  string(8) "Mediator"
  ["viewComponent:protected"]=>
  NULL
}
object(Mediator)#1 (3) {
  ["facade:protected"]=>
  object(Facade)#2 (3) {
    ["model:protected"]=>
    object(Model)#3 (1) {
      ["proxyMap:protected"]=>
      array(0) {
      }
    }
    ["view:protected"]=>
    object(View)#6 (2) {
      ["mediatorMap:protected"]=>
      array(0) {
      }
      ["observerMap:protected"]=>
      array(0) {
      }
    }
    ["controller:protected"]=>
    object(Controller)#4 (2) {
      ["view:protected"]=>
      object(View)#6 (2) {
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
  ["mediatorName:protected"]=>
  string(4) "blah"
  ["viewComponent:protected"]=>
  NULL
}
NULL
object(stdClass)#1 (0) {
}
string(4) "blah"
object(stdClass)#1 (0) {
}
