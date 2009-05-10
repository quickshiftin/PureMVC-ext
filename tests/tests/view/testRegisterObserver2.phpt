--TEST--
lets see if we can register more than one observer for the same notification
--FILE--
<?php
$v = View::getInstance();
$v->registerObserver('MyNotification',
		new Observer('executeCommand', new stdClass()));
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
    array(2) {
      [0]=>
      object(Observer)#2 (2) {
        ["notify:private"]=>
        string(14) "executeCommand"
        ["context:private"]=>
        object(stdClass)#3 (0) {
        }
      }
      [1]=>
      object(Observer)#4 (2) {
        ["notify:private"]=>
        string(14) "executeCommand"
        ["context:private"]=>
        object(stdClass)#5 (0) {
        }
      }
    }
  }
}
