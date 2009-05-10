--TEST--
lets see if we can register just one Observer
--FILE--
<?php
$v = View::getInstance();
$v->registerObserver('MyNotification',
		new Observer('executeCommand', new stdClass()));
var_dump($v);
?>
--EXPECT--
object(View)#1 (2) {
  ["mediatorMap:protected"]=>
  array(0) {
  }
  ["observerMap:protected"]=>
  array(1) {
    ["MyNotification"]=>
    array(1) {
      [0]=>
      object(Observer)#2 (2) {
        ["notify:private"]=>
        string(14) "executeCommand"
        ["context:private"]=>
        object(stdClass)#3 (0) {
        }
      }
    }
  }
}
