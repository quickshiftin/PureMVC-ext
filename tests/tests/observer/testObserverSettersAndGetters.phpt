--TEST--
lets test the setters / getters on the Observer
--FILE--
<?php
class Something {
	public function notify($notification) {
		if($notification->getName() == 'testEvent')
			echo 'testEvent recieved' . PHP_EOL;
	}
}
class SomethingElse {}
class MyObserver extends Observer {}
$o = new MyObserver('notify', new Something());
echo $o->getNotifyMethod() . PHP_EOL;
var_dump($o->getNotifyContext());
$o->setNotifyMethod('update');
$o->setNotifyContext(new SomethingElse());
echo $o->getNotifyMethod() . PHP_EOL;
var_dump($o->getNotifyContext());
?>
--EXPECT--
notify
object(Something)#2 (0) {
}
update
object(SomethingElse)#3 (0) {
}
