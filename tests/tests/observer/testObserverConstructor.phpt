--TEST--
verify setNotifyMethod()/setNotifyContext() are called and instance vals are set
--FILE--
<?php
class Something {
	public function notify($notification) {
		if($notification->getName() == 'testEvent')
			echo 'testEvent recieved' . PHP_EOL;
	}
}
class MyObserver extends Observer {}
$s = new Something();
var_dump(new MyObserver('notify', $s))
?>
--EXPECT--
object(MyObserver)#2 (2) {
  ["notify:private"]=>
  string(6) "notify"
  ["context:private"]=>
  object(Something)#1 (0) {
  }
}
